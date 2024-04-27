#include "event.h"

#include <Common-cpp/inc/Common.h>

nByte FrameInput::serialization_protocol_ =
  ExitGames::Common::SerializationProtocol::DEFAULT;

FrameInput::FrameInput(Math::Vec2F dir_to_mouse, FrameNbr frame_nbr, 
    input::PlayerInput input) noexcept :
  dir_to_mouse_(dir_to_mouse),
  frame_nbr_(frame_nbr),
  input_(input)
{
}

//FrameInput::FrameInput(FrameInput&& toMove) : super() {
//  dir_to_mouse_ = toMove.dir_to_mouse_;
//  frame_nbr_ = toMove.frame_nbr_;
//  input_ = toMove.input_;
//}
//
//
//FrameInput& FrameInput::operator=(FrameInput&& toMove) {
//  dir_to_mouse_ = toMove.dir_to_mouse_;
//  frame_nbr_ = toMove.frame_nbr_;
//  input_ = toMove.input_;
//  return *this;
//}
//
//FrameInput::FrameInput(const FrameInput& toCopy) : super() {
//  dir_to_mouse_ = toCopy.dir_to_mouse_;
//  frame_nbr_ = toCopy.frame_nbr_;
//  input_ = toCopy.input_;
//}
//
//FrameInput& FrameInput::operator=(const FrameInput& toCopy) {
//  dir_to_mouse_ = toCopy.dir_to_mouse_;
//  frame_nbr_ = toCopy.frame_nbr_;
//  input_ = toCopy.input_;
//  return *this;
//}

ExitGames::Common::JString& FrameInput::toString(
    ExitGames::Common::JString& retStr, bool withTypes) const {
  return retStr =
             ExitGames::Common::JString(L"<") +
             (withTypes ? ExitGames::Common::JString(L"(") + EG_STR_UCHAR + L")"
                        : L"") +
             dir_to_mouse_.X + L", " +
             dir_to_mouse_.Y + L", " +
             (withTypes ? ExitGames::Common::JString(L"(") + EG_STR_UCHAR + L")"
                        : L"") +
             frame_nbr_ + L">" +
              (withTypes ? ExitGames::Common::JString(L"(") + EG_STR_UCHAR + L")" : L"") +
             input_ + L">";
}

bool FrameInput::compare(const CustomTypeBase& other) const {
  return dir_to_mouse_ == dynamic_cast<const FrameInput&>(other).dir_to_mouse_ &&
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
         frame_nbr_ == other.frame_nbr_ &&
         input_ == other.input_;
}
