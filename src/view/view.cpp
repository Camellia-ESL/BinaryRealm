#include "view.h"

#include "../app/app.h"
#include "../config/config_manager.h"
#include "animation.h"
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

void RWindowView::opening_anim_(const ImVec2& size) {
  if (opening_anim_val_.get_state() == RAnimValState::PLAYING) {
    float scale = opening_anim_val_.val();
    const auto& theme =
        RConfigsManager::get().get_theme_mngr().get_active_theme();

    // Set the size of the window
    const ImVec2& win_default_size =
        size.x == 0.0f && size.y == 0.0f ? theme->viewport_default_size : size;
    ImVec2 win_size =
        ImVec2(win_default_size.x * scale, win_default_size.y * scale);
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

void RWindowView::closing_anim_() {
  if (closing_anim_val_.get_state() == RAnimValState::PLAYING) {
    float scale = closing_anim_val_.val();

    // Set the size of the window
    const ImVec2 win_size = ImVec2(closing_anim_win_start_size_.x * scale,
                                   closing_anim_win_start_size_.y * scale);
    ImGui::SetWindowSize(win_size);

    // Set the position of the window
    ImGui::SetWindowPos({closing_anim_win_start_pos_.x - (win_size.x / 2.0f),
                         closing_anim_win_start_pos_.y - (win_size.y / 2.0f)});
  }
}

bool RWindowView::begin_frame_(ImGuiWindowFlags flags, const ImVec2& size) {
  opening_anim_(size);
  ImGui::Begin(r_str_to_cstr(get_name() + "##" + uuid_), &open_,
               ImGuiWindowFlags_NoTitleBar | flags);

  if (ImGui::IsViewportFocused() && ImGui::IsKeyPressed(ImGuiKey_Escape)) {
    closing_anim_val_.play();
    closing_anim_win_start_size_ = ImGui::GetWindowSize();
    closing_anim_win_start_pos_ =
        ImGui::GetWindowPos() + ImVec2{closing_anim_win_start_size_.x / 2.0f,
                                       closing_anim_win_start_size_.y / 2.0f};
  }

  closing_anim_();

  return opening_anim_val_.get_state() == RAnimValState::COMPLETED &&
         closing_anim_val_.get_state() == RAnimValState::COMPLETED;
}

void RWindowView::end_frame_() { ImGui::End(); }

void RWindowView::on_spawn() {
  RView::on_spawn();
  opening_anim_val_.play();
}

bool RWindowView::update() {
  // Handle closing animation + view destruction
  if (closing_anim_val_.play_count > 0) open_ = false;

  // Update the opening animation of the view
  opening_anim_val_.update();
  closing_anim_val_.update();

  return RView::update();
}

void RNotificationView::on_spawn() {
  RView::on_spawn();
  opening_anim_val_.concatenate(&closing_anim_val_, 3.0f);
  opening_anim_val_.play();
}

bool RNotificationView::update() {
  // Handle closing animation + view destruction
  if (closing_anim_val_.play_count > 0) open_ = false;

  // Update the opening animation of the view
  opening_anim_val_.update();
  closing_anim_val_.update();

  return RView::update();
}

void RNotificationView::render() {
  opening_anim_();
  closing_anim_();
  ImGui::PushStyleColor(ImGuiCol_Border, {0.34, 0.34, 0.34, 1.0f});
  ImGui::Begin(r_str_to_cstr("##" + uuid_), &open_,
               ImGuiWindowFlags_NoTitleBar);
  ImGui::PopStyleColor();
  ImGui::TextUnformatted(title.c_str());
  ImGui::TextUnformatted(description.c_str());
  ImGui::End();
}

void RNotificationView::opening_anim_() { slide_from_right_anim_(); }

void RNotificationView::closing_anim_() { slide_from_right_anim_(); }

void RNotificationView::slide_from_right_anim_() {
  float anim_multiplier =
      opening_anim_val_.get_state() != RAnimValState::COMPLETED
          ? opening_anim_val_.val()
          : closing_anim_val_.val();
  const auto& theme =
      RConfigsManager::get().get_theme_mngr().get_active_theme();

  ImGui::SetNextWindowSize(theme->notification_size);

  // Set the position to the bottom right of the screen
  auto& main_monitor_rect = RApp::get().get_main_screen().get_rect();
  ImVec2 screen_size = {
      (float)main_monitor_rect.right - main_monitor_rect.left,
      (float)main_monitor_rect.bottom - main_monitor_rect.top};

  ImGui::SetNextWindowPos(
      {screen_size.x -
           ((theme->notification_size.x + HORIZONTAL_GAP_) * anim_multiplier),
       screen_size.y - theme->notification_size.y - VERTICAL_GAP_});

  // Set the alpha
  ImGui::SetNextWindowBgAlpha(ImGui::GetStyle().Colors[ImGuiCol_WindowBg].w *
                              anim_multiplier);
}
