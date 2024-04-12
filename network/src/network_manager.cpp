#include "network_manager.h"

NetworkManager::NetworkManager(const ExitGames::Common::JString& appID,
    const ExitGames::Common::JString& appVersion) :
load_balancing_client_(listener_, appID, appVersion) {
  
}

void NetworkManager::connect() {
  // connect() is asynchronous - the actual result arrives in the
  // Listener::connectReturn() or the Listener::connectionErrorReturn() callback
  std::cout << "hello\n";
  if (!load_balancing_client_.connect())
    EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"Could not connect.");
}

void NetworkManager::run() {
  load_balancing_client_.service();
}

void NetworkManager::disconnect() {
  load_balancing_client_
      .disconnect();  // disconnect() is asynchronous - the actual result
                      // arrives in the Listener::disconnectReturn() callback
}

void NetworkManager::createRoom(const ExitGames::Common::JString& roomName, 
    nByte maxPlayers) {
  if (load_balancing_client_.opCreateRoom(
          roomName,
          ExitGames::LoadBalancing::RoomOptions().setMaxPlayers(maxPlayers))) {
    std::cout << "Room successfully created with name: "
              << roomName.UTF8Representation().cstr() << '\n';
  }
}

void NetworkManager::JoinRandomRoom(
    ExitGames::Common::Hashtable expectedCustomRoomProperties) {
  load_balancing_client_.opJoinRandomRoom(expectedCustomRoomProperties);
}

void NetworkManager::RaiseEvent(bool reliable,
                                    const ExitGames::Common::Hashtable& data,
                                    nByte event_code) noexcept {
  load_balancing_client_.opRaiseEvent(reliable, data, event_code);
}

void NetworkManager::ReceiveEvent(int player_nr, nByte event_code,
    const ExitGames::Common::Object& event_content) noexcept {
  // logging the string representation of the eventContent can be really useful
  // for debugging, but use with care: for big events this might get expensive
  switch (event_code) {
    case 1: {
      // you can access the content as a copy (might be a bit expensive for
      // really big data constructs)
      ExitGames::Common::Hashtable content =
          ExitGames::Common::ValueObject<ExitGames::Common::Hashtable>(
              event_content).getDataCopy();

      // Print the content of the hashtable as a whole
      std::cout << "content: "
                << content.toString(true).UTF8Representation().cstr() << '\n';

      // Access the content by key
      ExitGames::Common::Object* value = content.getValue(L"message");
      if (value != nullptr) {
        std::cout << "content by key: "
                  << value->toString(true).UTF8Representation().cstr() << '\n';
      } else {
        std::cout << "Key 'message' not found in the hashtable.\n";
      }

      // or you access it by address (it will become invalid as soon as this
      // function returns, so (any part of the) data that you need to continue
      // having access to later on needs to be copied)
      ExitGames::Common::Hashtable* pContent =
          ExitGames::Common::ValueObject<ExitGames::Common::Hashtable>(
              event_content).getDataAddress();
    } break;
    case 2: {
      // of course, the payload does not need to be a Hashtable - how about just
      // sending around for example a plain 64bit integer?
      long long content =
          ExitGames::Common::ValueObject<long long>(event_content).getDataCopy();
    } break;
    case 3: {
      // or an array of floats?
      float* pContent =
          ExitGames::Common::ValueObject<float*>(event_content).getDataCopy();
      float** ppContent =
          ExitGames::Common::ValueObject<float*>(event_content).getDataAddress();
      short contentElementCount =
          *ExitGames::Common::ValueObject<float*>(event_content).getSizes();
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
