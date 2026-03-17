/*
===========================================================================
Copyright (C) 2026 the OpenMoHAA team

This file is part of OpenMoHAA source code.

OpenMoHAA source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

OpenMoHAA source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with OpenMoHAA source code; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/

// launcher_win32.cpp: Win32 GUI for the OpenMoHAA launcher

#ifdef _WIN32

#    include "launcher.h"

#    ifndef WIN32_LEAN_AND_MEAN
#        define WIN32_LEAN_AND_MEAN
#    endif
#    include <Windows.h>
#    include <CommCtrl.h>

#    pragma comment(lib, "comctl32.lib")

// Colors
#    define CLR_BG       RGB(240, 240, 240)
#    define CLR_BG_EDIT  RGB(255, 255, 255)
#    define CLR_TEXT      RGB(0, 0, 0)

// Control IDs
#    define ID_EDIT_IP       101
#    define ID_EDIT_PASS     103
#    define ID_EDIT_RCON     105
#    define ID_EDIT_NICKNAME 106
#    define ID_RADIO_AA      110
#    define ID_RADIO_SH      111
#    define ID_RADIO_BT      112
#    define ID_BTN_CONNECT   120
#    define ID_BTN_BM_0      130
#    define ID_BTN_BMSAVE    140
#    define ID_BTN_BMDEL     150

static HWND             hEditIP;
static HWND             hEditPass;
static HWND             hEditRcon;
static HWND             hEditNickname;
static HWND             hRadioAA;
static HWND             hRadioSH;
static HWND             hRadioBT;
static HWND             hBtnBookmark[MAX_BOOKMARKS];
static HWND             hBtnBmSave[MAX_BOOKMARKS];
static HWND             hBtnBmDel[MAX_BOOKMARKS];
static HWND             hStatusLabel;
static LauncherSettings currentSettings;
static HFONT            hFont;

static void GetWindowString(HWND hwnd, std::string& out)
{
    int len = GetWindowTextLengthA(hwnd);
    if (len <= 0) {
        out.clear();
        return;
    }
    out.resize(len);
    GetWindowTextA(hwnd, &out[0], len + 1);
}

static int GetSelectedGame()
{
    if (SendMessageA(hRadioSH, BM_GETCHECK, 0, 0) == BST_CHECKED) {
        return 1;
    }
    if (SendMessageA(hRadioBT, BM_GETCHECK, 0, 0) == BST_CHECKED) {
        return 2;
    }
    return 0;
}

static void SetSelectedGame(int gameType)
{
    SendMessageA(hRadioAA, BM_SETCHECK, gameType == 0 ? BST_CHECKED : BST_UNCHECKED, 0);
    SendMessageA(hRadioSH, BM_SETCHECK, gameType == 1 ? BST_CHECKED : BST_UNCHECKED, 0);
    SendMessageA(hRadioBT, BM_SETCHECK, gameType == 2 ? BST_CHECKED : BST_UNCHECKED, 0);
}

static void ReadCurrentFields()
{
    GetWindowString(hEditIP, currentSettings.ip);
    GetWindowString(hEditPass, currentSettings.password);
    GetWindowString(hEditRcon, currentSettings.rconPassword);
    GetWindowString(hEditNickname, currentSettings.nickname);
    currentSettings.gameType = GetSelectedGame();
}

static void UpdateStatusLabel()
{
    std::string text = "Currently connected as: ";
    if (!currentSettings.nickname.empty()) {
        text += currentSettings.nickname;
    } else {
        text += "(no nickname set)";
    }
    SetWindowTextA(hStatusLabel, text.c_str());
}

static void UpdateBookmarkButton(int index)
{
    if (currentSettings.bookmarks[index].name.empty()) {
        SetWindowTextA(hBtnBookmark[index], "(empty)");
        EnableWindow(hBtnBookmark[index], FALSE);
        EnableWindow(hBtnBmDel[index], FALSE);
    } else {
        SetWindowTextA(hBtnBookmark[index], currentSettings.bookmarks[index].name.c_str());
        EnableWindow(hBtnBookmark[index], TRUE);
        EnableWindow(hBtnBmDel[index], TRUE);
    }
}

static void LoadBookmark(int index)
{
    const Bookmark& bm = currentSettings.bookmarks[index];
    if (bm.name.empty()) {
        return;
    }
    SetWindowTextA(hEditIP, bm.ip.c_str());
    SetWindowTextA(hEditPass, bm.password.c_str());
    SetWindowTextA(hEditRcon, bm.rconPassword.c_str());
}

// Name input popup window
static HWND hNamePopup;
static HWND hNameEdit;
static HWND hNameOK;
static HWND hNameCancel;
static BOOL namePopupConfirmed;
static char namePopupResult[64];

static LRESULT CALLBACK NamePopupProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
    case WM_COMMAND: {
        int id = LOWORD(wParam);
        if (id == 1001) { // OK
            namePopupConfirmed = TRUE;
            GetWindowTextA(hNameEdit, namePopupResult, sizeof(namePopupResult));
            DestroyWindow(hwnd);
            return 0;
        }
        if (id == 1002) { // Cancel
            namePopupConfirmed = FALSE;
            DestroyWindow(hwnd);
            return 0;
        }
        break;
    }

    case WM_KEYDOWN:
        if (wParam == VK_RETURN) {
            namePopupConfirmed = TRUE;
            PostMessageA(hwnd, WM_CLOSE, 0, 0);
            return 0;
        }
        if (wParam == VK_ESCAPE) {
            namePopupConfirmed = FALSE;
            PostMessageA(hwnd, WM_CLOSE, 0, 0);
            return 0;
        }
        break;

    case WM_CLOSE:
        DestroyWindow(hwnd);
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProcA(hwnd, msg, wParam, lParam);
}

static BOOL PromptForName(HWND hwndParent, const char *defaultName, char *outName, int outSize)
{
    // Register popup class once
    static BOOL registered = FALSE;
    if (!registered) {
        WNDCLASSA wc    = {};
        wc.lpfnWndProc  = NamePopupProc;
        wc.hInstance     = GetModuleHandle(NULL);
        wc.lpszClassName = "OMLauncherNamePopup";
        wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
        wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
        RegisterClassA(&wc);
        registered = TRUE;
    }

    // Center on parent
    RECT parentRect;
    GetWindowRect(hwndParent, &parentRect);
    int popW = 300;
    int popH = 110;
    int popX = parentRect.left + ((parentRect.right - parentRect.left) - popW) / 2;
    int popY = parentRect.top + ((parentRect.bottom - parentRect.top) - popH) / 2;

    hNamePopup = CreateWindowA(
        "OMLauncherNamePopup",
        "Bookmark Name",
        WS_POPUP | WS_CAPTION | WS_SYSMENU,
        popX,
        popY,
        popW,
        popH,
        hwndParent,
        NULL,
        GetModuleHandle(NULL),
        NULL
    );

    HINSTANCE hInst = GetModuleHandle(NULL);

    HWND hLabel = CreateWindowA("STATIC", "Name:", WS_CHILD | WS_VISIBLE, 10, 12, 40, 20, hNamePopup, NULL, hInst, NULL);
    hNameEdit = CreateWindowA(
        "EDIT", defaultName ? defaultName : "", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, 55, 10, 220, 22, hNamePopup, NULL, hInst, NULL
    );
    hNameOK = CreateWindowA(
        "BUTTON", "OK", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 70, 42, 70, 24, hNamePopup, (HMENU)1001, hInst, NULL
    );
    hNameCancel = CreateWindowA(
        "BUTTON", "Cancel", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 155, 42, 70, 24, hNamePopup, (HMENU)1002, hInst, NULL
    );

    if (hFont) {
        SendMessageA(hLabel, WM_SETFONT, (WPARAM)hFont, TRUE);
        SendMessageA(hNameEdit, WM_SETFONT, (WPARAM)hFont, TRUE);
        SendMessageA(hNameOK, WM_SETFONT, (WPARAM)hFont, TRUE);
        SendMessageA(hNameCancel, WM_SETFONT, (WPARAM)hFont, TRUE);
    }

    // Select all text
    SendMessageA(hNameEdit, EM_SETSEL, 0, -1);
    SetFocus(hNameEdit);

    // Disable parent
    EnableWindow(hwndParent, FALSE);

    ShowWindow(hNamePopup, SW_SHOW);
    UpdateWindow(hNamePopup);

    namePopupConfirmed = FALSE;

    // Modal message loop
    MSG msg;
    while (GetMessageA(&msg, NULL, 0, 0)) {
        // Handle Enter/Escape in the edit field
        if (msg.message == WM_KEYDOWN && msg.hwnd == hNameEdit) {
            if (msg.wParam == VK_RETURN) {
                namePopupConfirmed = TRUE;
                GetWindowTextA(hNameEdit, namePopupResult, sizeof(namePopupResult));
                DestroyWindow(hNamePopup);
                continue;
            }
            if (msg.wParam == VK_ESCAPE) {
                namePopupConfirmed = FALSE;
                DestroyWindow(hNamePopup);
                continue;
            }
        }
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }

    // Re-enable parent
    EnableWindow(hwndParent, TRUE);
    SetForegroundWindow(hwndParent);

    if (namePopupConfirmed && namePopupResult[0] != '\0') {
        strncpy(outName, namePopupResult, outSize - 1);
        outName[outSize - 1] = '\0';
        return TRUE;
    }
    return FALSE;
}

static void SaveBookmark(int index, HWND hwnd)
{
    ReadCurrentFields();

    // Default name: existing bookmark name, then IP, then "Bookmark N"
    const char *defaultName;
    char        fallback[32];
    if (!currentSettings.bookmarks[index].name.empty()) {
        defaultName = currentSettings.bookmarks[index].name.c_str();
    } else if (!currentSettings.ip.empty()) {
        defaultName = currentSettings.ip.c_str();
    } else {
        snprintf(fallback, sizeof(fallback), "Bookmark %d", index + 1);
        defaultName = fallback;
    }

    char nameBuf[64];
    if (!PromptForName(hwnd, defaultName, nameBuf, sizeof(nameBuf))) {
        return; // User cancelled
    }

    currentSettings.bookmarks[index].name        = nameBuf;
    currentSettings.bookmarks[index].ip           = currentSettings.ip;
    currentSettings.bookmarks[index].password     = currentSettings.password;
    currentSettings.bookmarks[index].rconPassword = currentSettings.rconPassword;

    UpdateBookmarkButton(index);
    SaveSettings(currentSettings);
}

static void DeleteBookmark(int index)
{
    currentSettings.bookmarks[index] = Bookmark();
    UpdateBookmarkButton(index);
    SaveSettings(currentSettings);
}

static void SetFontOnChildren(HWND hwnd, HFONT font)
{
    HWND child = GetWindow(hwnd, GW_CHILD);
    while (child) {
        SendMessageA(child, WM_SETFONT, (WPARAM)font, TRUE);
        child = GetNextWindow(child, GW_HWNDNEXT);
    }
}

static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
    case WM_COMMAND: {
        int  id   = LOWORD(wParam);
        int  code = HIWORD(wParam);

        if (id == ID_BTN_CONNECT) {
            ReadCurrentFields();
            SaveSettings(currentSettings);
            LaunchGame(currentSettings);
            // Close the launcher after spawning the game
            DestroyWindow(hwnd);
            return 0;
        }

        // Update status label when nickname changes
        if (id == ID_EDIT_NICKNAME && code == EN_CHANGE) {
            GetWindowString(hEditNickname, currentSettings.nickname);
            UpdateStatusLabel();
            return 0;
        }

        if (id >= ID_BTN_BM_0 && id < ID_BTN_BM_0 + MAX_BOOKMARKS) {
            LoadBookmark(id - ID_BTN_BM_0);
            return 0;
        }

        if (id >= ID_BTN_BMSAVE && id < ID_BTN_BMSAVE + MAX_BOOKMARKS) {
            SaveBookmark(id - ID_BTN_BMSAVE, hwnd);
            return 0;
        }

        if (id >= ID_BTN_BMDEL && id < ID_BTN_BMDEL + MAX_BOOKMARKS) {
            DeleteBookmark(id - ID_BTN_BMDEL);
            return 0;
        }

        return 0;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProcA(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    (void)hPrevInstance;
    (void)lpCmdLine;

    InitCommonControls();

    // Use the system default GUI font
    NONCLIENTMETRICSA ncm = {};
    ncm.cbSize            = sizeof(ncm);
    SystemParametersInfoA(SPI_GETNONCLIENTMETRICS, sizeof(ncm), &ncm, 0);
    hFont = CreateFontIndirectA(&ncm.lfMessageFont);

    WNDCLASSA wc    = {};
    wc.lpfnWndProc  = WndProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = "OpenMoHAALauncher";
    wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    RegisterClassA(&wc);

    int winW = 390;
    int winH = 400;

    HWND hwnd = CreateWindowA(
        "OpenMoHAALauncher",
        "OpenMoHAA Launcher",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        winW,
        winH,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    // Margins and layout constants
    int margin  = 10;
    int innerX  = margin + 10;
    int innerW  = winW - 2 * margin - 40;
    int editH   = 22;
    int labelW  = 65;
    int editX   = innerX + labelW + 5;
    int editW   = innerW - labelW - 5;

    // ---- Server Settings group ----
    int grpY  = 5;
    int grpH  = 128;
    HWND hGrp1 = CreateWindowA(
        "BUTTON", "Server Settings",
        WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
        margin, grpY, winW - 2 * margin - 20, grpH,
        hwnd, NULL, hInstance, NULL
    );

    int y = grpY + 20;

    CreateWindowA("STATIC", "IP:", WS_CHILD | WS_VISIBLE, innerX, y + 2, labelW, 20, hwnd, NULL, hInstance, NULL);
    hEditIP = CreateWindowA("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, editX, y, editW, editH, hwnd, (HMENU)ID_EDIT_IP, hInstance, NULL);
    y += 28;

    CreateWindowA("STATIC", "Password:", WS_CHILD | WS_VISIBLE, innerX, y + 2, labelW, 20, hwnd, NULL, hInstance, NULL);
    hEditPass = CreateWindowA("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | ES_PASSWORD, editX, y, editW, editH, hwnd, (HMENU)ID_EDIT_PASS, hInstance, NULL);
    y += 28;

    CreateWindowA("STATIC", "RCON:", WS_CHILD | WS_VISIBLE, innerX, y + 2, labelW, 20, hwnd, NULL, hInstance, NULL);
    hEditRcon = CreateWindowA("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | ES_PASSWORD, editX, y, editW, editH, hwnd, (HMENU)ID_EDIT_RCON, hInstance, NULL);
    y += 28;

    // Connect button and Nickname on same row inside server settings group
    int connectW = 80;
    CreateWindowA(
        "BUTTON", "Connect",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        innerX, y, connectW, 26,
        hwnd, (HMENU)ID_BTN_CONNECT, hInstance, NULL
    );

    int nickEditX = innerX + connectW + 10;
    int nickEditW = editX + editW - nickEditX;
    hEditNickname = CreateWindowA("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, nickEditX, y, nickEditW, editH, hwnd, (HMENU)ID_EDIT_NICKNAME, hInstance, NULL);

    // ---- Game Selection group ----
    int grp2Y = grpY + grpH + 8;
    int grp2H = 50;
    CreateWindowA(
        "BUTTON", "Game",
        WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
        margin, grp2Y, winW - 2 * margin - 20, grp2H,
        hwnd, NULL, hInstance, NULL
    );

    int radioY = grp2Y + 20;
    int radioW = 100;
    hRadioAA = CreateWindowA("BUTTON", "Allied Assault", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP, innerX, radioY, radioW, 20, hwnd, (HMENU)ID_RADIO_AA, hInstance, NULL);
    hRadioSH = CreateWindowA("BUTTON", "Spearhead", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, innerX + radioW + 5, radioY, radioW, 20, hwnd, (HMENU)ID_RADIO_SH, hInstance, NULL);
    hRadioBT = CreateWindowA("BUTTON", "Breakthrough", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, innerX + 2 * (radioW + 5), radioY, radioW, 20, hwnd, (HMENU)ID_RADIO_BT, hInstance, NULL);

    // ---- Bookmarks group ----
    int grp3Y = grp2Y + grp2H + 8;
    int bmRows = MAX_BOOKMARKS;
    int grp3H  = 22 + bmRows * 28 + 5;
    CreateWindowA(
        "BUTTON", "Bookmarks",
        WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
        margin, grp3Y, winW - 2 * margin - 20, grp3H,
        hwnd, NULL, hInstance, NULL
    );

    int bmY    = grp3Y + 20;
    int bmBtnW = innerW - 90;
    int saveW  = 45;
    int delW   = 30;

    for (int i = 0; i < MAX_BOOKMARKS; i++) {
        hBtnBookmark[i] = CreateWindowA(
            "BUTTON", "(empty)",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            innerX, bmY, bmBtnW, 24,
            hwnd, (HMENU)(UINT_PTR)(ID_BTN_BM_0 + i), hInstance, NULL
        );
        hBtnBmSave[i] = CreateWindowA(
            "BUTTON", "Save",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            innerX + bmBtnW + 5, bmY, saveW, 24,
            hwnd, (HMENU)(UINT_PTR)(ID_BTN_BMSAVE + i), hInstance, NULL
        );
        hBtnBmDel[i] = CreateWindowA(
            "BUTTON", "X",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            innerX + bmBtnW + 5 + saveW + 5, bmY, delW, 24,
            hwnd, (HMENU)(UINT_PTR)(ID_BTN_BMDEL + i), hInstance, NULL
        );
        bmY += 28;
    }

    // Status label at the bottom
    int statusY = grp3Y + grp3H + 8;
    hStatusLabel = CreateWindowA(
        "STATIC", "Currently connected as: (no nickname set)",
        WS_CHILD | WS_VISIBLE,
        margin, statusY, winW - 2 * margin - 20, 20,
        hwnd, NULL, hInstance, NULL
    );

    // Apply font to all children
    SetFontOnChildren(hwnd, hFont);

    // Load saved settings
    currentSettings = LoadSettings();
    SetWindowTextA(hEditIP, currentSettings.ip.c_str());
    SetWindowTextA(hEditPass, currentSettings.password.c_str());
    SetWindowTextA(hEditRcon, currentSettings.rconPassword.c_str());
    SetWindowTextA(hEditNickname, currentSettings.nickname.c_str());
    SetSelectedGame(currentSettings.gameType);
    UpdateStatusLabel();

    for (int i = 0; i < MAX_BOOKMARKS; i++) {
        UpdateBookmarkButton(i);
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessageA(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }

    if (hFont) {
        DeleteObject(hFont);
    }

    return (int)msg.wParam;
}

#endif // _WIN32
