// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_PROTOCOL_THIRD_PARTY_HOST_AUTHENTICATOR_H_
#define REMOTING_PROTOCOL_THIRD_PARTY_HOST_AUTHENTICATOR_H_

#include <string>

#include "base/callback.h"
#include "base/macros.h"
#include "base/memory/scoped_ptr.h"
#include "remoting/protocol/third_party_authenticator_base.h"

namespace remoting {
namespace protocol {

class TokenValidator;

// Implements the host side of the third party authentication mechanism.
// The host authenticator sends the |token_url| and |scope| obtained from the
// |TokenValidator| to the client, and expects a |token| in response.
// Once that token is received, it calls |TokenValidator| asynchronously to
// validate it, and exchange it for a |shared_secret|. Once the |TokenValidator|
// returns, the host uses the |shared_secret| to create an underlying
// SPAKE2 authenticator, which is used to establish the encrypted connection.
class ThirdPartyHostAuthenticator : public ThirdPartyAuthenticatorBase {
 public:
  // Creates a third-party host authenticator.
  // |create_base_authenticator_callback| is used to create the base
  // authenticator. |token_validator| contains the token parameters to be sent
  // to the client and is used to obtain the shared secret.
  ThirdPartyHostAuthenticator(
      const CreateBaseAuthenticatorCallback& create_base_authenticator_callback,
      scoped_ptr<TokenValidator> token_validator);
  ~ThirdPartyHostAuthenticator() override;

 protected:
  // ThirdPartyAuthenticator implementation.
  void ProcessTokenMessage(const buzz::XmlElement* message,
                           const base::Closure& resume_callback) override;
  void AddTokenElements(buzz::XmlElement* message) override;

 private:
  void OnThirdPartyTokenValidated(const buzz::XmlElement* message,
                                  const base::Closure& resume_callback,
                                  const std::string& shared_secret);

  CreateBaseAuthenticatorCallback create_base_authenticator_callback_;
  scoped_ptr<TokenValidator> token_validator_;

  DISALLOW_COPY_AND_ASSIGN(ThirdPartyHostAuthenticator);
};

}  // namespace protocol
}  // namespace remoting

#endif  // REMOTING_PROTOCOL_THIRD_PARTY_HOST_AUTHENTICATOR_H_
