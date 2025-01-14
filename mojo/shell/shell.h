// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_SHELL_SHELL_H_
#define MOJO_SHELL_SHELL_H_

#include <map>
#include <vector>

#include "base/macros.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "mojo/public/cpp/bindings/binding_set.h"
#include "mojo/public/cpp/bindings/interface_ptr_set.h"
#include "mojo/shell/connect_params.h"
#include "mojo/shell/loader.h"
#include "mojo/shell/native_runner.h"
#include "mojo/shell/public/cpp/capabilities.h"
#include "mojo/shell/public/cpp/identity.h"
#include "mojo/shell/public/cpp/interface_factory.h"
#include "mojo/shell/public/cpp/shell_client.h"
#include "mojo/shell/public/interfaces/connector.mojom.h"
#include "mojo/shell/public/interfaces/interface_provider.mojom.h"
#include "mojo/shell/public/interfaces/shell.mojom.h"
#include "mojo/shell/public/interfaces/shell_client.mojom.h"
#include "mojo/shell/public/interfaces/shell_client_factory.mojom.h"
#include "mojo/shell/public/interfaces/shell_resolver.mojom.h"

namespace mojo {
class ShellConnection;
namespace shell {

// Creates an identity for the Shell, used when the Shell connects to
// applications.
Identity CreateShellIdentity();

class Shell : public ShellClient {
 public:
  // API for testing.
  class TestAPI {
   public:
    explicit TestAPI(Shell* shell);
    ~TestAPI();

    // Returns true if there is a Instance for this name.
    bool HasRunningInstanceForName(const std::string& name) const;
   private:
    Shell* shell_;

    DISALLOW_COPY_AND_ASSIGN(TestAPI);
  };

  // |native_runner_factory| is an instance of an object capable of vending
  // implementations of NativeRunner, e.g. for in or out-of-process execution.
  // See native_runner.h and RunNativeApplication().
  // |file_task_runner| provides access to a thread to perform file copy
  // operations on. This may be null only in testing environments where
  // applications are loaded via Loader implementations.
  Shell(scoped_ptr<NativeRunnerFactory> native_runner_factory,
        mojom::ShellClientPtr catalog);
  ~Shell() override;

  // Provide a callback to be notified whenever an instance is destroyed.
  // Typically the creator of the Shell will use this to determine when some set
  // of instances it created are destroyed, so it can shut down.
  void SetInstanceQuitCallback(base::Callback<void(const Identity&)> callback);

  // Completes a connection between a source and target application as defined
  // by |params|, exchanging InterfaceProviders between them. If no existing
  // instance of the target application is running, one will be loaded.
  void Connect(scoped_ptr<ConnectParams> params);

  // Creates a new Instance identified as |name|. This is intended for use by
  // the Shell's embedder to register itself with the shell. This must only be
  // called once.
  mojom::ShellClientRequest InitInstanceForEmbedder(const std::string& name);

  // Sets the default Loader to be used if not overridden by SetLoaderForName().
  void set_default_loader(scoped_ptr<Loader> loader) {
    default_loader_ = std::move(loader);
  }

  // Sets a Loader to be used for a specific name.
  void SetLoaderForName(scoped_ptr<Loader> loader, const std::string& name);

 private:
  class Instance;

  using IdentityToInstanceMap = std::map<Identity, Instance*>;
  using NameToLoaderMap = std::map<std::string, Loader*>;
  using IdentityToShellClientFactoryMap =
      std::map<Identity, mojom::ShellClientFactoryPtr>;

  // ShellClient:
  bool AcceptConnection(Connection* connection) override;

  void InitCatalog(mojom::ShellClientPtr catalog);

  // Destroys all Shell-ends of connections established with Applications.
  // Applications connected by this Shell will observe pipe errors and have a
  // chance to shutdown.
  void TerminateShellConnections();

  // Removes a Instance when it encounters an error.
  void OnInstanceError(Instance* instance);

  // Completes a connection between a source and target application as defined
  // by |params|, exchanging InterfaceProviders between them. If no existing
  // instance of the target application is running, one will be loaded.
  //
  // If |client| is not null, there must not be an instance of the target
  // application already running. The shell will create a new instance and use
  // |client| to control.
  void Connect(scoped_ptr<ConnectParams> params, mojom::ShellClientPtr client);

  // Returns a running instance matching |identity|.
  Instance* GetExistingInstance(const Identity& identity) const;
  // Like GetExistingInstance, but if no instance for |identity.user_id()| is
  // found, looks for kRootUserID too.
  Instance* GetExistingOrRootInstance(const Identity& identity) const;

  void NotifyPIDAvailable(uint32_t id, base::ProcessId pid);

  // Attempt to complete the connection requested by |params| by connecting to
  // an existing instance. If there is an existing instance, |params| is taken,
  // and this function returns true.
  bool ConnectToExistingInstance(scoped_ptr<ConnectParams>* params);

  Instance* CreateInstance(const Identity& target,
                           const CapabilitySpec& spec,
                           mojom::ShellClientPtr client);

  // Called from the instance implementing mojom::Shell.
  void AddInstanceListener(mojom::InstanceListenerPtr listener);

  void CreateShellClient(const Identity& source,
                         const Identity& shell_client_factory,
                         const std::string& name,
                         mojom::ShellClientRequest request);
  // Returns a running ShellClientFactory for |shell_client_factory_identity|,
  // if there is not one running one is started for |source_identity|.
  mojom::ShellClientFactory* GetShellClientFactory(
      const Identity& shell_client_factory_identity,
      const Identity& source_identity);
  void OnShellClientFactoryLost(const Identity& which);;

  // Callback when remote Catalog resolves mojo:foo to mojo:bar.
  // |params| are the params passed to Connect().
  // |client| if provided is a ShellClientPtr which should be used to manage the
  // new application instance. This may be null.
  // |resolved_name| is the mojo: name identifying the physical package
  // application.
  // |file_url| is the resolved file:// URL of the physical package.
  // |capabilities| is the CapabilitySpecPtr the requested application should be
  // run with, from its manifest.
  void OnGotResolvedName(scoped_ptr<ConnectParams> params,
                         mojom::ShellClientPtr client,
                         const String& resolved_name,
                         const String& resolved_instance,
                         mojom::CapabilitySpecPtr capabilities,
                         const String& file_url);

  // Tries to load |target| with an Loader. Returns true if one was registered
  // and it was loaded, in which case |request| is taken.
  bool LoadWithLoader(const Identity& target,
                      mojom::ShellClientRequest* request);

  // Returns the appropriate loader for |name|, or the default loader if there
  // is no loader configured for the name.
  Loader* GetLoaderForName(const std::string& name);

  void CleanupRunner(NativeRunner* runner);

  mojom::ShellResolverPtr shell_resolver_;

  // Loader management.
  // Loaders are chosen in the order they are listed here.
  NameToLoaderMap name_to_loader_;
  scoped_ptr<Loader> default_loader_;

  IdentityToInstanceMap identity_to_instance_;

  IdentityToShellClientFactoryMap shell_client_factories_;
  // Counter used to assign ids to content handlers.
  uint32_t shell_client_factory_id_counter_;

  InterfacePtrSet<mojom::InstanceListener> instance_listeners_;

  base::Callback<void(const Identity&)> instance_quit_callback_;
  scoped_ptr<NativeRunnerFactory> native_runner_factory_;
  std::vector<scoped_ptr<NativeRunner>> native_runners_;
  scoped_ptr<ShellConnection> shell_connection_;
  base::WeakPtrFactory<Shell> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(Shell);
};

mojom::Connector::ConnectCallback EmptyConnectCallback();

}  // namespace shell
}  // namespace mojo

#endif  // MOJO_SHELL_SHELL_H_
