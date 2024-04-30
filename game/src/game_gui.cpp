#include "game_gui.h"

#include <string>

void GameGui::Draw(
    const raylib::RenderTexture2D& render_target) const noexcept {

  const auto player_1_life_ui_pos_x =
      kPlayer1LifeUiPercentageX * render_target.texture.width;
  const auto player_2_life_ui_pos_x =
      kPlayer2LifeUiPercentageX * render_target.texture.width;
  const auto player_lives_ui_pos_y =
      kPlayersLivesUiPercentageY * render_target.texture.height;

  const std::string player_1_life_txt =
      std::to_string(local_game_manager_->player_manager().GetPlayerHp(0));
  const std::string player_2_life_txt =
      std::to_string(local_game_manager_->player_manager().GetPlayerHp(1));

  raylib::DrawRaylibText(player_1_life_txt.c_str(), player_1_life_ui_pos_x,
                         player_lives_ui_pos_y, 30, raylib::WHITE);
  raylib::DrawRaylibText(player_2_life_txt.c_str(), player_2_life_ui_pos_x,
                         player_lives_ui_pos_y, 30, raylib::WHITE);
}
