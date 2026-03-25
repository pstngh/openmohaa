/*
===========================================================================
OpenMoHAA ImGui Menu - Ported from Volute project
F7 in-game overlay menu with Game and Server tabs.

This file is compiled as part of the renderer (SDL_RENDERER_SOURCES) so
it uses the renderer import table (ri.*) for engine interaction.
===========================================================================
*/

#ifdef USE_INTERNAL_SDL_HEADERS
#   include "SDL.h"
#else
#   include <SDL.h>
#endif

#include <SDL_opengl.h>

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl2.h"

#include "../renderercommon/tr_common.h"
#include "cl_imgui_menu.h"

// Helper: execute a console command via the renderer import table
static void ExecCmd(const char *cmd)
{
    ri.Cmd_ExecuteText(EXEC_APPEND, cmd);
}

// Helper: get a cvar string value
static const char *GetCvarString(const char *name)
{
    cvar_t *cv = ri.Cvar_Get(name, "", 0);
    return cv ? cv->string : "";
}

// ---------------------------------------------------------------------------
// Menu state
// ---------------------------------------------------------------------------
static bool         s_initialized = false;
static bool         s_visible = false;
static SDL_Window  *s_window = NULL;

// Crosshair types matching Volute
enum CrosshairType {
    CROSSHAIR_ORIGINAL = 0,
    CROSSHAIR_CIRCLE,
    CROSSHAIR_LINE
};

// ---------------------------------------------------------------------------
// Game settings
// ---------------------------------------------------------------------------
static struct {
    // FOV
    float fov;

    // Crosshair
    int   crosshairType;
    float crosshairSize;
    float crosshairThickness;
    float crosshairColor[4];
    float crosshairGap;

    // HUD toggles
    bool showFPS;
    bool drawGun;

    // Rcon
    char rconPassword[128];
    bool rconAuthenticated;
    char rconCommand[512];
} s_settings;

static void ResetSettings(void)
{
    s_settings.fov = 80.0f;
    s_settings.crosshairType = CROSSHAIR_ORIGINAL;
    s_settings.crosshairSize = 20.0f;
    s_settings.crosshairThickness = 1.0f;
    s_settings.crosshairColor[0] = 0.0f;
    s_settings.crosshairColor[1] = 1.0f;
    s_settings.crosshairColor[2] = 0.0f;
    s_settings.crosshairColor[3] = 1.0f;
    s_settings.crosshairGap = 4.0f;
    s_settings.showFPS = false;
    s_settings.drawGun = true;
    s_settings.rconAuthenticated = false;
    memset(s_settings.rconPassword, 0, sizeof(s_settings.rconPassword));
    memset(s_settings.rconCommand, 0, sizeof(s_settings.rconCommand));
}

// ---------------------------------------------------------------------------
// Theme: Half-Life green (from Volute)
// ---------------------------------------------------------------------------
static void SetHalfLifeTheme()
{
    ImGuiStyle &style = ImGui::GetStyle();
    style.WindowRounding = 2.0f;
    style.FrameRounding = 2.0f;
    style.ScrollbarRounding = 2.0f;
    style.GrabRounding = 2.0f;
    style.WindowBorderSize = 1.0f;
    style.FrameBorderSize = 0.0f;
    style.WindowPadding = ImVec2(8, 8);
    style.FramePadding = ImVec2(4, 3);
    style.ItemSpacing = ImVec2(8, 4);
    style.ItemInnerSpacing = ImVec2(4, 4);

    ImVec4 *colors = style.Colors;
    colors[ImGuiCol_Text]                  = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TextDisabled]          = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_WindowBg]              = ImVec4(0.10f, 0.10f, 0.10f, 0.94f);
    colors[ImGuiCol_ChildBg]               = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_PopupBg]               = ImVec4(0.12f, 0.12f, 0.12f, 0.94f);
    colors[ImGuiCol_Border]                = ImVec4(0.30f, 0.50f, 0.10f, 0.50f);
    colors[ImGuiCol_BorderShadow]          = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg]               = ImVec4(0.20f, 0.25f, 0.15f, 0.54f);
    colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.30f, 0.45f, 0.15f, 0.40f);
    colors[ImGuiCol_FrameBgActive]         = ImVec4(0.40f, 0.55f, 0.15f, 0.67f);
    colors[ImGuiCol_TitleBg]               = ImVec4(0.08f, 0.12f, 0.05f, 1.00f);
    colors[ImGuiCol_TitleBgActive]         = ImVec4(0.20f, 0.35f, 0.10f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    colors[ImGuiCol_MenuBarBg]             = ImVec4(0.14f, 0.18f, 0.10f, 1.00f);
    colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
    colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.20f, 0.35f, 0.10f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.30f, 0.50f, 0.15f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.40f, 0.60f, 0.20f, 1.00f);
    colors[ImGuiCol_CheckMark]             = ImVec4(0.40f, 0.80f, 0.15f, 1.00f);
    colors[ImGuiCol_SliderGrab]            = ImVec4(0.30f, 0.55f, 0.15f, 1.00f);
    colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.40f, 0.70f, 0.20f, 1.00f);
    colors[ImGuiCol_Button]                = ImVec4(0.20f, 0.35f, 0.10f, 0.40f);
    colors[ImGuiCol_ButtonHovered]         = ImVec4(0.30f, 0.55f, 0.15f, 1.00f);
    colors[ImGuiCol_ButtonActive]          = ImVec4(0.40f, 0.65f, 0.20f, 1.00f);
    colors[ImGuiCol_Header]                = ImVec4(0.20f, 0.35f, 0.10f, 0.31f);
    colors[ImGuiCol_HeaderHovered]         = ImVec4(0.30f, 0.55f, 0.15f, 0.80f);
    colors[ImGuiCol_HeaderActive]          = ImVec4(0.40f, 0.65f, 0.20f, 1.00f);
    colors[ImGuiCol_Separator]             = ImVec4(0.30f, 0.50f, 0.10f, 0.50f);
    colors[ImGuiCol_SeparatorHovered]      = ImVec4(0.40f, 0.60f, 0.15f, 0.78f);
    colors[ImGuiCol_SeparatorActive]       = ImVec4(0.50f, 0.70f, 0.20f, 1.00f);
    colors[ImGuiCol_ResizeGrip]            = ImVec4(0.20f, 0.35f, 0.10f, 0.20f);
    colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.30f, 0.55f, 0.15f, 0.67f);
    colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.40f, 0.65f, 0.20f, 0.95f);
    colors[ImGuiCol_Tab]                   = ImVec4(0.15f, 0.25f, 0.08f, 0.86f);
    colors[ImGuiCol_TabHovered]            = ImVec4(0.30f, 0.55f, 0.15f, 0.80f);
    colors[ImGuiCol_TabSelected]           = ImVec4(0.25f, 0.45f, 0.12f, 1.00f);
    colors[ImGuiCol_TableHeaderBg]         = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
    colors[ImGuiCol_TableBorderStrong]     = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
    colors[ImGuiCol_TableBorderLight]      = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
    colors[ImGuiCol_TableRowBg]            = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_TableRowBgAlt]         = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
}

// ---------------------------------------------------------------------------
// Tab: Game Settings (FOV, crosshair, HUD)
// ---------------------------------------------------------------------------
static void DrawGameTab()
{
    ImGui::Text("Game Settings");
    ImGui::Separator();

    // FOV
    if (ImGui::CollapsingHeader("Field of View", ImGuiTreeNodeFlags_DefaultOpen)) {
        if (ImGui::SliderFloat("FOV", &s_settings.fov, 60.0f, 120.0f, "%.0f")) {
            char cmd[64];
            snprintf(cmd, sizeof(cmd), "cg_fov %g\n", (double)s_settings.fov);
            ExecCmd(cmd);
        }
        ImGui::SameLine();
        if (ImGui::Button("Reset FOV")) {
            s_settings.fov = 80.0f;
            ExecCmd("cg_fov 80\n");
        }
    }

    // Crosshair settings
    if (ImGui::CollapsingHeader("Crosshair", ImGuiTreeNodeFlags_DefaultOpen)) {
        const char *crosshairTypes[] = {"Original", "Circle", "Lines"};
        if (ImGui::Combo("Type", &s_settings.crosshairType, crosshairTypes, 3)) {
            char cmd[64];
            snprintf(cmd, sizeof(cmd), "cg_crosshair %d\n", s_settings.crosshairType);
            ExecCmd(cmd);
        }

        if (s_settings.crosshairType != CROSSHAIR_ORIGINAL) {
            ImGui::SliderFloat("Size", &s_settings.crosshairSize, 5.0f, 50.0f, "%.0f");
            ImGui::SliderFloat("Thickness", &s_settings.crosshairThickness, 0.5f, 5.0f, "%.1f");
            ImGui::SliderFloat("Gap", &s_settings.crosshairGap, 0.0f, 20.0f, "%.0f");
            ImGui::ColorEdit4("Color", s_settings.crosshairColor, ImGuiColorEditFlags_NoInputs);
        }
    }

    // HUD Options
    if (ImGui::CollapsingHeader("HUD Options")) {
        if (ImGui::Checkbox("Show FPS", &s_settings.showFPS)) {
            ExecCmd(s_settings.showFPS ? "cg_drawfps 1\n" : "cg_drawfps 0\n");
        }
        if (ImGui::Checkbox("Draw Weapon", &s_settings.drawGun)) {
            ExecCmd(s_settings.drawGun ? "cg_drawGun 1\n" : "cg_drawGun 0\n");
        }
    }

    // Graphics quick settings
    if (ImGui::CollapsingHeader("Graphics Quick Settings")) {
        if (ImGui::Button("Low"))    { ExecCmd("exec low.cfg\n"); }
        ImGui::SameLine();
        if (ImGui::Button("Medium")) { ExecCmd("exec medium.cfg\n"); }
        ImGui::SameLine();
        if (ImGui::Button("High"))   { ExecCmd("exec high.cfg\n"); }
    }
}

// ---------------------------------------------------------------------------
// Tab: Server (rcon)
// ---------------------------------------------------------------------------
static void DrawServerTab()
{
    ImGui::Text("Server - Rcon Console");
    ImGui::Separator();

    ImGui::InputText("Rcon Password", s_settings.rconPassword, sizeof(s_settings.rconPassword),
                     ImGuiInputTextFlags_Password);

    ImGui::SameLine();
    if (ImGui::Button("Authenticate")) {
        if (s_settings.rconPassword[0]) {
            char cmd[256];
            snprintf(cmd, sizeof(cmd), "rconpassword %s\n", s_settings.rconPassword);
            ExecCmd(cmd);
            s_settings.rconAuthenticated = true;
        }
    }

    if (s_settings.rconAuthenticated) {
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(0.4f, 0.8f, 0.15f, 1.0f), "Authenticated");
    }

    ImGui::Separator();
    ImGui::Spacing();

    ImGui::Text("Rcon Command:");
    bool sendCmd = ImGui::InputText("##rcon_cmd", s_settings.rconCommand, sizeof(s_settings.rconCommand),
                                    ImGuiInputTextFlags_EnterReturnsTrue);

    ImGui::SameLine();
    if (ImGui::Button("Send") || sendCmd) {
        if (s_settings.rconCommand[0]) {
            char cmd[600];
            snprintf(cmd, sizeof(cmd), "rcon %s\n", s_settings.rconCommand);
            ExecCmd(cmd);
            s_settings.rconCommand[0] = '\0';
        }
    }

    ImGui::Spacing();
    ImGui::Separator();

    ImGui::Text("Quick Actions:");
    if (ImGui::Button("Status"))     { ExecCmd("rcon status\n"); }
    ImGui::SameLine();
    if (ImGui::Button("Map List"))   { ExecCmd("rcon sv maplist\n"); }
    ImGui::SameLine();
    if (ImGui::Button("Restart Map")){ ExecCmd("rcon restart\n"); }
    ImGui::SameLine();
    if (ImGui::Button("Next Map"))   { ExecCmd("rcon sv nextmap\n"); }

    ImGui::Spacing();

    if (ImGui::Button("Server Info")) { ExecCmd("serverinfo\n"); }
    ImGui::SameLine();
    if (ImGui::Button("Client Info")) { ExecCmd("clientinfo\n"); }
}

// ---------------------------------------------------------------------------
// Public API
// ---------------------------------------------------------------------------

void ImGuiMenu_Init(void *sdlWindow, void *glContext)
{
    if (s_initialized) return;

    s_window = (SDL_Window *)sdlWindow;
    ResetSettings();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.IniFilename = NULL;

    SetHalfLifeTheme();

    ImGui_ImplSDL2_InitForOpenGL(s_window, glContext);
    ImGui_ImplOpenGL2_Init();

    s_initialized = true;
    ri.Printf(PRINT_ALL, "ImGui Menu initialized (F7 to toggle)\n");
}

void ImGuiMenu_Shutdown(void)
{
    if (!s_initialized) return;

    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    s_initialized = false;
    s_visible = false;
}

int ImGuiMenu_IsVisible(void)
{
    return s_visible ? 1 : 0;
}

void ImGuiMenu_Toggle(void)
{
    s_visible = !s_visible;
    // Mouse handling is done by the client via Key_SetCatcher
}

void ImGuiMenu_Draw(void)
{
    if (!s_initialized) return;
    if (!s_visible) return;

    // ImGui's OpenGL2 backend (ImGui_ImplOpenGL2_RenderDrawData) already
    // does comprehensive GL state save/restore via glPushAttrib/glPopAttrib.
    // No additional state management needed here.

    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    ImGuiIO &io = ImGui::GetIO();
    ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f),
                           ImGuiCond_FirstUseEver, ImVec2(0.5f, 0.5f));

    ImGui::Begin("OpenMoHAA Menu (F7)", &s_visible,
                 ImGuiWindowFlags_NoCollapse);

    if (ImGui::BeginTabBar("MenuTabs")) {
        if (ImGui::BeginTabItem("Game")) {
            DrawGameTab();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Server")) {
            DrawServerTab();
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }

    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
}

int ImGuiMenu_ProcessEvent(void *sdlEvent)
{
    if (!s_initialized) return 0;
    if (!s_visible) return 0;

    ImGui_ImplSDL2_ProcessEvent((SDL_Event *)sdlEvent);

    ImGuiIO &io = ImGui::GetIO();
    if (io.WantCaptureKeyboard || io.WantCaptureMouse) {
        return 1;
    }
    return 0;
}
