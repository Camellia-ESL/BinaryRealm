#include "view.h"

#include "../app/app.h"
#include "../config/config_manager.h"
#include "viewpool.h"

bool RView::update() {
  // Destroy's the view when it get's closed
  if (!open_) {
    RViewPool::get().destroy(this);
    return false;
  }

  return true;
}

void RView::init() { uuid_ = r_str_uuid(); }

void RWindowView::opening_anim() {
  if (opening_anim_val_.is_playing) {
    float scale = opening_anim_val_.val();
    const auto& theme =
        RConfigsManager::get().get_theme_mngr().get_active_theme();

    // Set the size of the window
    ImVec2 win_size = ImVec2(theme->viewport_default_size.x * scale,
                             theme->viewport_default_size.y * scale);
    ImGui::SetNextWindowSize(win_size);

    // Set the position to the center of the screen
    auto& main_monitor_rect = RApp::get().get_main_screen().get_rect();
    ImVec2 screen_center = {
        (main_monitor_rect.right - main_monitor_rect.left) / 2.0f,
        (main_monitor_rect.bottom - main_monitor_rect.top) / 2.0f};

    ImGui::SetNextWindowPos({screen_center.x - (win_size.x / 2.0f),
                             screen_center.y - (win_size.y / 2.0f)});

    // Set the alpha
    ImGui::SetNextWindowBgAlpha(ImGui::GetStyle().Colors[ImGuiCol_WindowBg].w *
                                scale);
  }
}

void RWindowView::closing_anim() {
  if (closing_anim_val_.is_playing) {
    float scale = closing_anim_val_.val();
    const auto& theme =
        RConfigsManager::get().get_theme_mngr().get_active_theme();

    // Set the size of the window
    const ImVec2 win_size = ImVec2(closing_anim_win_start_size_.x * scale,
                                   closing_anim_win_start_size_.y * scale);
    ImGui::SetWindowSize(win_size);

    // Set the position of the window
    ImGui::SetWindowPos({closing_anim_win_start_pos_.x - (win_size.x / 2.0f),
                         closing_anim_win_start_pos_.y - (win_size.y / 2.0f)});
  }
}

void RWindowView::begin_frame(ImGuiWindowFlags flags) {
  opening_anim();
  ImGui::Begin(r_str_to_cstr(get_name() + "##" + uuid_), &open_,
               ImGuiWindowFlags_NoTitleBar | flags);

  if (ImGui::IsViewportFocused() && ImGui::IsKeyPressed(ImGuiKey_Escape)) {
    closing_anim_val_.play();
    closing_anim_win_start_size_ = ImGui::GetWindowSize();
    closing_anim_win_start_pos_ =
        ImGui::GetWindowPos() + ImVec2{closing_anim_win_start_size_.x / 2.0f,
                                       closing_anim_win_start_size_.y / 2.0f};
  }

  closing_anim();
}

void RWindowView::end_frame() { ImGui::End(); }

void RWindowView::on_spawn() {
  RView::on_spawn();
  opening_anim_val_.play();
}

bool RWindowView::update() {
  // Handle closing animation + view destruction
  if (closing_anim_val_.play_count > 0) open_ = false;

  // Update the opening animation of the view
  opening_anim_val_.update(ImGui::GetIO().DeltaTime);
  closing_anim_val_.update(ImGui::GetIO().DeltaTime);

  return RView::update();
}
