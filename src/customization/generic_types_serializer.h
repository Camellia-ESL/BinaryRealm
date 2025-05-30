#pragma once

#include "../../external/imgui/imgui.h"
#include "../../external/nhlomannjson/json.hpp"

using json = nlohmann::json;

inline void to_json(json& j, const ImVec2& v) {
  j = json{{"x", v.x}, {"y", v.y}};
}

inline void from_json(const json& j, ImVec2& v) {
  v.x = j.value("x", 0.0f);
  v.y = j.value("y", 0.0f);
}

inline void to_json(json& j, const ImVec4& v) {
  j = json{{"r", v.x}, {"g", v.y}, {"b", v.z}, {"a", v.w}};
}

inline void from_json(const json& j, ImVec4& v) {
  v.x = j.value("r", 0.0f);
  v.y = j.value("g", 0.0f);
  v.z = j.value("b", 0.0f);
  v.w = j.value("a", 0.0f);
}

inline void to_json(json& j, const ImGuiDir& dir) { j = static_cast<int>(dir); }
inline void from_json(const json& j, ImGuiDir& dir) {
  dir = static_cast<ImGuiDir>(j.get<int>());
}

inline void to_json(json& j, const ImGuiHoveredFlags& flags) {
  j = static_cast<int>(flags);
}
inline void from_json(const json& j, ImGuiHoveredFlags& flags) {
  flags = static_cast<ImGuiHoveredFlags>(j.get<int>());
}

inline void to_json(json& j, const ImGuiStyle& style) {
  j = json{
      {"Alpha", style.Alpha},
      {"DisabledAlpha", style.DisabledAlpha},
      {"WindowPadding", style.WindowPadding},
      {"WindowRounding", style.WindowRounding},
      {"WindowBorderSize", style.WindowBorderSize},
      {"WindowBorderHoverPadding", style.WindowBorderHoverPadding},
      {"WindowMinSize", style.WindowMinSize},
      {"WindowTitleAlign", style.WindowTitleAlign},
      {"WindowMenuButtonPosition",
       static_cast<int>(style.WindowMenuButtonPosition)},
      {"ChildRounding", style.ChildRounding},
      {"ChildBorderSize", style.ChildBorderSize},
      {"PopupRounding", style.PopupRounding},
      {"PopupBorderSize", style.PopupBorderSize},
      {"FramePadding", style.FramePadding},
      {"FrameRounding", style.FrameRounding},
      {"FrameBorderSize", style.FrameBorderSize},
      {"ItemSpacing", style.ItemSpacing},
      {"ItemInnerSpacing", style.ItemInnerSpacing},
      {"CellPadding", style.CellPadding},
      {"TouchExtraPadding", style.TouchExtraPadding},
      {"IndentSpacing", style.IndentSpacing},
      {"ColumnsMinSpacing", style.ColumnsMinSpacing},
      {"ScrollbarSize", style.ScrollbarSize},
      {"ScrollbarRounding", style.ScrollbarRounding},
      {"GrabMinSize", style.GrabMinSize},
      {"GrabRounding", style.GrabRounding},
      {"LogSliderDeadzone", style.LogSliderDeadzone},
      {"ImageBorderSize", style.ImageBorderSize},
      {"TabRounding", style.TabRounding},
      {"TabBorderSize", style.TabBorderSize},
      {"TabCloseButtonMinWidthSelected", style.TabCloseButtonMinWidthSelected},
      {"TabCloseButtonMinWidthUnselected",
       style.TabCloseButtonMinWidthUnselected},
      {"TabBarBorderSize", style.TabBarBorderSize},
      {"TabBarOverlineSize", style.TabBarOverlineSize},
      {"TableAngledHeadersAngle", style.TableAngledHeadersAngle},
      {"TableAngledHeadersTextAlign", style.TableAngledHeadersTextAlign},
      {"ColorButtonPosition", static_cast<int>(style.ColorButtonPosition)},
      {"ButtonTextAlign", style.ButtonTextAlign},
      {"SelectableTextAlign", style.SelectableTextAlign},
      {"SeparatorTextBorderSize", style.SeparatorTextBorderSize},
      {"SeparatorTextAlign", style.SeparatorTextAlign},
      {"SeparatorTextPadding", style.SeparatorTextPadding},
      {"DisplayWindowPadding", style.DisplayWindowPadding},
      {"DisplaySafeAreaPadding", style.DisplaySafeAreaPadding},
      {"DockingSeparatorSize", style.DockingSeparatorSize},
      {"MouseCursorScale", style.MouseCursorScale},
      {"AntiAliasedLines", style.AntiAliasedLines},
      {"AntiAliasedLinesUseTex", style.AntiAliasedLinesUseTex},
      {"AntiAliasedFill", style.AntiAliasedFill},
      {"CurveTessellationTol", style.CurveTessellationTol},
      {"CircleTessellationMaxError", style.CircleTessellationMaxError},
      {"GlassBlurBackground", style.GlassBlurBackground},
      {"HoverStationaryDelay", style.HoverStationaryDelay},
      {"HoverDelayShort", style.HoverDelayShort},
      {"HoverDelayNormal", style.HoverDelayNormal},
      {"HoverFlagsForTooltipMouse",
       static_cast<int>(style.HoverFlagsForTooltipMouse)},
      {"HoverFlagsForTooltipNav",
       static_cast<int>(style.HoverFlagsForTooltipNav)}};
  for (int i = 0; i < ImGuiCol_COUNT; ++i) {
    j["Colors"].emplace_back(style.Colors[i]);
  }
}

inline void from_json(const json& j, ImGuiStyle& style) {
  style.Alpha = j.value("Alpha", style.Alpha);
  style.DisabledAlpha = j.value("DisabledAlpha", style.DisabledAlpha);
  style.WindowPadding = j.value("WindowPadding", style.WindowPadding);
  style.WindowRounding = j.value("WindowRounding", style.WindowRounding);
  style.WindowBorderSize = j.value("WindowBorderSize", style.WindowBorderSize);
  style.WindowBorderHoverPadding =
      j.value("WindowBorderHoverPadding", style.WindowBorderHoverPadding);
  style.WindowMinSize = j.value("WindowMinSize", style.WindowMinSize);
  style.WindowTitleAlign = j.value("WindowTitleAlign", style.WindowTitleAlign);
  style.WindowMenuButtonPosition = static_cast<ImGuiDir>(
      j.value("WindowMenuButtonPosition",
              static_cast<int>(style.WindowMenuButtonPosition)));
  style.ChildRounding = j.value("ChildRounding", style.ChildRounding);
  style.ChildBorderSize = j.value("ChildBorderSize", style.ChildBorderSize);
  style.PopupRounding = j.value("PopupRounding", style.PopupRounding);
  style.PopupBorderSize = j.value("PopupBorderSize", style.PopupBorderSize);
  style.FramePadding = j.value("FramePadding", style.FramePadding);
  style.FrameRounding = j.value("FrameRounding", style.FrameRounding);
  style.FrameBorderSize = j.value("FrameBorderSize", style.FrameBorderSize);
  style.ItemSpacing = j.value("ItemSpacing", style.ItemSpacing);
  style.ItemInnerSpacing = j.value("ItemInnerSpacing", style.ItemInnerSpacing);
  style.CellPadding = j.value("CellPadding", style.CellPadding);
  style.TouchExtraPadding =
      j.value("TouchExtraPadding", style.TouchExtraPadding);
  style.IndentSpacing = j.value("IndentSpacing", style.IndentSpacing);
  style.ColumnsMinSpacing =
      j.value("ColumnsMinSpacing", style.ColumnsMinSpacing);
  style.ScrollbarSize = j.value("ScrollbarSize", style.ScrollbarSize);
  style.ScrollbarRounding =
      j.value("ScrollbarRounding", style.ScrollbarRounding);
  style.GrabMinSize = j.value("GrabMinSize", style.GrabMinSize);
  style.GrabRounding = j.value("GrabRounding", style.GrabRounding);
  style.LogSliderDeadzone =
      j.value("LogSliderDeadzone", style.LogSliderDeadzone);
  style.ImageBorderSize = j.value("ImageBorderSize", style.ImageBorderSize);
  style.TabRounding = j.value("TabRounding", style.TabRounding);
  style.TabBorderSize = j.value("TabBorderSize", style.TabBorderSize);
  style.TabCloseButtonMinWidthSelected = j.value(
      "TabCloseButtonMinWidthSelected", style.TabCloseButtonMinWidthSelected);
  style.TabCloseButtonMinWidthUnselected =
      j.value("TabCloseButtonMinWidthUnselected",
              style.TabCloseButtonMinWidthUnselected);
  style.TabBarBorderSize = j.value("TabBarBorderSize", style.TabBarBorderSize);
  style.TabBarOverlineSize =
      j.value("TabBarOverlineSize", style.TabBarOverlineSize);
  style.TableAngledHeadersAngle =
      j.value("TableAngledHeadersAngle", style.TableAngledHeadersAngle);
  style.TableAngledHeadersTextAlign =
      j.value("TableAngledHeadersTextAlign", style.TableAngledHeadersTextAlign);
  style.ColorButtonPosition = static_cast<ImGuiDir>(j.value(
      "ColorButtonPosition", static_cast<int>(style.ColorButtonPosition)));
  style.ButtonTextAlign = j.value("ButtonTextAlign", style.ButtonTextAlign);
  style.SelectableTextAlign =
      j.value("SelectableTextAlign", style.SelectableTextAlign);
  style.SeparatorTextBorderSize =
      j.value("SeparatorTextBorderSize", style.SeparatorTextBorderSize);
  style.SeparatorTextAlign =
      j.value("SeparatorTextAlign", style.SeparatorTextAlign);
  style.SeparatorTextPadding =
      j.value("SeparatorTextPadding", style.SeparatorTextPadding);
  style.DisplayWindowPadding =
      j.value("DisplayWindowPadding", style.DisplayWindowPadding);
  style.DisplaySafeAreaPadding =
      j.value("DisplaySafeAreaPadding", style.DisplaySafeAreaPadding);
  style.DockingSeparatorSize =
      j.value("DockingSeparatorSize", style.DockingSeparatorSize);
  style.MouseCursorScale = j.value("MouseCursorScale", style.MouseCursorScale);
  style.AntiAliasedLines = j.value("AntiAliasedLines", style.AntiAliasedLines);
  style.AntiAliasedLinesUseTex =
      j.value("AntiAliasedLinesUseTex", style.AntiAliasedLinesUseTex);
  style.AntiAliasedFill = j.value("AntiAliasedFill", style.AntiAliasedFill);
  style.CurveTessellationTol =
      j.value("CurveTessellationTol", style.CurveTessellationTol);
  style.CircleTessellationMaxError =
      j.value("CircleTessellationMaxError", style.CircleTessellationMaxError);
  style.GlassBlurBackground =
      j.value("GlassBlurBackground", style.GlassBlurBackground);
  style.HoverStationaryDelay =
      j.value("HoverStationaryDelay", style.HoverStationaryDelay);
  style.HoverDelayShort = j.value("HoverDelayShort", style.HoverDelayShort);
  style.HoverDelayNormal = j.value("HoverDelayNormal", style.HoverDelayNormal);
  style.HoverFlagsForTooltipMouse = static_cast<ImGuiHoveredFlags>(
      j.value("HoverFlagsForTooltipMouse",
              static_cast<int>(style.HoverFlagsForTooltipMouse)));
  style.HoverFlagsForTooltipNav = static_cast<ImGuiHoveredFlags>(
      j.value("HoverFlagsForTooltipNav",
              static_cast<int>(style.HoverFlagsForTooltipNav)));

  if (j.contains("Colors")) {
    for (size_t i = 0; i < ImGuiCol_COUNT && i < j["Colors"].size(); ++i) {
      style.Colors[i] = j["Colors"][i];
    }
  }
}
