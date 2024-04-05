#pragma once

class Application {
public:
  constexpr explicit Application() noexcept = default;
  explicit Application(Application&& other) noexcept = default;
  Application& operator=(Application&& other) noexcept = default;
  explicit Application(const Application& other) noexcept = default;
  Application& operator=(const Application& other) noexcept = default;
  virtual ~Application() noexcept = default;

  virtual void Setup() noexcept = 0;
  virtual void Update() noexcept = 0;
  virtual void Draw() noexcept = 0;
  virtual void DrawImGui() noexcept = 0;
  virtual void TearDown() noexcept = 0;
};