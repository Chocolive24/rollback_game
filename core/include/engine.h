#pragma once

#include "app_interface.h"
#include "Vec2.h"

/**
 * \brief Engine is the class which creates the main window and run an application
 *  that draws in it.
 */
class Engine {
public:
  constexpr explicit Engine(Application* app) noexcept : application_(app){}
  void Run() noexcept;

private:
  void Setup() noexcept;
  void TearDown() noexcept;

  Application* application_ = nullptr;
  Math::Vec2I window_size_{1280, 720};
};
