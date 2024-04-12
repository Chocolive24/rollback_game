#include "network_logic.h"

void Listener::debugReturn(int debugLevel,
                           const ExitGames::Common::JString& string) {
  std::cout << "debug return: debug level: " << debugLevel
            << " string: " << string.UTF8Representation().cstr() << '\n';
}

void Listener::connectionErrorReturn(int errorCode) {
  std::cout << "error connection\n";
}

void Listener::clientErrorReturn(int errorCode) {
  std::cout << "client error code : " << errorCode << '\n';
}

void Listener::warningReturn(int warningCode) {
  std::cout << "client warning code : " << warningCode << '\n';
}

void Listener::serverErrorReturn(int errorCode) {
  std::cout << "server error code : " << errorCode << '\n';
}

void Listener::joinRoomEventAction(
    int playerNr, const ExitGames::Common::JVector<int>& playernrs,
    const ExitGames::LoadBalancing::Player& player) {
  std::cout << "Room state: player nr: " << playerNr
            << " player nrs size: " << playernrs.getSize() << " player userID: "
            << player.getUserID().UTF8Representation().cstr() << '\n';
}

void Listener::leaveRoomEventAction(int playerNr, bool isInactive) {
  std::cout << "player nr: " << playerNr << " is inactive: " << isInactive
            << '\n';
}

void Listener::customEventAction(int playerNr, nByte eventCode,
                       const ExitGames::Common::Object& eventContent) {
  // logging the string representation of the eventContent can be really useful
  // for debugging, but use with care: for big events this might get expensive

  switch (eventCode) {
    case 1: {
      // you can access the content as a copy (might be a bit expensive for
      // really big data constructs)
      ExitGames::Common::Hashtable content =
          ExitGames::Common::ValueObject<ExitGames::Common::Hashtable>(
              eventContent).getDataCopy();

      // Print the content of the hashtable as a whole
      std::cout << "content: " << content.toString(true).UTF8Representation().cstr()
                << '\n';

      // Access the content by key
      ExitGames::Common::Object* value = content.getValue(L"message");
      if (value != nullptr) {
        std::cout << "content by key: " << value->toString(true).UTF8Representation().cstr() << '\n';
      } else {
        std::cout << "Key 'message' not found in the hashtable.\n";
      }

      // or you access it by address (it will become invalid as soon as this
      // function returns, so (any part of the) data that you need to continue
      // having access to later on needs to be copied)
      ExitGames::Common::Hashtable* pContent =
          ExitGames::Common::ValueObject<ExitGames::Common::Hashtable>(
              eventContent).getDataAddress();
    } break;
    case 2: {
      // of course, the payload does not need to be a Hashtable - how about just
      // sending around for example a plain 64bit integer?
      long long content =
          ExitGames::Common::ValueObject<long long>(eventContent).getDataCopy();
    } break;
    case 3: {
      // or an array of floats?
      float* pContent =
          ExitGames::Common::ValueObject<float*>(eventContent).getDataCopy();
      float** ppContent =
          ExitGames::Common::ValueObject<float*>(eventContent).getDataAddress();
      short contentElementCount =
          *ExitGames::Common::ValueObject<float*>(eventContent).getSizes();
      // when calling getDataCopy() on Objects that hold an array as payload,
      // then you must deallocate the copy of the array yourself using
      // deallocateArray()!
      ExitGames::Common::MemoryManagement::deallocateArray(pContent);
    } break;
    default: {
      // have a look at demo_typeSupport inside the C++ client SDKs for example
      // code on how to send and receive more fancy data types
    } break;
  }
}   

void Listener::connectReturn(
    int errorCode, const ExitGames::Common::JString& errorString,
                   const ExitGames::Common::JString& region,
                   const ExitGames::Common::JString& cluster) {
  std::cout << "connected, error code:" << errorCode << " "
            << "error string: " << errorString.UTF8Representation().cstr()
            << " "
            << "region: " << region.UTF8Representation().cstr() << " "
            << "cluster: " << cluster.UTF8Representation().cstr() << '\n';
}

void Listener::disconnectReturn() {
  std::cout << "client disconnected\n";
}

void Listener::leaveRoomReturn(int errorCode,
                     const ExitGames::Common::JString& errorString) {
  std::cout << "Leave room return: error code: " << errorCode
            << " error string: " << errorString.UTF8Representation().cstr()
            << '\n';
}

SampleNetworkLogic::SampleNetworkLogic(const ExitGames::Common::JString& appID,
    const ExitGames::Common::JString& appVersion) :
load_balancing_client_(listener_, appID, appVersion) {
  
}

void SampleNetworkLogic::connect() {
  // connect() is asynchronous - the actual result arrives in the
  // Listener::connectReturn() or the Listener::connectionErrorReturn() callback
  std::cout << "hello\n";
  if (!load_balancing_client_.connect())
    EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"Could not connect.");
}

void SampleNetworkLogic::run() {
  load_balancing_client_.service();
}

void SampleNetworkLogic::disconnect() {
  load_balancing_client_
      .disconnect();  // disconnect() is asynchronous - the actual result
                      // arrives in the Listener::disconnectReturn() callback
}

void SampleNetworkLogic::createRoom(const ExitGames::Common::JString& roomName, 
    nByte maxPlayers) {
  if (load_balancing_client_.opCreateRoom(
          roomName,
          ExitGames::LoadBalancing::RoomOptions().setMaxPlayers(maxPlayers))) {
    std::cout << "Room successfully created with name: "
              << roomName.UTF8Representation().cstr() << '\n';
  }
}

void SampleNetworkLogic::JoinRandomRoom(
    ExitGames::Common::Hashtable expectedCustomRoomProperties) {
  load_balancing_client_.opJoinRandomRoom(expectedCustomRoomProperties);
}

void SampleNetworkLogic::RaiseEvent(bool send_reliable,
                                    const ExitGames::Common::Hashtable& data, nByte event_code) {
  load_balancing_client_.opRaiseEvent(send_reliable, data, event_code);
}

void SampleNetworkLogic::customEventAction(int playerNr, nByte eventCode,
    const ExitGames::Common::Object& eventContent) {
  // logging the string representation of the eventContent can be really useful
  // for debugging, but use with care: for big events this might get expensive
  EGLOG(ExitGames::Common::DebugLevel::ALL,
        L"an event of type %d from player Nr %d with the following content has "
        L"just arrived: %ls",
        eventCode, playerNr, eventContent.toString(true).cstr());

  switch (eventCode) {
    case 1: {
      // you can access the content as a copy (might be a bit expensive for
      // really big data constructs)
      ExitGames::Common::Hashtable content =
          ExitGames::Common::ValueObject<ExitGames::Common::Hashtable>(
              eventContent)
              .getDataCopy();
      // or you access it by address (it will become invalid as soon as this
      // function returns, so (any part of the) data that you need to continue
      // having access to later on needs to be copied)
      ExitGames::Common::Hashtable* pContent =
          ExitGames::Common::ValueObject<ExitGames::Common::Hashtable>(
              eventContent).getDataAddress();
    } break;
    case 2: {
      // of course, the payload does not need to be a Hashtable - how about just
      // sending around for example a plain 64bit integer?
      long long content =
          ExitGames::Common::ValueObject<long long>(eventContent).getDataCopy();
    } break;
    case 3: {
      // or an array of floats?
      float* pContent =
          ExitGames::Common::ValueObject<float*>(eventContent).getDataCopy();
      float** ppContent =
          ExitGames::Common::ValueObject<float*>(eventContent).getDataAddress();
      short contentElementCount =
          *ExitGames::Common::ValueObject<float*>(eventContent).getSizes();
      // when calling getDataCopy() on Objects that hold an array as payload,
      // then you must deallocate the copy of the array yourself using
      // deallocateArray()!
      ExitGames::Common::MemoryManagement::deallocateArray(pContent);
    } break;
    default: {
      // have a look at demo_typeSupport inside the C++ client SDKs for example
      // code on how to send and receive more fancy data types
    } break;
  }
}