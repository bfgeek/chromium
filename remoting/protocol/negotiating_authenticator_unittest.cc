// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/bind.h"
#include "base/macros.h"
#include "net/base/net_errors.h"
#include "remoting/base/rsa_key_pair.h"
#include "remoting/protocol/auth_util.h"
#include "remoting/protocol/authenticator_test_base.h"
#include "remoting/protocol/channel_authenticator.h"
#include "remoting/protocol/connection_tester.h"
#include "remoting/protocol/negotiating_authenticator_base.h"
#include "remoting/protocol/negotiating_client_authenticator.h"
#include "remoting/protocol/negotiating_host_authenticator.h"
#include "remoting/protocol/pairing_registry.h"
#include "remoting/protocol/protocol_mock_objects.h"
#include "testing/gmock/include/gmock/gmock.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "third_party/webrtc/libjingle/xmllite/xmlelement.h"

using testing::_;
using testing::DeleteArg;
using testing::SaveArg;

namespace remoting {
namespace protocol {

namespace {

const int kMessageSize = 100;
const int kMessages = 1;

const char kNoClientId[] = "";
const char kNoPairedSecret[] = "";
const char kTestClientName[] = "client-name";
const char kTestClientId[] = "client-id";
const char kTestHostId[] = "12345678910123456";

const char kClientJid[] = "alice@gmail.com/abc";
const char kHostJid[] = "alice@gmail.com/123";

const char kTestPairedSecret[] = "1111-2222-3333";
const char kTestPairedSecretBad[] = "4444-5555-6666";
const char kTestPin[] = "123456";
const char kTestPinBad[] = "654321";

}  // namespace

class NegotiatingAuthenticatorTest : public AuthenticatorTestBase {
 public:
  NegotiatingAuthenticatorTest() {}
  ~NegotiatingAuthenticatorTest() override {}

 protected:
  void InitAuthenticators(const std::string& client_id,
                          const std::string& client_paired_secret,
                          const std::string& client_interactive_pin,
                          const std::string& host_secret,
                          bool it2me) {
    if (it2me) {
      host_ = NegotiatingHostAuthenticator::CreateForIt2Me(
          kHostJid, kClientJid, host_cert_, key_pair_, host_secret);
    } else {
      std::string host_secret_hash =
          GetSharedSecretHash(kTestHostId, host_secret);
      host_ = NegotiatingHostAuthenticator::CreateWithPin(
          kHostJid, kClientJid, host_cert_, key_pair_, host_secret_hash,
          pairing_registry_);
    }

    protocol::ClientAuthenticationConfig client_auth_config;
    client_auth_config.host_id = kTestHostId;
    client_auth_config.pairing_client_id = client_id;
    client_auth_config.pairing_secret= client_paired_secret;
    bool pairing_expected = pairing_registry_.get() != nullptr;
    client_auth_config.fetch_secret_callback =
        base::Bind(&NegotiatingAuthenticatorTest::FetchSecret,
                   client_interactive_pin, pairing_expected);
    client_as_negotiating_authenticator_ = new NegotiatingClientAuthenticator(
        kClientJid, kHostJid, client_auth_config);
    client_.reset(client_as_negotiating_authenticator_);
  }

  void CreatePairingRegistry(bool with_paired_client) {
    pairing_registry_ = new SynchronousPairingRegistry(
        make_scoped_ptr(new MockPairingRegistryDelegate()));
    if (with_paired_client) {
      PairingRegistry::Pairing pairing(
          base::Time(), kTestClientName, kTestClientId, kTestPairedSecret);
      pairing_registry_->AddPairing(pairing);
    }
  }

  static void FetchSecret(
      const std::string& client_secret,
      bool pairing_supported,
      bool pairing_expected,
      const protocol::SecretFetchedCallback& secret_fetched_callback) {
    secret_fetched_callback.Run(client_secret);
    ASSERT_EQ(pairing_supported, pairing_expected);
  }

  void VerifyRejected(Authenticator::RejectionReason reason) {
    ASSERT_TRUE(client_->state() == Authenticator::REJECTED ||
                host_->state() == Authenticator::REJECTED);
    if (client_->state() == Authenticator::REJECTED) {
      ASSERT_EQ(client_->rejection_reason(), reason);
    }
    if (host_->state() == Authenticator::REJECTED) {
      ASSERT_EQ(host_->rejection_reason(), reason);
    }
  }

  void VerifyAccepted(NegotiatingAuthenticatorBase::Method expected_method) {
    ASSERT_NO_FATAL_FAILURE(RunAuthExchange());

    ASSERT_EQ(Authenticator::ACCEPTED, host_->state());
    ASSERT_EQ(Authenticator::ACCEPTED, client_->state());

    client_auth_ = client_->CreateChannelAuthenticator();
    host_auth_ = host_->CreateChannelAuthenticator();
    RunChannelAuth(false);

    EXPECT_TRUE(client_socket_.get() != nullptr);
    EXPECT_TRUE(host_socket_.get() != nullptr);

    StreamConnectionTester tester(host_socket_.get(), client_socket_.get(),
                                  kMessageSize, kMessages);

    tester.Start();
    message_loop_.Run();
    tester.CheckResults();
    EXPECT_EQ(expected_method,
              client_as_negotiating_authenticator_->current_method_);
  }

  // Use a bare pointer because the storage is managed by the base class.
  NegotiatingClientAuthenticator* client_as_negotiating_authenticator_;

 private:
  scoped_refptr<PairingRegistry> pairing_registry_;

  DISALLOW_COPY_AND_ASSIGN(NegotiatingAuthenticatorTest);
};

TEST_F(NegotiatingAuthenticatorTest, SuccessfulAuthMe2MePin) {
  ASSERT_NO_FATAL_FAILURE(InitAuthenticators(kNoClientId, kNoPairedSecret,
                                             kTestPin, kTestPin, false));
  VerifyAccepted(
      NegotiatingAuthenticatorBase::Method::SHARED_SECRET_SPAKE2_CURVE25519);
}

TEST_F(NegotiatingAuthenticatorTest, SuccessfulAuthIt2me) {
  ASSERT_NO_FATAL_FAILURE(InitAuthenticators(kNoClientId, kNoPairedSecret,
                                             kTestPin, kTestPin, true));
  VerifyAccepted(
      NegotiatingAuthenticatorBase::Method::SHARED_SECRET_PLAIN_SPAKE2_P224);
}

TEST_F(NegotiatingAuthenticatorTest, InvalidMe2MePin) {
  ASSERT_NO_FATAL_FAILURE(InitAuthenticators(kNoClientId, kNoPairedSecret,
                                             kTestPinBad, kTestPin, false));
  ASSERT_NO_FATAL_FAILURE(RunAuthExchange());

  VerifyRejected(Authenticator::INVALID_CREDENTIALS);
}

TEST_F(NegotiatingAuthenticatorTest, InvalidIt2MeAccessCode) {
  ASSERT_NO_FATAL_FAILURE(InitAuthenticators(kNoClientId, kNoPairedSecret,
                                             kTestPin, kTestPinBad, true));
  ASSERT_NO_FATAL_FAILURE(RunAuthExchange());

  VerifyRejected(Authenticator::INVALID_CREDENTIALS);
}

TEST_F(NegotiatingAuthenticatorTest, IncompatibleMethods) {
  ASSERT_NO_FATAL_FAILURE(InitAuthenticators(kNoClientId, kNoPairedSecret,
                                             kTestPin, kTestPinBad, true));
  std::vector<NegotiatingAuthenticatorBase::Method>* methods =
      &(client_as_negotiating_authenticator_->methods_);
  methods->erase(std::find(
      methods->begin(), methods->end(),
      NegotiatingAuthenticatorBase::Method::SHARED_SECRET_PLAIN_SPAKE2_P224));

  ASSERT_NO_FATAL_FAILURE(RunAuthExchange());

  VerifyRejected(Authenticator::PROTOCOL_ERROR);
}

TEST_F(NegotiatingAuthenticatorTest, PairingNotSupported) {
  ASSERT_NO_FATAL_FAILURE(InitAuthenticators(kTestClientId, kTestPairedSecret,
                                             kTestPin, kTestPin, false));
  ASSERT_NO_FATAL_FAILURE(RunAuthExchange());
  VerifyAccepted(
      NegotiatingAuthenticatorBase::Method::SHARED_SECRET_SPAKE2_CURVE25519);
}

TEST_F(NegotiatingAuthenticatorTest, PairingSupportedButNotPaired) {
  CreatePairingRegistry(false);
  ASSERT_NO_FATAL_FAILURE(InitAuthenticators(kNoClientId, kNoPairedSecret,
                                             kTestPin, kTestPin, false));
  ASSERT_NO_FATAL_FAILURE(RunAuthExchange());
  VerifyAccepted(NegotiatingAuthenticatorBase::Method::PAIRED_SPAKE2_P224);
}

TEST_F(NegotiatingAuthenticatorTest, PairingRevokedPinOkay) {
  CreatePairingRegistry(false);
  ASSERT_NO_FATAL_FAILURE(InitAuthenticators(kTestClientId, kTestPairedSecret,
                                             kTestPin, kTestPin, false));
  ASSERT_NO_FATAL_FAILURE(RunAuthExchange());
  VerifyAccepted(NegotiatingAuthenticatorBase::Method::PAIRED_SPAKE2_P224);
}

TEST_F(NegotiatingAuthenticatorTest, PairingRevokedPinBad) {
  CreatePairingRegistry(false);
  ASSERT_NO_FATAL_FAILURE(InitAuthenticators(kTestClientId, kTestPairedSecret,
                                             kTestPinBad, kTestPin, false));
  ASSERT_NO_FATAL_FAILURE(RunAuthExchange());
  VerifyRejected(Authenticator::INVALID_CREDENTIALS);
}

TEST_F(NegotiatingAuthenticatorTest, PairingSucceeded) {
  CreatePairingRegistry(true);
  ASSERT_NO_FATAL_FAILURE(InitAuthenticators(kTestClientId, kTestPairedSecret,
                                             kTestPinBad, kTestPin, false));
  ASSERT_NO_FATAL_FAILURE(RunAuthExchange());
  VerifyAccepted(NegotiatingAuthenticatorBase::Method::PAIRED_SPAKE2_P224);
}

TEST_F(NegotiatingAuthenticatorTest,
       PairingSucceededInvalidSecretButPinOkay) {
  CreatePairingRegistry(true);
  ASSERT_NO_FATAL_FAILURE(InitAuthenticators(
      kTestClientId, kTestPairedSecretBad, kTestPin, kTestPin, false));
  ASSERT_NO_FATAL_FAILURE(RunAuthExchange());
  VerifyAccepted(NegotiatingAuthenticatorBase::Method::PAIRED_SPAKE2_P224);
}

TEST_F(NegotiatingAuthenticatorTest, PairingFailedInvalidSecretAndPin) {
  CreatePairingRegistry(true);
  ASSERT_NO_FATAL_FAILURE(InitAuthenticators(
      kTestClientId, kTestPairedSecretBad, kTestPinBad, kTestPin, false));
  ASSERT_NO_FATAL_FAILURE(RunAuthExchange());
  VerifyRejected(Authenticator::INVALID_CREDENTIALS);
}

}  // namespace protocol
}  // namespace remoting
