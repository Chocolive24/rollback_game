#include "audio_manager.h"

void AudioManager::Init() {
  if (!is_audio_device_initialized_) {
    raylib::InitAudioDevice();
    is_audio_device_initialized_ = true;
  }

  start_menu_theme_ = raylib::LoadMusicStream("data/sounds/start_menu.wav");
  start_menu_theme_.looping = true;
  raylib::SetMusicVolume(start_menu_theme_, global_volume_);

  battle_theme_ = raylib::LoadMusicStream("data/sounds/battle.wav");
  battle_theme_.looping = true;
  raylib::SetMusicVolume(battle_theme_, global_volume_);
}

void AudioManager::Update() {
  if (current_music_ != nullptr && is_audio_enabled) {
    raylib::UpdateMusicStream(*current_music_);
  }
}

void AudioManager::Deinit() {
  raylib::UnloadMusicStream(start_menu_theme_);
  raylib::UnloadMusicStream(battle_theme_);

  if (is_audio_device_initialized_)
  {
    raylib::CloseAudioDevice();
    is_audio_device_initialized_ = false;
  }
}

void AudioManager::PlayMusic(const MusicType music_type) noexcept {
  if (current_music_ != nullptr) {
    raylib::StopMusicStream(*current_music_);
  }

  switch (music_type) {
  case MusicType::kStartMenu:
      current_music_ = &start_menu_theme_;
    break;
  case MusicType::kBattle:
    current_music_ = &battle_theme_;
    break;
  default:
    break;
  }

  raylib::PlayMusicStream(*current_music_);
}
