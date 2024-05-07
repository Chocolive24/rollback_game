#pragma once

#include "Vec2.h"
#include "types.h"
#include "game_constants.h"

#include <Common-cpp/inc/CustomType.h>

#include <vector>

namespace input {

/**
* \brief PlayerInputs is an uint8 which stores all the inputs value of a player
* in a frame.
*/
using PlayerInput = std::uint8_t;

enum class PlayerInputType : std::uint8_t {
  kUp = 1 << 0,
  kDown = 1 << 1,
  kRight = 1 << 2,
  kLeft = 1 << 3,
  kShoot = 1 << 4,
};

constexpr PlayerInput kInvalidInput = 255;

extern std::array<Math::Vec2F, game_constants::kMaxPlayerCount> mouse_pos;

[[nodiscard]] PlayerInput GetPlayerInput(int input_profile_id) noexcept;
[[nodiscard]] Math::Vec2F CalculateDirToMouse(Math::Vec2F pos, PlayerId player_id) noexcept;

class FrameInput final : public ExitGames::Common::CustomType<FrameInput, 1> {
  typedef CustomType<FrameInput, 1> super;

 public:
  FrameInput() noexcept = default;
  FrameInput(Math::Vec2F dir_to_mouse, FrameNbr frame_nbr,
             input::PlayerInput input) noexcept;
  FrameInput(FrameInput&& toMove) noexcept = default;
  FrameInput& operator=(FrameInput&& toMove) noexcept = default;
  FrameInput(const FrameInput& toCopy) noexcept = default;
  FrameInput& operator=(const FrameInput& toCopy) noexcept = default;
  ~FrameInput() override = default;

  ExitGames::Common::JString& toString(ExitGames::Common::JString& retStr,
                                       bool withTypes) const override;
  bool compare(const CustomTypeBase& other) const override;
  void deserialize(const nByte* pData, short length) override;
  short serialize(nByte* pRetVal) const override;
  void duplicate(CustomTypeBase* pRetVal) const override;

  bool operator==(const FrameInput& other) const noexcept;

  [[nodiscard]] Math::Vec2F dir_to_mouse() const noexcept {
    return dir_to_mouse_;
  }

  [[nodiscard]] FrameNbr frame_nbr() const noexcept { return frame_nbr_; }
  [[nodiscard]] input::PlayerInput input() const noexcept { return input_; }

 private:
  Math::Vec2F dir_to_mouse_ = Math::Vec2F::Zero();
  FrameNbr frame_nbr_ = 0;
  input::PlayerInput input_ = 0;

  static nByte serialization_protocol_;
};

struct FrameToConfirm {
  Checksum checksum;
  std::vector<FrameInput> frame_inputs{};
};

}  // namespace input