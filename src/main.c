#define UNICODE
#define _UNICODE
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shellapi.h>
#include <dwmapi.h>
#include <wchar.h>
#include <stdio.h>
#include "account_id.h"

#define APP_VERSION L"1.0.1"
#define ID_OPEN 101
#define ID_GUIDE 102
#define ID_CONVERT 103
#define ID_COPY 104
#define ID_ABOUT 105
#define MAX_APP_PATH 32768
static HINSTANCE instance;
static HWND input, output, status_label;
static HFONT normal_font, title_font, heading_font, small_font, mono_font;
static HBRUSH background_brush, input_brush, card_brush;
static HICON large_icon;
static HANDLE engine_setup_process;
static int dpi = 96;
static wchar_t root_dir[MAX_APP_PATH], engine_path[MAX_APP_PATH];
static int px(int n) { return MulDiv(n,dpi,96); }
static void status(const wchar_t *s) { SetWindowTextW(status_label,s); }
static int file_exists(const wchar_t *path) {
    DWORD a = GetFileAttributesW(path); return a != INVALID_FILE_ATTRIBUTES && !(a & FILE_ATTRIBUTE_DIRECTORY);
}
static int join_path(wchar_t *out, const wchar_t *tail) {
    if (wcslen(root_dir)+wcslen(tail)+2 >= MAX_APP_PATH) return 0;
    wcscpy(out,root_dir); wcscat(out,L"\\"); wcscat(out,tail); return 1;
}
static int locate_engine(void) {
    const wchar_t *paths[] = {L"engine\\chiaki-ng-Win\\chiaki.exe",L"engine\\chiaki.exe",L"engine\\chiaki-ng.exe"};
    for (int i=0;i<3;i++) if (join_path(engine_path,paths[i]) && file_exists(engine_path)) return 1;
    engine_path[0] = 0; return 0;
}
static void install_engine(HWND window) {
    wchar_t script[MAX_APP_PATH],shell[MAX_APP_PATH],parameters[MAX_APP_PATH+128];
    if (!join_path(script,L"scripts\\Fetch-Engine.ps1") || !file_exists(script)) {
        status(L"Engine setup script missing. Extract the complete release ZIP.");return;
    }
    if (MessageBoxW(window,L"Download the official chiaki-ng v1.10.0 streaming engine now?\n\nThis is a one-time download of about 97 MB. The setup script verifies the official SHA-256 before extracting it. A PowerShell progress window will open.\n\nThe engine stays in this folder. No administrator access is requested.",L"Set up streaming engine",MB_YESNO|MB_ICONINFORMATION)!=IDYES) return;
    UINT length=GetSystemDirectoryW(shell,MAX_APP_PATH);
    const wchar_t *suffix=L"\\WindowsPowerShell\\v1.0\\powershell.exe";
    if (!length || length+wcslen(suffix)>=MAX_APP_PATH) return;
    wcscat(shell,suffix);
    _snwprintf(parameters,MAX_APP_PATH+128,L"-NoLogo -NoProfile -ExecutionPolicy Bypass -File \"%ls\"",script);
    SHELLEXECUTEINFOW info={0};info.cbSize=sizeof(info);info.fMask=SEE_MASK_NOCLOSEPROCESS;
    info.hwnd=window;info.lpVerb=L"open";info.lpFile=shell;info.lpParameters=parameters;info.lpDirectory=root_dir;info.nShow=SW_SHOWNORMAL;
    if (ShellExecuteExW(&info) && info.hProcess) {
        engine_setup_process=info.hProcess;EnableWindow(GetDlgItem(window,ID_OPEN),FALSE);
        status(L"Downloading verified engine. Keep the setup window open.");SetTimer(window,1,500,NULL);
    } else status(L"Could not start engine setup. See the manual setup instructions.");
}
static void open_client(HWND window) {
    if (!locate_engine()) {
        install_engine(window);
        return;
    }
    wchar_t working[MAX_APP_PATH]; wcscpy(working,engine_path);
    wchar_t *slash = wcsrchr(working,L'\\'); if (slash) *slash = 0;
    SHELLEXECUTEINFOW info = {0}; info.cbSize=sizeof(info); info.fMask=SEE_MASK_NOCLOSEPROCESS;
    info.hwnd=window; info.lpVerb=L"open"; info.lpFile=engine_path; info.lpDirectory=working; info.nShow=SW_SHOWNORMAL;
    if (!ShellExecuteExW(&info)) {
        wchar_t error[180]; wsprintfW(error,L"Could not open the client (Windows error %lu). Check the release was fully extracted.",GetLastError());
        status(error); MessageBoxW(window,error,L"Noctra Link",MB_OK|MB_ICONERROR);
    } else { if (info.hProcess) CloseHandle(info.hProcess); status(L"Client opened. Pair your console once, then select it to stream."); }
}
static void open_guide(HWND window) {
    wchar_t path[MAX_APP_PATH];
    if (!join_path(path,L"docs\\QUICKSTART.html") || !file_exists(path)) {status(L"Setup guide missing. Extract the whole release ZIP.");return;}
    if ((INT_PTR)ShellExecuteW(window,L"open",path,NULL,root_dir,SW_SHOWNORMAL)<=32)
        status(L"Could not open your browser. Open docs/QUICKSTART.html manually.");
}
static void convert_id(void) {
    wchar_t wide[64],result[32]; char hex[64],encoded[13];
    GetWindowTextW(input,wide,64);
    int len=WideCharToMultiByte(CP_UTF8,0,wide,-1,hex,sizeof(hex),NULL,NULL);
    if (!len || !noctra_account_id(hex,encoded)) {
        SetWindowTextW(output,L"");status(L"Enter exactly 16 hexadecimal digits (0-9, a-f), optionally prefixed 0x.");return;
    }
    MultiByteToWideChar(CP_UTF8,0,encoded,-1,result,32);
    SetWindowTextW(output,result);status(L"Converted locally. Paste the result into the client's PSN Account-ID field.");
}
static void copy_id(HWND window) {
    wchar_t value[32]; int length=GetWindowTextW(output,value,32);
    if (!length) {status(L"Convert an Account-ID first.");return;}
    if (!OpenClipboard(window)) {status(L"Clipboard busy. Try again.");return;}
    HGLOBAL memory=GlobalAlloc(GMEM_MOVEABLE,((SIZE_T)length+1)*sizeof(wchar_t));
    if (memory) {
        void *p=GlobalLock(memory);
        if (p) {CopyMemory(p,value,((SIZE_T)length+1)*sizeof(wchar_t));GlobalUnlock(memory);
            if (EmptyClipboard() && SetClipboardData(CF_UNICODETEXT,memory)) {memory=NULL;status(L"Account-ID copied to clipboard.");}}
        if (memory) {GlobalFree(memory);status(L"Could not copy to clipboard. Select and copy the value manually.");}
    } else status(L"Could not allocate clipboard memory.");
    CloseClipboard();
}
static HWND control(HWND parent,const wchar_t *type,const wchar_t *text,int id,int x,int y,int w,int h,DWORD style) {
    HWND item=CreateWindowExW(0,type,text,WS_CHILD|WS_VISIBLE|style,px(x),px(y),px(w),px(h),parent,(HMENU)(INT_PTR)id,instance,NULL);
    SendMessageW(item,WM_SETFONT,(WPARAM)normal_font,TRUE);return item;
}
static void label(HDC dc,const wchar_t *text,int x,int y,int w,int h,HFONT font,COLORREF color) {
    RECT r={px(x),px(y),px(x+w),px(y+h)};SelectObject(dc,font);SetTextColor(dc,color);SetBkMode(dc,TRANSPARENT);
    DrawTextW(dc,text,-1,&r,DT_LEFT|DT_WORDBREAK|DT_NOPREFIX);
}
static void panel(HDC dc,int x,int y,int w,int h) {
    HGDIOBJ old_brush=SelectObject(dc,card_brush),old_pen=SelectObject(dc,GetStockObject(NULL_PEN));
    RoundRect(dc,px(x),px(y),px(x+w),px(y+h),px(18),px(18));SelectObject(dc,old_brush);SelectObject(dc,old_pen);
}
static void paint(HWND window) {
    PAINTSTRUCT ps;HDC dc=BeginPaint(window,&ps);RECT rect;GetClientRect(window,&rect);FillRect(dc,&rect,background_brush);
    if (large_icon) DrawIconEx(dc,px(30),px(30),large_icon,px(72),px(72),0,NULL,DI_NORMAL);
    label(dc,L"NOCTRA LINK",120,31,540,42,title_font,RGB(240,244,255));
    label(dc,L"BY SOLITARY   /   LOCAL PS4 STREAMING",122,77,570,25,small_font,RGB(130,166,211));
    label(dc,L"Your PS4. On your PC.",30,132,700,40,heading_font,RGB(230,237,250));
    label(dc,L"Open the streaming client, pair once, and play over your local network.",31,176,690,44,normal_font,RGB(168,184,208));
    panel(dc,30,235,700,115);
    label(dc,L"POWERED BY CHIAKI-NG",49,251,550,22,small_font,RGB(137,161,200));
    label(dc,L"Picture, sound, console menus and games. Pairing is required.",49,279,650,28,normal_font,RGB(222,230,245));
    label(dc,L"Protected content may be blocked by the console.",49,310,650,23,small_font,RGB(144,161,186));
    panel(dc,30,374,700,183);
    label(dc,L"Offline Account-ID helper",49,390,600,31,heading_font,RGB(230,237,250));
    label(dc,L"Hex Account-ID (16 digits)",49,433,300,22,small_font,RGB(166,183,210));
    label(dc,L"Base64 for the client",394,433,300,22,small_font,RGB(166,183,210));
    label(dc,L"Conversion only. This does not activate or modify a console account.",49,523,650,22,small_font,RGB(144,161,186));
    label(dc,L"v" APP_VERSION L"  /  Windows x64",30,655,400,22,small_font,RGB(126,146,178));
    EndPaint(window,&ps);
}
static LRESULT CALLBACK window_proc(HWND window,UINT message,WPARAM wparam,LPARAM lparam) {
    switch (message) {
    case WM_CREATE: {
        BOOL dark=TRUE;DwmSetWindowAttribute(window,20,&dark,sizeof(dark));
        control(window,L"BUTTON",L"Open streaming client",ID_OPEN,30,577,425,46,BS_OWNERDRAW|WS_TABSTOP);
        control(window,L"BUTTON",L"Setup guide",ID_GUIDE,470,577,260,46,BS_OWNERDRAW|WS_TABSTOP);
        input=control(window,L"EDIT",L"",201,49,461,228,40,ES_AUTOHSCROLL|WS_TABSTOP|WS_BORDER);
        output=control(window,L"EDIT",L"",202,394,461,205,40,ES_AUTOHSCROLL|ES_READONLY|WS_TABSTOP|WS_BORDER);
        SendMessageW(input,EM_SETLIMITTEXT,18,0);SendMessageW(input,WM_SETFONT,(WPARAM)mono_font,TRUE);SendMessageW(output,WM_SETFONT,(WPARAM)mono_font,TRUE);
        control(window,L"BUTTON",L"Convert",ID_CONVERT,289,461,88,40,BS_OWNERDRAW|WS_TABSTOP);
        control(window,L"BUTTON",L"Copy",ID_COPY,611,461,99,40,BS_OWNERDRAW|WS_TABSTOP);
        control(window,L"BUTTON",L"About",ID_ABOUT,637,648,93,31,BS_OWNERDRAW|WS_TABSTOP);
        status_label=control(window,L"STATIC",L"",203,30,632,590,20,SS_LEFT);
        SendMessageW(status_label,WM_SETFONT,(WPARAM)small_font,TRUE);
        status(locate_engine()?L"Engine ready. Pair your console in chiaki-ng on first use.":L"First use: Open streaming client will offer the official engine download.");
        return 0;
    }
    case WM_COMMAND:
        if (HIWORD(wparam)==BN_CLICKED) switch(LOWORD(wparam)) {
            case ID_OPEN:open_client(window);break;case ID_GUIDE:open_guide(window);break;
            case ID_CONVERT:convert_id();break;case ID_COPY:copy_id(window);break;
            case ID_ABOUT:MessageBoxW(window,L"Noctra Link 1.0.1\nBy Solitary\n\nA Windows launcher and setup companion for chiaki-ng.\nThe unmodified chiaki-ng client provides registration, video, audio and controls.\n\nLauncher: MIT license.\nchiaki-ng: upstream AGPL license with its OpenSSL exception.\nSee THIRD_PARTY_NOTICES.md and licenses/ for details.\n\nIndependent project; not affiliated with Sony or chiaki-ng.",L"About Noctra Link",MB_OK|MB_ICONINFORMATION);break;
        }
        if (LOWORD(wparam)==201 && HIWORD(wparam)==EN_CHANGE) SetWindowTextW(output,L"");
        return 0;
    case WM_DRAWITEM: {
        DRAWITEMSTRUCT *draw=(DRAWITEMSTRUCT*)lparam;
        COLORREF color=draw->CtlID==ID_OPEN?RGB(66,82,216):RGB(38,53,79);
        if (draw->itemState&ODS_SELECTED) color=RGB(63,104,171);
        HBRUSH brush=CreateSolidBrush(color);FillRect(draw->hDC,&draw->rcItem,brush);DeleteObject(brush);
        wchar_t text[100];GetWindowTextW(draw->hwndItem,text,100);SetBkMode(draw->hDC,TRANSPARENT);SetTextColor(draw->hDC,RGB(241,245,255));SelectObject(draw->hDC,normal_font);
        DrawTextW(draw->hDC,text,-1,&draw->rcItem,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
        if (draw->itemState&ODS_FOCUS) {RECT focus=draw->rcItem;InflateRect(&focus,-px(4),-px(4));DrawFocusRect(draw->hDC,&focus);}return TRUE;
    }
    case WM_CTLCOLORSTATIC: {
        HDC dc=(HDC)wparam;SetTextColor(dc,RGB(147,173,209));SetBkColor(dc,(HWND)lparam==output?RGB(12,21,36):RGB(11,17,29));
        return (LRESULT)((HWND)lparam==output?input_brush:background_brush);
    }
    case WM_CTLCOLOREDIT: SetTextColor((HDC)wparam,RGB(232,241,255));SetBkColor((HDC)wparam,RGB(12,21,36));return (LRESULT)input_brush;
    case WM_ERASEBKGND:return 1;
    case WM_TIMER:
        if (wparam==1 && engine_setup_process && WaitForSingleObject(engine_setup_process,0)==WAIT_OBJECT_0) {
            DWORD code=1;GetExitCodeProcess(engine_setup_process,&code);CloseHandle(engine_setup_process);engine_setup_process=NULL;
            KillTimer(window,1);EnableWindow(GetDlgItem(window,ID_OPEN),TRUE);
            if (code==0 && locate_engine()) open_client(window);
            else status(L"Engine setup did not finish. Check your connection and try again.");
        }
        return 0;
    case WM_PAINT:paint(window);return 0;
    case WM_CLOSE:DestroyWindow(window);return 0;
    case WM_DESTROY:
        if (engine_setup_process) {CloseHandle(engine_setup_process);engine_setup_process=NULL;}
        PostQuitMessage(0);return 0;
    }
    return DefWindowProcW(window,message,wparam,lparam);
}
int WINAPI wWinMain(HINSTANCE current,HINSTANCE previous,LPWSTR command,int show) {
    (void)previous;(void)command;instance=current;SetProcessDPIAware();
    HDC screen=GetDC(NULL);dpi=GetDeviceCaps(screen,LOGPIXELSX);ReleaseDC(NULL,screen);
    DWORD length=GetModuleFileNameW(NULL,root_dir,MAX_APP_PATH);
    if (!length || length>=MAX_APP_PATH) return 1;
    wchar_t *slash=wcsrchr(root_dir,L'\\');if(!slash)return 1;*slash=0;
    background_brush=CreateSolidBrush(RGB(11,17,29));input_brush=CreateSolidBrush(RGB(12,21,36));card_brush=CreateSolidBrush(RGB(20,31,49));
    normal_font=CreateFontW(-px(15),0,0,0,FW_NORMAL,0,0,0,DEFAULT_CHARSET,0,0,CLEARTYPE_QUALITY,0,L"Segoe UI");
    title_font=CreateFontW(-px(31),0,0,0,FW_BOLD,0,0,0,DEFAULT_CHARSET,0,0,CLEARTYPE_QUALITY,0,L"Segoe UI");
    heading_font=CreateFontW(-px(23),0,0,0,FW_SEMIBOLD,0,0,0,DEFAULT_CHARSET,0,0,CLEARTYPE_QUALITY,0,L"Segoe UI");
    small_font=CreateFontW(-px(12),0,0,0,FW_NORMAL,0,0,0,DEFAULT_CHARSET,0,0,CLEARTYPE_QUALITY,0,L"Segoe UI");
    mono_font=CreateFontW(-px(17),0,0,0,FW_NORMAL,0,0,0,DEFAULT_CHARSET,0,0,CLEARTYPE_QUALITY,0,L"Consolas");
    large_icon=(HICON)LoadImageW(current,MAKEINTRESOURCEW(1),IMAGE_ICON,px(96),px(96),LR_DEFAULTCOLOR);
    WNDCLASSEXW type={0};type.cbSize=sizeof(type);type.lpfnWndProc=window_proc;type.hInstance=current;type.hCursor=LoadCursorW(NULL,IDC_ARROW);
    type.hIcon=LoadIconW(current,MAKEINTRESOURCEW(1));type.hIconSm=(HICON)LoadImageW(current,MAKEINTRESOURCEW(1),IMAGE_ICON,px(16),px(16),0);
    type.lpszClassName=L"NoctraLinkWindow";type.hbrBackground=background_brush;if(!RegisterClassExW(&type))return 1;
    RECT size={0,0,px(760),px(695)};DWORD style=WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX;AdjustWindowRect(&size,style,FALSE);
    HWND window=CreateWindowExW(0,type.lpszClassName,L"Noctra Link - By Solitary",style,CW_USEDEFAULT,CW_USEDEFAULT,size.right-size.left,size.bottom-size.top,NULL,NULL,current,NULL);
    if (!window) return 1;
    ShowWindow(window,show);UpdateWindow(window);
    MSG message;int result;
    while ((result=GetMessageW(&message,NULL,0,0))>0) {if (!IsDialogMessageW(window,&message)) {TranslateMessage(&message);DispatchMessageW(&message);}}
    DeleteObject(normal_font);DeleteObject(title_font);DeleteObject(heading_font);DeleteObject(small_font);DeleteObject(mono_font);
    DeleteObject(background_brush);DeleteObject(input_brush);DeleteObject(card_brush);if(large_icon)DestroyIcon(large_icon);
    return result<0?1:(int)message.wParam;
}
