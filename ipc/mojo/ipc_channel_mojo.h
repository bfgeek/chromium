// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef IPC_IPC_CHANNEL_MOJO_H_
#define IPC_IPC_CHANNEL_MOJO_H_

#include <stdint.h>

#include <vector>

#include "base/macros.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "base/memory/weak_ptr.h"
#include "build/build_config.h"
#include "ipc/ipc_channel.h"
#include "ipc/ipc_channel_factory.h"
#include "ipc/ipc_export.h"
#include "ipc/mojo/ipc_message_pipe_reader.h"
#include "ipc/mojo/ipc_mojo_bootstrap.h"
#include "mojo/edk/embedder/scoped_platform_handle.h"
#include "mojo/public/cpp/system/core.h"

namespace IPC {

// Mojo-based IPC::Channel implementation over a Mojo message pipe.
//
// ChannelMojo builds a Mojo MessagePipe using the provided message pipe
// |handle| and builds an associated interface for each direction on the
// channel.
//
// TODO(morrita): Add APIs to create extra MessagePipes to let
//                Mojo-based objects talk over this Channel.
//
class IPC_MOJO_EXPORT ChannelMojo
    : public Channel,
      public MojoBootstrap::Delegate,
      public NON_EXPORTED_BASE(internal::MessagePipeReader::Delegate) {
 public:
  // True if ChannelMojo should be used regardless of the flag.
  static bool ShouldBeUsed();

  // Creates a ChannelMojo.
  static scoped_ptr<ChannelMojo> Create(mojo::ScopedMessagePipeHandle handle,
                                        Mode mode,
                                        Listener* listener);

  // Create a factory object for ChannelMojo.
  // The factory is used to create Mojo-based ChannelProxy family.
  // |host| must not be null.
  static scoped_ptr<ChannelFactory> CreateServerFactory(
      mojo::ScopedMessagePipeHandle handle);

  static scoped_ptr<ChannelFactory> CreateClientFactory(
      mojo::ScopedMessagePipeHandle handle);

  ~ChannelMojo() override;

  // Channel implementation
  bool Connect() override;
  void Close() override;
  bool Send(Message* message) override;
  base::ProcessId GetPeerPID() const override;
  base::ProcessId GetSelfPID() const override;

#if defined(OS_POSIX) && !defined(OS_NACL)
  int GetClientFileDescriptor() const override;
  base::ScopedFD TakeClientFileDescriptor() override;
#endif  // defined(OS_POSIX) && !defined(OS_NACL)

  // These access protected API of IPC::Message, which has ChannelMojo
  // as a friend class.
  static MojoResult WriteToMessageAttachmentSet(
      mojo::Array<mojom::SerializedHandlePtr> handle_buffer,
      Message* message);
  static MojoResult ReadFromMessageAttachmentSet(
      Message* message,
      mojo::Array<mojom::SerializedHandlePtr>* handles);

  // MojoBootstrapDelegate implementation
  void OnPipesAvailable(mojom::ChannelAssociatedPtrInfo send_channel,
                        mojom::ChannelAssociatedRequest receive_channel,
                        int32_t peer_pid) override;
  void OnBootstrapError() override;

  // MessagePipeReader::Delegate
  void OnMessageReceived(const Message& message) override;
  void OnPipeClosed(internal::MessagePipeReader* reader) override;
  void OnPipeError(internal::MessagePipeReader* reader) override;

 private:
  ChannelMojo(mojo::ScopedMessagePipeHandle handle,
              Mode mode,
              Listener* listener);

  void InitMessageReader(mojom::ChannelAssociatedPtrInfo sender,
                         mojom::ChannelAssociatedRequest receiver,
                         base::ProcessId peer_pid);

  // ChannelMojo needs to kill its MessagePipeReader in delayed manner
  // because the channel wants to kill these readers during the
  // notifications invoked by them.
  typedef internal::MessagePipeReader::DelayedDeleter ReaderDeleter;

  scoped_ptr<MojoBootstrap> bootstrap_;
  Listener* listener_;

  scoped_ptr<internal::MessagePipeReader, ReaderDeleter> message_reader_;
  std::vector<scoped_ptr<Message>> pending_messages_;
  bool waiting_connect_;

  base::WeakPtrFactory<ChannelMojo> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(ChannelMojo);
};

}  // namespace IPC

#endif  // IPC_IPC_CHANNEL_MOJO_H_
