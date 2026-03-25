/*
===========================================================================
OpenMoHAA ImGui Menu - Ported from Volute project
F7 in-game overlay menu with Players, Server, Chat, Game, and Settings tabs
===========================================================================
*/

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void ImGuiMenu_Init(void *sdlWindow, void *glContext);
void ImGuiMenu_Shutdown(void);
int  ImGuiMenu_IsVisible(void);
void ImGuiMenu_Toggle(void);
void ImGuiMenu_Draw(void);
int  ImGuiMenu_ProcessEvent(void *sdlEvent);

#ifdef __cplusplus
}
#endif
