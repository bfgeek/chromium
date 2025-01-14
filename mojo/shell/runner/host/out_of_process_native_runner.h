// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_SHELL_RUNNER_HOST_OUT_OF_PROCESS_NATIVE_RUNNER_H_
#define MOJO_SHELL_RUNNER_HOST_OUT_OF_PROCESS_NATIVE_RUNNER_H_

#include <stdint.h>

#include "base/callback.h"
#include "base/files/file_path.h"
#include "base/macros.h"
#include "base/memory/scoped_ptr.h"
#include "mojo/shell/native_runner.h"
#include "mojo/shell/public/interfaces/shell_client_factory.mojom.h"

namespace base {
class TaskRunner;
}

namespace mojo {
namespace shell {

class ChildProcessHost;
class NativeRunnerDelegate;

// An implementation of |NativeRunner| that loads/runs the given app (from the
// file system) in a separate process (of its own).
class OutOfProcessNativeRunner : public NativeRunner {
 public:
  OutOfProcessNativeRunner(base::TaskRunner* launch_process_runner,
                           NativeRunnerDelegate* delegate);
  ~OutOfProcessNativeRunner() override;

  // NativeRunner:
  void Start(
      const base::FilePath& app_path,
      const Identity& identity,
      bool start_sandboxed,
      mojom::ShellClientRequest request,
      const base::Callback<void(base::ProcessId)>& pid_available_callback,
      const base::Closure& app_completed_callback) override;

 private:
  void AppCompleted();

  base::TaskRunner* const launch_process_runner_;
  NativeRunnerDelegate* delegate_;

  base::FilePath app_path_;
  base::Closure app_completed_callback_;

  scoped_ptr<ChildProcessHost> child_process_host_;

  DISALLOW_COPY_AND_ASSIGN(OutOfProcessNativeRunner);
};

class OutOfProcessNativeRunnerFactory : public NativeRunnerFactory {
 public:
  OutOfProcessNativeRunnerFactory(base::TaskRunner* launch_process_runner,
                                  NativeRunnerDelegate* delegate);
  ~OutOfProcessNativeRunnerFactory() override;

  scoped_ptr<NativeRunner> Create(const base::FilePath& app_path) override;

 private:
  base::TaskRunner* const launch_process_runner_;
  NativeRunnerDelegate* delegate_;

  DISALLOW_COPY_AND_ASSIGN(OutOfProcessNativeRunnerFactory);
};

}  // namespace shell
}  // namespace mojo

#endif  // MOJO_SHELL_RUNNER_HOST_OUT_OF_PROCESS_NATIVE_RUNNER_H_
