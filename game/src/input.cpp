#include "input.h"
#include "Metrics.h"
#include "engine.h"

#include <Common-cpp/inc/Common.h>
#include <raylib_wrapper.h>


namespace input {

extern std::array<Math::Vec2F, game_constants::kMaxPlayerCount> mouse_pos{};

PlayerInput GetPlayerInput(int input_profile_id) noexcept {
  switch (input_profile_id) {
    case 0: {
      PlayerInput player_1_inputs = 0;

      if (raylib::IsKeyDown(raylib::KEY_W)) {
        player_1_inputs |= static_cast<std::uint8_t>(PlayerInputType::kUp);
      }
      if (IsKeyDown(raylib::KEY_A)) {
        player_1_inputs |= static_cast<std::uint8_t>(PlayerInputType::kLeft);
      }
      if (IsKeyDown(raylib::KEY_S)) {
        player_1_inputs |= static_cast<std::uint8_t>(PlayerInputType::kDown);
      }
      if (IsKeyDown(raylib::KEY_D)) {
        player_1_inputs |= static_cast<std::uint8_t>(PlayerInputType::kRight);
      }
      if (IsMouseButtonDown(raylib::MOUSE_BUTTON_LEFT) && Engine::are_mouse_inputs_enabled()) {
        // Check if the mouse is not being used for window move.
        if (!raylib::IsWindowResized() && !raylib::IsWindowMinimized()) {
          player_1_inputs |= static_cast<std::uint8_t>(PlayerInputType::kShoot);
        }
      }

      return player_1_inputs;
    }

    case 1: {
      PlayerInput player_2_inputs = 0;

      if (IsKeyDown(raylib::KEY_UP)) {
        player_2_inputs |= static_cast<std::uint8_t>(PlayerInputType::kUp);
      }
      if (IsKeyDown(raylib::KEY_LEFT)) {
        player_2_inputs |= static_cast<std::uint8_t>(PlayerInputType::kLeft);
      }
      if (IsKeyDown(raylib::KEY_DOWN)) {
        player_2_inputs |= static_cast<std::uint8_t>(PlayerInputType::kDown);
      }
      if (IsKeyDown(raylib::KEY_RIGHT)) {
        player_2_inputs |= static_cast<std::uint8_t>(PlayerInputType::kRight);
      }
      if (IsMouseButtonDown(raylib::MOUSE_BUTTON_RIGHT) &&
          Engine::are_mouse_inputs_enabled()) {
        player_2_inputs |= static_cast<std::uint8_t>(PlayerInputType::kShoot);
      }

      return player_2_inputs;
    }
    default:
      return PlayerInput();
  }
}

Math::Vec2F CalculateDirToMouse(const Math::Vec2F pos, PlayerId player_id) noexcept {
  return  (mouse_pos[player_id] - pos).Normalized();
}

nByte FrameInput::serialization_protocol_ =
    ExitGames::Common::SerializationProtocol::DEFAULT;

FrameInput::FrameInput(Math::Vec2F dir_to_mouse, FrameNbr frame_nbr,
                       PlayerInput input) noexcept
    : dir_to_mouse_(dir_to_mouse), frame_nbr_(frame_nbr), input_(input) {}

ExitGames::Common::JString& FrameInput::toString(
    ExitGames::Common::JString& retStr, bool withTypes) const {
  return retStr =
             ExitGames::Common::JString(L"<") +
             (withTypes ? ExitGames::Common::JString(L"(") + EG_STR_UCHAR + L")"
                        : L"") +
             dir_to_mouse_.X + L", " + dir_to_mouse_.Y + L", " +
             (withTypes ? ExitGames::Common::JString(L"(") + EG_STR_UCHAR + L")"
                        : L"") +
             frame_nbr_ + L">" +
             (withTypes ? ExitGames::Common::JString(L"(") + EG_STR_UCHAR + L")"
                        : L"") +
             input_ + L">";
}

bool FrameInput::compare(const CustomTypeBase& other) const {
  return dir_to_mouse_ ==
             dynamic_cast<const FrameInput&>(other).dir_to_mouse_ &&
         frame_nbr_ == dynamic_cast<const FrameInput&>(other).frame_nbr_ &&
         input_ == dynamic_cast<const FrameInput&>(other).input_;
}

void FrameInput::deserialize(const nByte* pData, short length) {
  ExitGames::Common::Deserializer d(pData, length, serialization_protocol_);
  ExitGames::Common::Object o{};

  d.pop(o);
  dir_to_mouse_.X = ExitGames::Common::ValueObject<float>(o).getDataCopy();

  d.pop(o);
  dir_to_mouse_.Y = ExitGames::Common::ValueObject<float>(o).getDataCopy();

  d.pop(o);
  frame_nbr_ = ExitGames::Common::ValueObject<FrameNbr>(o).getDataCopy();

  d.pop(o);
  input_ = ExitGames::Common::ValueObject<input::PlayerInput>(o).getDataCopy();
}

short FrameInput::serialize(nByte* pRetVal) const {
  ExitGames::Common::Serializer s(serialization_protocol_);

  s.push(dir_to_mouse_.X);
  s.push(dir_to_mouse_.Y);
  s.push(frame_nbr_);
  s.push(input_);

  if (pRetVal) {
    MEMCPY(pRetVal, s.getData(), s.getSize());
  }

  return static_cast<short>(s.getSize());
}

void FrameInput::duplicate(CustomTypeBase* pRetVal) const {
  *reinterpret_cast<FrameInput*>(pRetVal) = *this;
}

bool FrameInput::operator==(const FrameInput& other) const noexcept {
  return dir_to_mouse_ == other.dir_to_mouse_ &&
         frame_nbr_ == other.frame_nbr_ && input_ == other.input_;
}



}  // namespace input