//#include "serialization.h"
//
//namespace serialization {
//// Serialize FrameInput into a Hashtable
//ExitGames::Common::Hashtable serializeFrameInput(const FrameInput& input) {
//  ExitGames::Common::Hashtable data;
//  data.put("x", input.x);
//  data.put("y", input.y);
//  return data;
//}
//
//// Deserialize Hashtable into FrameInput
//FrameInput deserializeFrameInput(const ExitGames::Common::Hashtable& data) {
//  FrameInput input;
//  input.x = data.getValue<int>("x");
//  input.y = data.getValue<int>("y");
//  return input;
//}
//}