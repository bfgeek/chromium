// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_PROTOCOL_ME2ME_HOST_AUTHENTICATOR_FACTORY_H_
#define REMOTING_PROTOCOL_ME2ME_HOST_AUTHENTICATOR_FACTORY_H_

#include <string>

#include "base/compiler_specific.h"
#include "base/macros.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "remoting/protocol/authenticator.h"
#include "remoting/protocol/third_party_host_authenticator.h"
#include "remoting/protocol/token_validator.h"

namespace remoting {

class RsaKeyPair;

namespace protocol {

class PairingRegistry;

class Me2MeHostAuthenticatorFactory : public AuthenticatorFactory {
 public:
  // Create a factory that dispenses shared secret authenticators.
  static scoped_ptr<AuthenticatorFactory> CreateWithPin(
      bool use_service_account,
      const std::string& host_owner,
      const std::string& local_cert,
      scoped_refptr<RsaKeyPair> key_pair,
      const std::string& required_client_domain,
      const std::string& pin_hash,
      scoped_refptr<PairingRegistry> pairing_registry);

  // Create a factory that dispenses third party authenticators.
  static scoped_ptr<AuthenticatorFactory> CreateWithThirdPartyAuth(
      bool use_service_account,
      const std::string& host_owner,
      const std::string& local_cert,
      scoped_refptr<RsaKeyPair> key_pair,
      const std::string& required_client_domain,
      scoped_refptr<TokenValidatorFactory> token_validator_factory);

  Me2MeHostAuthenticatorFactory();
  ~Me2MeHostAuthenticatorFactory() override;

  // AuthenticatorFactory interface.
  scoped_ptr<Authenticator> CreateAuthenticator(
      const std::string& local_jid,
      const std::string& remote_jid) override;

 private:
  // Used for all host authenticators.
  bool use_service_account_;
  std::string host_owner_;
  std::string local_cert_;
  scoped_refptr<RsaKeyPair> key_pair_;
  std::string required_client_domain_;

  // Used only for PIN-based host authenticators.
  std::string pin_hash_;

  // Used only for third party host authenticators.
  scoped_refptr<TokenValidatorFactory> token_validator_factory_;

  // Used only for pairing host authenticators.
  scoped_refptr<PairingRegistry> pairing_registry_;

  DISALLOW_COPY_AND_ASSIGN(Me2MeHostAuthenticatorFactory);
};

}  // namespace protocol
}  // namespace remoting

#endif  // REMOTING_PROTOCOL_ME2ME_HOST_AUTHENTICATOR_FACTORY_H_
