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

private:
  raylib::Music start_menu_theme_{};
  raylib::Music battle_theme_{};

  raylib::Music* current_music_ = nullptr;

  /**
   * \brief is_audio_device_initialized_ is a static boolean used to know if the audio
   * device was initialized or not.
   *
   * It is important to init the audio device only once.
   */
  static inline bool is_audio_device_initialized_ = false;
};