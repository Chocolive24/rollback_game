#pragma once

#include "raylib_wrapper.h"

enum class MusicType : unsigned char {
  kStartMenu,
  kBattle
};

class AudioManager {
 public:
  void Init();
  void Update();
  void Deinit();

  void PlayMusic(MusicType music_type) noexcept;
  [[nodiscard]] float global_volume() const noexcept { return global_volume_; }
  void SetGlobalVolume(float volume) noexcept {
    global_volume_ = volume;
    raylib::SetMusicVolume(start_menu_theme_, global_volume_);
    raylib::SetMusicVolume(battle_theme_, global_volume_);
  }

  bool is_audio_enabled = true;

private:
  raylib::Music start_menu_theme_{};
  raylib::Music battle_theme_{};

  raylib::Music* current_music_ = nullptr;

  float global_volume_ = 0.2f;
  

  /**
   * \brief is_audio_device_initialized_ is a static boolean used to know if the audio
   * device was initialized or not.
   *
   * It is important to init the audio device only once.
   */
  static inline bool is_audio_device_initialized_ = false;
};