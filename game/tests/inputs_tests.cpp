#include "debug_client.h"
#include "debug_server.h"

#include "gtest/gtest.h"

TEST(Inputs, CheckInputReceived) {
  DebugServer server;
  DebugClient client1;
  DebugClient client2;

  server.Setup();
  server.RegisterClients(&client1, &client2);

  client1.Init(1, Math::Vec2I::Zero());
  client1.RegisterServer(&server);

  client2.Init(1, Math::Vec2I::Zero());
  client2.RegisterServer(&server);

  inputs::PlayerInputs inputs = 0;
  inputs |= static_cast<std::uint8_t>(inputs::PlayerInputTypes::kUp);

  client1.SendInputs(inputs::FrameInput{inputs, 0});

  EXPECT_TRUE(true);

  while (true)
  {
    client1.Update();
    client2.Update();

    server.Update();
  }
}