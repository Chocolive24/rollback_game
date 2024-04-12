#pragma once

/**
 * \brief Application is an interface for creating and managing applications
 * within the project.
 *
 * This interface defines the basic lifecycle functions that any application
 * within the engine must implement. Each function corresponds to a specific
 * stage in the application lifecycle.
 */
class Application {
 public:
  constexpr explicit Application() noexcept = default;
  explicit Application(Application&& other) noexcept = default;
  Application& operator=(Application&& other) noexcept = default;
  explicit Application(const Application& other) noexcept = default;
  Application& operator=(const Application& other) noexcept = default;
  virtual ~Application() noexcept = default;

  /**
   * \brief Initialize the application.
   *
   * This function is called once at the start of the application's lifecycle.
   */
  virtual void Setup() noexcept = 0;

  /**
   * \brief Update the application state.
   *
   * This function is called every frame to update the application logic.
   */
  virtual void Update() noexcept = 0;

  /**
   * \brief Render the application content.
   *
   * This function is called every frame to render the application's graphics.
   */
  virtual void Draw() noexcept = 0;

  /**
   * \brief Render the ImGui user interface.
   *
   * This function is called every frame to render the ImGui-based user
   * interface.
   */
  virtual void DrawImGui() noexcept = 0;

  /**
   * \brief Clean up the resources used by the application.
   *
   * This function is called once at the end of the application's lifecycle.
   */
  virtual void TearDown() noexcept = 0;
};