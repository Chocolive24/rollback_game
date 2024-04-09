#pragma once

#include "application.h"
#include "Vec2.h"

/**
 * \brief Engine is the class which creates the main window and run an application
 *  that draws in it.
 */
class Engine {
public:
  constexpr explicit Engine(Application* app) noexcept : application_(app){}
  void ProcessFrame();
  void Run() noexcept;

  static Math::Vec2I window_size() noexcept { return window_size_; }

private:
  void Setup() noexcept;
  void TearDown() noexcept;

  Application* application_ = nullptr;
  inline static Math::Vec2I window_size_{1280, 720};
};
