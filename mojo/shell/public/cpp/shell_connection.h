// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_SHELL_PUBLIC_CPP_SHELL_CONNECTION_H_
#define MOJO_SHELL_PUBLIC_CPP_SHELL_CONNECTION_H_

#include <utility>
#include <vector>

#include "base/macros.h"
#include "base/memory/scoped_vector.h"
#include "mojo/public/cpp/bindings/binding.h"
#include "mojo/public/cpp/bindings/callback.h"
#include "mojo/public/cpp/system/core.h"
#include "mojo/shell/public/cpp/shell_client.h"
#include "mojo/shell/public/interfaces/connector.mojom.h"
#include "mojo/shell/public/interfaces/shell_client.mojom.h"

namespace mojo {

class Connector;

// Encapsulates a connection to the Mojo Shell in two parts:
// - a bound InterfacePtr to mojo::shell::mojom::Shell, the primary mechanism
//   by which the instantiating application interacts with other services
//   brokered by the Mojo Shell.
// - a bound InterfaceRequest of mojo::shell::mojom::ShellClient, an interface
//   used by the Mojo Shell to inform this application of lifecycle events and
//   inbound connections brokered by it.
//
// This class should be used in two scenarios:
// - During early startup to bind the mojo::shell::mojom::ShellClientRequest
//   obtained from the Mojo Shell. This is typically in response to either
//   MojoMain() or main().
// - In an implementation of mojo::shell::mojom::ContentHandler to bind the
//   mojo::shell::mojom::ShellClientRequest passed via StartApplication. In this
//   scenario there can be many instances of this class per process.
//
// Instances of this class are constructed with an implementation of the Shell
// Client Lib's mojo::ShellClient interface. See documentation in shell_client.h
// for details.
//
class ShellConnection : public shell::mojom::ShellClient {
 public:
  // Creates a new ShellConnection to eventually be bound to a
  // ShellClientRequest (see BindToRequest()). This connection may be used
  // immediately to begin making outgoing connections via connector().
  //
  // Does not take ownership of |client|, which must remain valid for the
  // lifetime of ShellConnection.
  explicit ShellConnection(mojo::ShellClient* client);

  // Like above but binds to |request| upon construction.
  ShellConnection(mojo::ShellClient* client,
                  shell::mojom::ShellClientRequest request);

  ~ShellConnection() override;

  Connector* connector() { return connector_.get(); }

  // Binds this ShellConnection to a client request if one was not available at
  // construction time.
  void BindToRequest(shell::mojom::ShellClientRequest request);

  // TODO(rockot): Remove this once we get rid of app tests.
  void SetAppTestConnectorForTesting(shell::mojom::ConnectorPtr connector);

 private:
  // shell::mojom::ShellClient:
  void Initialize(shell::mojom::IdentityPtr identity,
                  uint32_t id,
                  const InitializeCallback& callback) override;
  void AcceptConnection(
      shell::mojom::IdentityPtr source,
      uint32_t source_id,
      shell::mojom::InterfaceProviderRequest remote_interfaces,
      shell::mojom::InterfaceProviderPtr local_interfaces,
      shell::mojom::CapabilityRequestPtr allowed_capabilities,
      const String& name) override;

  void OnConnectionError();

  // We track the lifetime of incoming connection registries as it more
  // convenient for the client.
  ScopedVector<Connection> incoming_connections_;

  // A pending Connector request which will eventually be passed to the shell.
  shell::mojom::ConnectorRequest pending_connector_request_;

  mojo::ShellClient* client_;
  Binding<shell::mojom::ShellClient> binding_;
  scoped_ptr<Connector> connector_;

  DISALLOW_COPY_AND_ASSIGN(ShellConnection);
};

}  // namespace mojo

#endif  // MOJO_SHELL_PUBLIC_CPP_SHELL_CONNECTION_H_
