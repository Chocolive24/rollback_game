#pragma once

#include "Vec2.h"
#include "application.h"

/**
 * \brief The Engine class manages the main execution loop and coordinates
 * the application lifecycle given to it.
 *
 * This class serves as the central control unit of the application framework,
 * responsible for initializing, running, and terminating the application.
 */
class Engine {
 public:
  /**
   * \brief Constructs the Engine object with a pointer to an application.
   * \param app A pointer to the application object.
   */
  constexpr explicit Engine(Application* app) noexcept : application_(app) {}

  /**
   * \brief Processes a single frame of the application.
   *
   * This function is called repeatedly to update the application state and
   * render the graphics for each frame.
   */
  void ProcessFrame();

  /**
   * \brief Runs the application.
   *
   * This function initiates the main loop of the application, handling setup,
   * execution, and teardown.
   */
  void Run() noexcept;

  /**
   * \brief Returns the size of the application window.
   * \return A Math::Vec2I representing the size of the window.
   */
  static Math::Vec2I window_size() noexcept { return window_size_; }

 private:
  /**
   * \brief Performs setup tasks before running the application.
   *
   * This function initializes the engine and prepares it for execution.
   */
  void Setup() noexcept;

  /**
   * \brief Performs cleanup tasks after the application has finished running.
   *
   * This function cleans up any resources used by the engine and application.
   */
  void TearDown() noexcept;

  Application* application_ = nullptr;
  inline static Math::Vec2I window_size_{1280, 720};
};
