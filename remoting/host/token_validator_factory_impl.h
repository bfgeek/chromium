// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_TOKEN_VALIDATOR_FACTORY_IMPL_H_
#define REMOTING_HOST_TOKEN_VALIDATOR_FACTORY_IMPL_H_

#include <set>
#include <string>

#include "base/macros.h"
#include "net/url_request/url_request_context_getter.h"
#include "remoting/host/token_validator_base.h"
#include "remoting/protocol/token_validator.h"

namespace remoting {

// This class dispenses |TokenValidator| implementations that use a UrlFetcher
// to contact a |token_validation_url| and exchange the |token| for a
// |shared_secret|.
class TokenValidatorFactoryImpl : public protocol::TokenValidatorFactory {
 public:
  // Creates a new factory. |token_url| and |token_validation_url| are the
  // third party authentication service URLs, obtained via policy. |key_pair_|
  // is used by the host to authenticate with the service by signing the token.
  TokenValidatorFactoryImpl(
      const ThirdPartyAuthConfig& third_party_auth_config,
      scoped_refptr<RsaKeyPair> key_pair,
      scoped_refptr<net::URLRequestContextGetter> request_context_getter);

  // TokenValidatorFactory interface.
  scoped_ptr<protocol::TokenValidator> CreateTokenValidator(
      const std::string& local_jid,
      const std::string& remote_jid) override;

 private:
  ~TokenValidatorFactoryImpl() override;

  ThirdPartyAuthConfig third_party_auth_config_;
  scoped_refptr<RsaKeyPair> key_pair_;
  scoped_refptr<net::URLRequestContextGetter> request_context_getter_;

  DISALLOW_COPY_AND_ASSIGN(TokenValidatorFactoryImpl);
};

}  // namespace remoting

#endif  // REMOTING_HOST_URL_FETCHER_TOKEN_VALIDATOR_FACTORY_H_
