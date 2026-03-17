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

// Dark theme colors
#    define CLR_BG          RGB(30, 30, 30)
#    define CLR_BG_EDIT     RGB(45, 45, 45)
#    define CLR_TEXT         RGB(220, 220, 220)
#    define CLR_TEXT_DIS     RGB(100, 100, 100)
#    define CLR_BORDER       RGB(70, 70, 70)
#    define CLR_BTN         RGB(55, 55, 55)
#    define CLR_BTN_HOVER   RGB(70, 70, 70)
#    define CLR_BTN_PRESSED RGB(40, 40, 40)

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
static LauncherSettings currentSettings;
static HFONT            hFont;
static HBRUSH           hBrushBg;
static HBRUSH           hBrushEdit;
static HWND             hHoverBtn;

static void DrawDarkButton(DRAWITEMSTRUCT *dis)
{
    HDC     hdc  = dis->hDC;
    RECT    rc   = dis->rcItem;
    BOOL    pressed  = (dis->itemState & ODS_SELECTED);
    BOOL    disabled = (dis->itemState & ODS_DISABLED);
    BOOL    focused  = (dis->itemState & ODS_FOCUS);
    BOOL    hover    = (dis->hwndItem == hHoverBtn) && !disabled;

    // Button face
    COLORREF bgColor = pressed ? CLR_BTN_PRESSED : (hover ? CLR_BTN_HOVER : CLR_BTN);
    HBRUSH   hBr     = CreateSolidBrush(bgColor);
    FillRect(hdc, &rc, hBr);
    DeleteObject(hBr);

    // Border
    HPEN hPen = CreatePen(PS_SOLID, 1, focused ? RGB(100, 100, 100) : CLR_BORDER);
    HPEN hOld = (HPEN)SelectObject(hdc, hPen);
    SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
    SelectObject(hdc, hOld);
    DeleteObject(hPen);

    // Text
    char text[128];
    GetWindowTextA(dis->hwndItem, text, sizeof(text));
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, disabled ? CLR_TEXT_DIS : CLR_TEXT);
    DrawTextA(hdc, text, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

static void DrawDarkGroupBox(DRAWITEMSTRUCT *dis)
{
    HDC  hdc = dis->hDC;
    RECT rc  = dis->rcItem;

    char text[64];
    GetWindowTextA(dis->hwndItem, text, sizeof(text));

    // Measure text
    SIZE textSize;
    GetTextExtentPoint32A(hdc, text, (int)strlen(text), &textSize);

    // Fill background
    FillRect(hdc, &rc, hBrushBg);

    // Draw frame (inset from top by half text height)
    int textTop = rc.top;
    RECT frameRc = {rc.left, textTop + textSize.cy / 2, rc.right, rc.bottom};
    HPEN hPen = CreatePen(PS_SOLID, 1, CLR_BORDER);
    HPEN hOld = (HPEN)SelectObject(hdc, hPen);
    SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Rectangle(hdc, frameRc.left, frameRc.top, frameRc.right, frameRc.bottom);
    SelectObject(hdc, hOld);
    DeleteObject(hPen);

    // Draw text label with gap in frame
    int textX = rc.left + 8;
    RECT textBg = {textX - 2, textTop, textX + textSize.cx + 2, textTop + textSize.cy};
    FillRect(hdc, &textBg, hBrushBg);
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, CLR_TEXT);
    TextOutA(hdc, textX, textTop, text, (int)strlen(text));
}

static void DrawDarkRadioButton(DRAWITEMSTRUCT *dis)
{
    HDC  hdc = dis->hDC;
    RECT rc  = dis->rcItem;
    BOOL checked = (dis->itemState & ODS_SELECTED)
                || (SendMessageA(dis->hwndItem, BM_GETCHECK, 0, 0) == BST_CHECKED);

    FillRect(hdc, &rc, hBrushBg);

    // Draw radio circle
    int circleSize = 12;
    int circleY    = rc.top + (rc.bottom - rc.top - circleSize) / 2;
    HPEN hPen      = CreatePen(PS_SOLID, 1, CLR_BORDER);
    HPEN hOld      = (HPEN)SelectObject(hdc, hPen);
    HBRUSH hBrFill = CreateSolidBrush(CLR_BG_EDIT);
    SelectObject(hdc, hBrFill);
    Ellipse(hdc, rc.left, circleY, rc.left + circleSize, circleY + circleSize);
    SelectObject(hdc, hOld);
    DeleteObject(hPen);
    DeleteObject(hBrFill);

    // Inner filled dot if checked
    if (checked) {
        int dotInset  = 3;
        HBRUSH hDot   = CreateSolidBrush(RGB(100, 180, 255));
        SelectObject(hdc, GetStockObject(NULL_PEN));
        SelectObject(hdc, hDot);
        Ellipse(
            hdc,
            rc.left + dotInset,
            circleY + dotInset,
            rc.left + circleSize - dotInset,
            circleY + circleSize - dotInset
        );
        DeleteObject(hDot);
    }

    // Text
    RECT textRc = {rc.left + circleSize + 4, rc.top, rc.right, rc.bottom};
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, CLR_TEXT);
    char text[64];
    GetWindowTextA(dis->hwndItem, text, sizeof(text));
    DrawTextA(hdc, text, -1, &textRc, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
}

static LRESULT CALLBACK DarkBtnSubclassProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
    (void)uIdSubclass;
    (void)dwRefData;

    switch (msg) {
    case WM_MOUSEMOVE: {
        if (hHoverBtn != hwnd) {
            hHoverBtn = hwnd;
            InvalidateRect(hwnd, NULL, FALSE);
            TRACKMOUSEEVENT tme = {};
            tme.cbSize          = sizeof(tme);
            tme.dwFlags         = TME_LEAVE;
            tme.hwndTrack       = hwnd;
            TrackMouseEvent(&tme);
        }
        break;
    }
    case WM_MOUSELEAVE:
        if (hHoverBtn == hwnd) {
            hHoverBtn = NULL;
            InvalidateRect(hwnd, NULL, FALSE);
        }
        break;
    case WM_NCDESTROY:
        RemoveWindowSubclass(hwnd, DarkBtnSubclassProc, uIdSubclass);
        break;
    }
    return DefSubclassProc(hwnd, msg, wParam, lParam);
}

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
    // Nickname is NOT changed by loading a bookmark - it's persistent
}

static void AutoSaveBookmark()
{
    // If the current IP matches an existing bookmark, update it
    for (int i = 0; i < MAX_BOOKMARKS; i++) {
        if (!currentSettings.bookmarks[i].name.empty() && currentSettings.bookmarks[i].ip == currentSettings.ip) {
            currentSettings.bookmarks[i].password     = currentSettings.password;
            currentSettings.bookmarks[i].rconPassword = currentSettings.rconPassword;
            UpdateBookmarkButton(i);
            break;
        }
    }
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
    case WM_CTLCOLORSTATIC:
    case WM_CTLCOLOREDIT: {
        HDC hdc = (HDC)wParam;
        SetTextColor(hdc, CLR_TEXT);
        SetBkColor(hdc, CLR_BG_EDIT);
        return (LRESULT)hBrushEdit;
    }

    case WM_CTLCOLORDLG:
        return (LRESULT)hBrushBg;

    case WM_DRAWITEM: {
        DRAWITEMSTRUCT *dis = (DRAWITEMSTRUCT *)lParam;
        if (dis->CtlType == ODT_BUTTON) {
            DrawDarkButton(dis);
            return TRUE;
        }
        break;
    }

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

    HWND hLabel = CreateWindowA("STATIC", "Name:", WS_CHILD | WS_VISIBLE, 10, 12, 40, 20, hNamePopup, NULL, hInst, NULL);
    hNameEdit = CreateWindowA(
        "EDIT", defaultName ? defaultName : "", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, 55, 10, 220, 22, hNamePopup, NULL, hInst, NULL
    );
    hNameOK = CreateWindowA(
        "BUTTON", "OK", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 70, 42, 70, 24, hNamePopup, (HMENU)1001, hInst, NULL
    );
    hNameCancel = CreateWindowA(
        "BUTTON", "Cancel", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 155, 42, 70, 24, hNamePopup, (HMENU)1002, hInst, NULL
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
        DRAWITEMSTRUCT *dis = (DRAWITEMSTRUCT *)lParam;
        if (dis->CtlType == ODT_BUTTON) {
            if (dis->CtlID == 0) {
                DrawDarkGroupBox(dis);
            } else if (dis->CtlID >= ID_RADIO_AA && dis->CtlID <= ID_RADIO_BT) {
                DrawDarkRadioButton(dis);
            } else {
                DrawDarkButton(dis);
            }
            return TRUE;
        }
        break;
    }

    case WM_COMMAND: {
        int id   = LOWORD(wParam);

        // Manual radio button handling (owner-drawn replaces auto behavior)
        if (id >= ID_RADIO_AA && id <= ID_RADIO_BT) {
            SendMessageA(hRadioAA, BM_SETCHECK, id == ID_RADIO_AA ? BST_CHECKED : BST_UNCHECKED, 0);
            SendMessageA(hRadioSH, BM_SETCHECK, id == ID_RADIO_SH ? BST_CHECKED : BST_UNCHECKED, 0);
            SendMessageA(hRadioBT, BM_SETCHECK, id == ID_RADIO_BT ? BST_CHECKED : BST_UNCHECKED, 0);
            InvalidateRect(hRadioAA, NULL, TRUE);
            InvalidateRect(hRadioSH, NULL, TRUE);
            InvalidateRect(hRadioBT, NULL, TRUE);
            return 0;
        }

        if (id == ID_BTN_CONNECT) {
            ReadCurrentFields();
            AutoSaveBookmark();
            SaveSettings(currentSettings);
            LaunchGame(currentSettings);
            // Close the launcher after spawning the game
            DestroyWindow(hwnd);
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

    // Create dark theme brushes
    hBrushBg   = CreateSolidBrush(CLR_BG);
    hBrushEdit = CreateSolidBrush(CLR_BG_EDIT);

    // Use the system default GUI font
    NONCLIENTMETRICSA ncm = {};
    ncm.cbSize            = sizeof(ncm);
    SystemParametersInfoA(SPI_GETNONCLIENTMETRICS, sizeof(ncm), &ncm, 0);
    hFont = CreateFontIndirectA(&ncm.lfMessageFont);

    WNDCLASSA wc    = {};
    wc.lpfnWndProc  = WndProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = "OpenMoHAALauncher";
    wc.hbrBackground = hBrushBg;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(1));
    RegisterClassA(&wc);

    int winW = 390;
    int winH = 390;

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

    // ---- Nickname (persistent, separate from server settings) ----
    int y = 10;
    CreateWindowA("STATIC", "Nickname:", WS_CHILD | WS_VISIBLE, innerX, y + 2, labelW, 20, hwnd, NULL, hInstance, NULL);
    hEditNickname = CreateWindowA(
        "EDIT",
        "",
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
        editX,
        y,
        editW,
        editH,
        hwnd,
        (HMENU)ID_EDIT_NICKNAME,
        hInstance,
        NULL
    );

    // ---- Server Settings group ----
    int grpY = y + editH + 10;
    int grpH = 105;
    CreateWindowA(
        "BUTTON",
        "Server Settings",
        WS_CHILD | WS_VISIBLE | BS_GROUPBOX | BS_OWNERDRAW,
        margin,
        grpY,
        winW - 2 * margin - 20,
        grpH,
        hwnd,
        NULL,
        hInstance,
        NULL
    );

    y = grpY + 20;

    CreateWindowA("STATIC", "IP:", WS_CHILD | WS_VISIBLE, innerX, y + 2, labelW, 20, hwnd, NULL, hInstance, NULL);
    hEditIP = CreateWindowA(
        "EDIT",
        "",
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
        editX,
        y,
        editW,
        editH,
        hwnd,
        (HMENU)ID_EDIT_IP,
        hInstance,
        NULL
    );
    y += 28;

    CreateWindowA(
        "STATIC", "Password:", WS_CHILD | WS_VISIBLE, innerX, y + 2, labelW, 20, hwnd, NULL, hInstance, NULL
    );
    hEditPass = CreateWindowA(
        "EDIT",
        "",
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | ES_PASSWORD,
        editX,
        y,
        editW,
        editH,
        hwnd,
        (HMENU)ID_EDIT_PASS,
        hInstance,
        NULL
    );
    y += 28;

    CreateWindowA("STATIC", "RCON:", WS_CHILD | WS_VISIBLE, innerX, y + 2, labelW, 20, hwnd, NULL, hInstance, NULL);
    hEditRcon = CreateWindowA(
        "EDIT",
        "",
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | ES_PASSWORD,
        editX,
        y,
        editW,
        editH,
        hwnd,
        (HMENU)ID_EDIT_RCON,
        hInstance,
        NULL
    );

    // ---- Game Selection group ----
    int grp2Y = grpY + grpH + 8;
    int grp2H = 50;
    CreateWindowA(
        "BUTTON",
        "Game",
        WS_CHILD | WS_VISIBLE | BS_GROUPBOX | BS_OWNERDRAW,
        margin,
        grp2Y,
        winW - 2 * margin - 20,
        grp2H,
        hwnd,
        NULL,
        hInstance,
        NULL
    );

    int radioY = grp2Y + 20;
    int radioW = 100;
    hRadioAA   = CreateWindowA(
        "BUTTON",
        "Allied Assault",
        WS_CHILD | WS_VISIBLE | BS_OWNERDRAW | WS_GROUP,
        innerX,
        radioY,
        radioW,
        20,
        hwnd,
        (HMENU)ID_RADIO_AA,
        hInstance,
        NULL
    );
    hRadioSH = CreateWindowA(
        "BUTTON",
        "Spearhead",
        WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
        innerX + radioW + 5,
        radioY,
        radioW,
        20,
        hwnd,
        (HMENU)ID_RADIO_SH,
        hInstance,
        NULL
    );
    hRadioBT = CreateWindowA(
        "BUTTON",
        "Breakthrough",
        WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
        innerX + 2 * (radioW + 5),
        radioY,
        radioW,
        20,
        hwnd,
        (HMENU)ID_RADIO_BT,
        hInstance,
        NULL
    );

    // ---- Bookmarks group ----
    int grp3Y  = grp2Y + grp2H + 8;
    int bmRows = MAX_BOOKMARKS;
    int grp3H  = 22 + bmRows * 28 + 5;
    CreateWindowA(
        "BUTTON",
        "Bookmarks",
        WS_CHILD | WS_VISIBLE | BS_GROUPBOX | BS_OWNERDRAW,
        margin,
        grp3Y,
        winW - 2 * margin - 20,
        grp3H,
        hwnd,
        NULL,
        hInstance,
        NULL
    );

    int bmY    = grp3Y + 20;
    int bmBtnW = innerW - 90;
    int saveW  = 45;
    int delW   = 30;

    for (int i = 0; i < MAX_BOOKMARKS; i++) {
        hBtnBookmark[i] = CreateWindowA(
            "BUTTON",
            "(empty)",
            WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
            innerX,
            bmY,
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
            innerX + bmBtnW + 5,
            bmY,
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
            innerX + bmBtnW + 5 + saveW + 5,
            bmY,
            delW,
            24,
            hwnd,
            (HMENU)(UINT_PTR)(ID_BTN_BMDEL + i),
            hInstance,
            NULL
        );
        bmY += 28;
    }

    // ---- Connect button at the bottom ----
    int connectY = grp3Y + grp3H + 10;
    int connectW = winW - 2 * margin - 20;
    CreateWindowA(
        "BUTTON",
        "Connect",
        WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
        margin,
        connectY,
        connectW,
        30,
        hwnd,
        (HMENU)ID_BTN_CONNECT,
        hInstance,
        NULL
    );

    // Subclass push buttons for hover tracking
    for (int i = 0; i < MAX_BOOKMARKS; i++) {
        SetWindowSubclass(hBtnBookmark[i], DarkBtnSubclassProc, 0, 0);
        SetWindowSubclass(hBtnBmSave[i], DarkBtnSubclassProc, 0, 0);
        SetWindowSubclass(hBtnBmDel[i], DarkBtnSubclassProc, 0, 0);
    }
    SetWindowSubclass(GetDlgItem(hwnd, ID_BTN_CONNECT), DarkBtnSubclassProc, 0, 0);

    // Apply font to all children
    SetFontOnChildren(hwnd, hFont);

    // Load saved settings
    currentSettings = LoadSettings();
    SetWindowTextA(hEditIP, currentSettings.ip.c_str());
    SetWindowTextA(hEditPass, currentSettings.password.c_str());
    SetWindowTextA(hEditRcon, currentSettings.rconPassword.c_str());
    SetWindowTextA(hEditNickname, currentSettings.nickname.c_str());
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

    if (hFont) {
        DeleteObject(hFont);
    }
    if (hBrushBg) {
        DeleteObject(hBrushBg);
    }
    if (hBrushEdit) {
        DeleteObject(hBrushEdit);
    }

    return (int)msg.wParam;
}

#endif // _WIN32
