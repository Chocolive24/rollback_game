#pragma once

#include <Common-cpp/inc/defines.h>
#include <Common-cpp/inc/Containers/Hashtable.h>

#include "inputs.h"
#include "Vec2.h"

/**
 * \brief EventCode is an enum which differentiates between the various
 * events in the application.
 */
enum class NetworkEventCode : nByte {
  kInput = 0,
  kFrameConfirmation
};

/**
 * \brief EventKey is an enum which differentiates between the various
 * keys used to serialize/deserialize the hashtable events.
 */
enum class NetworkEventKey : nByte {
  kPlayerInput = 0,
  kFrameNbr,
  kDelay,
  kCheckSum
};

struct NetworkEvent {
  NetworkEventCode code{};
  ExitGames::Common::Hashtable content{};
};

class FrameInput final : public ExitGames::Common::CustomType<FrameInput, 1> {
  typedef ExitGames::Common::CustomType<FrameInput, 1> super;

 public:
  FrameInput() noexcept = default;
  FrameInput(Math::Vec2F dir_to_mouse, FrameNbr frame_nbr, 
	  input::PlayerInput input) noexcept;
  FrameInput(FrameInput&& toMove);
  FrameInput& operator=(FrameInput&& toMove);
  FrameInput(const FrameInput& toCopy);
  FrameInput& operator=(const FrameInput& toCopy);
  ~FrameInput() override = default;

  ExitGames::Common::JString& toString(ExitGames::Common::JString& retStr,
                                       bool withTypes) const override;
  bool compare(const CustomTypeBase& other) const override;
  void deserialize(const nByte* pData, short length) override;
  short serialize(nByte* pRetVal) const override;
  void duplicate(CustomTypeBase* pRetVal) const override;

  bool operator==(const FrameInput& other) const noexcept;

  [[nodiscard]] Math::Vec2F dir_to_mouse() const noexcept { return dir_to_mouse_; }
  [[nodiscard]] FrameNbr frame_nbr() const noexcept { return frame_nbr_; }
  [[nodiscard]] input::PlayerInput input() const noexcept { return input_; }

private:
  Math::Vec2F dir_to_mouse_{};
  FrameNbr frame_nbr_;
  input::PlayerInput input_;
  
  static nByte serialization_protocol_;
};