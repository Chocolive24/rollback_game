#include "LoadBalancing-cpp/inc/Client.h"

#include <stdlib.h>

class Listener : public ExitGames::LoadBalancing::Listener {
public:
  void debugReturn(int debugLevel, const ExitGames::Common::JString& string) override{}
  void connectionErrorReturn(int errorCode) override{}
  void clientErrorReturn(int errorCode) override{}
  void warningReturn(int warningCode) override{}
  void serverErrorReturn(int errorCode) override{}
  void joinRoomEventAction(int playerNr, const ExitGames::Common::JVector<int>& playernrs,
    const ExitGames::LoadBalancing::Player& player) override{}
  void leaveRoomEventAction(int playerNr, bool isInactive) override{}
  void customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContent) override{}
  void connectReturn(int errorCode, const ExitGames::Common::JString& errorString,
    const ExitGames::Common::JString& region, const ExitGames::Common::JString& cluster) override{}
  void disconnectReturn() override{}
  void leaveRoomReturn(int errorCode, const ExitGames::Common::JString& errorString) override{}
};
//
int main() {
//  Listener listener{};
//  ExitGames::LoadBalancing::Listener* listenr_ptr = &listener;
//  ExitGames::LoadBalancing::Client client(*listenr_ptr);
  return EXIT_SUCCESS;
}
