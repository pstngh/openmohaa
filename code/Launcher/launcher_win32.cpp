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

// launcher_win32.cpp: Win32 GUI for the OpenMoHAA launcher (dark theme)

#ifdef _WIN32

#    include "launcher.h"

#    ifndef WIN32_LEAN_AND_MEAN
#        define WIN32_LEAN_AND_MEAN
#    endif
#    include <Windows.h>
#    include <CommCtrl.h>

#    pragma comment(lib, "comctl32.lib")

// Dark theme colors
#    define CLR_BG       RGB(30, 30, 30)
#    define CLR_BG_EDIT  RGB(45, 45, 45)
#    define CLR_TEXT      RGB(220, 220, 220)
#    define CLR_BTN      RGB(55, 55, 55)
#    define CLR_BTN_HOT  RGB(70, 70, 70)
#    define CLR_BTN_DOWN RGB(40, 40, 40)
#    define CLR_ACCENT   RGB(80, 120, 60)
#    define CLR_BORDER   RGB(80, 80, 80)

// Control IDs
#    define ID_EDIT_IP     101
#    define ID_EDIT_PASS   103
#    define ID_EDIT_RCON   105
#    define ID_RADIO_AA    110
#    define ID_RADIO_SH    111
#    define ID_RADIO_BT    112
#    define ID_BTN_LAUNCH  120
#    define ID_BTN_BM_0    130 // bookmark load buttons: 130-133
#    define ID_BTN_BMSAVE  140 // bookmark save buttons: 140-143
#    define ID_BTN_BMDEL   150 // bookmark delete buttons: 150-153

static HWND             hEditIP;
static HWND             hEditPass;
static HWND             hEditRcon;
static HWND             hRadioAA;
static HWND             hRadioSH;
static HWND             hRadioBT;
static HWND             hBtnBookmark[MAX_BOOKMARKS];
static HWND             hBtnBmSave[MAX_BOOKMARKS];
static HWND             hBtnBmDel[MAX_BOOKMARKS];
static LauncherSettings currentSettings;

static HBRUSH hBrushBg;
static HBRUSH hBrushEdit;

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
    currentSettings.gameType = GetSelectedGame();
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

static void DrawDarkButton(LPDRAWITEMSTRUCT dis);

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

    case WM_CTLCOLORSTATIC: {
        HDC hdc = (HDC)wParam;
        SetTextColor(hdc, CLR_TEXT);
        SetBkColor(hdc, CLR_BG);
        return (LRESULT)hBrushBg;
    }

    case WM_CTLCOLOREDIT: {
        HDC hdc = (HDC)wParam;
        SetTextColor(hdc, CLR_TEXT);
        SetBkColor(hdc, CLR_BG_EDIT);
        return (LRESULT)hBrushEdit;
    }

    case WM_DRAWITEM: {
        LPDRAWITEMSTRUCT dis = (LPDRAWITEMSTRUCT)lParam;
        DrawDarkButton(dis);
        return TRUE;
    }

    case WM_ERASEBKGND: {
        HDC  hdc = (HDC)wParam;
        RECT rc;
        GetClientRect(hwnd, &rc);
        FillRect(hdc, &rc, hBrushBg);
        return 1;
    }

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
        wc.hbrBackground = hBrushBg;
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

    CreateWindowA("STATIC", "Name:", WS_CHILD | WS_VISIBLE, 10, 12, 40, 20, hNamePopup, NULL, hInst, NULL);
    hNameEdit = CreateWindowA(
        "EDIT", defaultName ? defaultName : "", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, 55, 10, 220, 22, hNamePopup, NULL, hInst, NULL
    );
    hNameOK = CreateWindowA(
        "BUTTON", "OK", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 70, 42, 70, 24, hNamePopup, (HMENU)1001, hInst, NULL
    );
    hNameCancel = CreateWindowA(
        "BUTTON", "Cancel", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 155, 42, 70, 24, hNamePopup, (HMENU)1002, hInst, NULL
    );

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

static void DrawDarkButton(LPDRAWITEMSTRUCT dis)
{
    HWND    btn  = dis->hwndItem;
    HDC     hdc  = dis->hDC;
    RECT    rc   = dis->rcItem;
    BOOL    down = (dis->itemState & ODS_SELECTED);
    BOOL    disabled = (dis->itemState & ODS_DISABLED);

    // Determine if this is the Launch button
    int     id   = (int)dis->CtlID;
    BOOL    isLaunch = (id == ID_BTN_LAUNCH);

    COLORREF bgColor;
    if (disabled) {
        bgColor = RGB(40, 40, 40);
    } else if (down) {
        bgColor = CLR_BTN_DOWN;
    } else if (isLaunch) {
        bgColor = CLR_ACCENT;
    } else {
        bgColor = CLR_BTN;
    }

    // Fill background
    HBRUSH hBr = CreateSolidBrush(bgColor);
    FillRect(hdc, &rc, hBr);
    DeleteObject(hBr);

    // Border
    HPEN hPen = CreatePen(PS_SOLID, 1, CLR_BORDER);
    HPEN hOld = (HPEN)SelectObject(hdc, hPen);
    SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
    SelectObject(hdc, hOld);
    DeleteObject(hPen);

    // Text
    char text[128];
    GetWindowTextA(btn, text, sizeof(text));
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, disabled ? RGB(100, 100, 100) : CLR_TEXT);
    DrawTextA(hdc, text, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
    case WM_COMMAND: {
        int id = LOWORD(wParam);

        if (id == ID_BTN_LAUNCH) {
            ReadCurrentFields();
            SaveSettings(currentSettings);
            LaunchGame(currentSettings);
            PostQuitMessage(0);
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

    case WM_CTLCOLORSTATIC: {
        HDC hdcStatic = (HDC)wParam;
        SetTextColor(hdcStatic, CLR_TEXT);
        SetBkColor(hdcStatic, CLR_BG);
        return (LRESULT)hBrushBg;
    }

    case WM_CTLCOLOREDIT: {
        HDC hdcEdit = (HDC)wParam;
        SetTextColor(hdcEdit, CLR_TEXT);
        SetBkColor(hdcEdit, CLR_BG_EDIT);
        return (LRESULT)hBrushEdit;
    }

    case WM_DRAWITEM: {
        LPDRAWITEMSTRUCT dis = (LPDRAWITEMSTRUCT)lParam;
        DrawDarkButton(dis);
        return TRUE;
    }

    case WM_ERASEBKGND: {
        HDC  hdc = (HDC)wParam;
        RECT rc;
        GetClientRect(hwnd, &rc);
        FillRect(hdc, &rc, hBrushBg);
        return 1;
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

    hBrushBg   = CreateSolidBrush(CLR_BG);
    hBrushEdit = CreateSolidBrush(CLR_BG_EDIT);

    WNDCLASSA wc    = {};
    wc.lpfnWndProc  = WndProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = "OpenMoHAALauncher";
    wc.hbrBackground = hBrushBg;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    RegisterClassA(&wc);

    int winW = 360;
    int winH = 360;

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

    int y      = 10;
    int labelW = 70;
    int editX  = 80;
    int editW  = 250;
    int editH  = 22;
    int gap    = 30;

    // IP field
    CreateWindowA("STATIC", "IP:", WS_CHILD | WS_VISIBLE, 10, y + 2, labelW, 20, hwnd, NULL, hInstance, NULL);
    hEditIP = CreateWindowA("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, editX, y, editW, editH, hwnd, (HMENU)ID_EDIT_IP, hInstance, NULL);
    y += gap;

    // Password field
    CreateWindowA("STATIC", "Password:", WS_CHILD | WS_VISIBLE, 10, y + 2, labelW, 20, hwnd, NULL, hInstance, NULL);
    hEditPass = CreateWindowA("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | ES_PASSWORD, editX, y, editW, editH, hwnd, (HMENU)ID_EDIT_PASS, hInstance, NULL);
    y += gap;

    // RCON field
    CreateWindowA("STATIC", "RCON:", WS_CHILD | WS_VISIBLE, 10, y + 2, labelW, 20, hwnd, NULL, hInstance, NULL);
    hEditRcon = CreateWindowA("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | ES_PASSWORD, editX, y, editW, editH, hwnd, (HMENU)ID_EDIT_RCON, hInstance, NULL);
    y += gap;

    // Game selection - radio buttons in a row
    CreateWindowA("STATIC", "Game:", WS_CHILD | WS_VISIBLE, 10, y + 2, 40, 20, hwnd, NULL, hInstance, NULL);
    int radioX = editX;
    int radioW = 55;
    hRadioAA = CreateWindowA("BUTTON", "AA", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP, radioX, y, radioW, 22, hwnd, (HMENU)ID_RADIO_AA, hInstance, NULL);
    radioX += radioW + 10;
    hRadioSH = CreateWindowA("BUTTON", "SH", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, radioX, y, radioW, 22, hwnd, (HMENU)ID_RADIO_SH, hInstance, NULL);
    radioX += radioW + 10;
    hRadioBT = CreateWindowA("BUTTON", "BT", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, radioX, y, radioW, 22, hwnd, (HMENU)ID_RADIO_BT, hInstance, NULL);
    y += gap + 5;

    // Launch button (owner-drawn for dark theme)
    CreateWindowA(
        "BUTTON",
        "Launch",
        WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
        (winW - 120) / 2,
        y,
        120,
        30,
        hwnd,
        (HMENU)ID_BTN_LAUNCH,
        hInstance,
        NULL
    );
    y += 45;

    // Separator line
    CreateWindowA("STATIC", "", WS_CHILD | WS_VISIBLE | SS_ETCHEDHORZ, 10, y, winW - 40, 2, hwnd, NULL, hInstance, NULL);
    y += 10;

    // Bookmarks section
    CreateWindowA("STATIC", "Bookmarks:", WS_CHILD | WS_VISIBLE, 10, y, 80, 20, hwnd, NULL, hInstance, NULL);
    y += 22;

    int bmBtnW = 180;
    int saveW  = 50;
    int delW   = 30;
    int bmGap  = 28;

    for (int i = 0; i < MAX_BOOKMARKS; i++) {
        hBtnBookmark[i] = CreateWindowA(
            "BUTTON",
            "(empty)",
            WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
            10,
            y,
            bmBtnW,
            24,
            hwnd,
            (HMENU)(UINT_PTR)(ID_BTN_BM_0 + i),
            hInstance,
            NULL
        );
        hBtnBmSave[i] = CreateWindowA(
            "BUTTON",
            "Save",
            WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
            10 + bmBtnW + 5,
            y,
            saveW,
            24,
            hwnd,
            (HMENU)(UINT_PTR)(ID_BTN_BMSAVE + i),
            hInstance,
            NULL
        );
        hBtnBmDel[i] = CreateWindowA(
            "BUTTON",
            "X",
            WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
            10 + bmBtnW + 5 + saveW + 5,
            y,
            delW,
            24,
            hwnd,
            (HMENU)(UINT_PTR)(ID_BTN_BMDEL + i),
            hInstance,
            NULL
        );
        y += bmGap;
    }

    // Load saved settings
    currentSettings = LoadSettings();
    SetWindowTextA(hEditIP, currentSettings.ip.c_str());
    SetWindowTextA(hEditPass, currentSettings.password.c_str());
    SetWindowTextA(hEditRcon, currentSettings.rconPassword.c_str());
    SetSelectedGame(currentSettings.gameType);

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

    DeleteObject(hBrushBg);
    DeleteObject(hBrushEdit);

    return (int)msg.wParam;
}

#endif // _WIN32
