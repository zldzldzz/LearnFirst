#include <windows.h>
#include <TCHAR.H>
#include <stdio.h>
#include "Resource.h"
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg,
    WPARAM wParam, LPARAM lParam);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpszCmdLine, int nCmdShow)
{
    HWND     hwnd;
    MSG     msg;
    WNDCLASS WndClass;
    HACCEL   hAcc;
    hAcc = LoadAccelerators(hInstance,
        MAKEINTRESOURCE(IDR_ACCELERATOR_1));
    WndClass.style = CS_HREDRAW | CS_VREDRAW;
    WndClass.lpfnWndProc = WndProc;
    WndClass.cbClsExtra = 0;
    WndClass.cbWndExtra = 0;
    WndClass.hInstance = hInstance;
    WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU4_1);
    WndClass.lpszClassName = _T("Window Class Name");
    RegisterClass(&WndClass);
    hwnd = CreateWindow(_T("Window Class Name"),
        _T("Window Title Name"),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        NULL,
        NULL,
        hInstance,
        NULL
    );
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
    while (GetMessage(&msg, NULL, 0, 0))
    {
        if (!TranslateAccelerator(hwnd, hAcc, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    return (int)msg.wParam;
}

void OutFromFile(TCHAR filename[], HWND hwnd)
{
    FILE* fPtr;
    HDC hdc;
    int line;
    TCHAR buffer[500];
    line = 0;
    hdc = GetDC(hwnd);
#ifdef _UNICODE
    _tfopen_s(&fPtr, filename, _T("r, ccs = UNICODE"));
#else
    _tfopen_s(&fPtr, filename, _T("r"));
#endif
    while (_fgetts(buffer, 100, fPtr) != NULL)
    {
        if (buffer[_tcslen(buffer) - 1] == _T('\n'))
            buffer[_tcslen(buffer) - 1] = NULL;
        TextOut(hdc, 0, line * 20, buffer, _tcslen(buffer));
        line++;
    }
    fclose(fPtr);
    ReleaseDC(hwnd, hdc);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg,
    WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    CHOOSEFONT FONT;
    static COLORREF   fColor;
    HFONT  hFont, OldFont;
    static LOGFONT    LogFont;

    CHOOSECOLOR COLOR;
static COLORREF tmp[16], color;
HBRUSH hBrush, OldBrush;
int i;


    int answer;
    OPENFILENAME OFN;
    OPENFILENAME SFN;

    TCHAR str[100], lpstrFile[100] = _T("");
    TCHAR filter[] = _T("Every File(*.*) \0*.*\0")
        _T("Text File\0*.txt;*.doc\0");;
    switch (iMsg)
    {
    case WM_CREATE:
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case ID_FONTDLG:
            memset(&FONT, 0, sizeof(CHOOSEFONT));
            FONT.lStructSize = sizeof(CHOOSEFONT);
            FONT.hwndOwner = hwnd;
            FONT.lpLogFont = &LogFont;
            FONT.Flags = CF_EFFECTS | CF_SCREENFONTS;
            if (ChooseFont(&FONT) != 0) {
                fColor = FONT.rgbColors;
                InvalidateRgn(hwnd, NULL, TRUE);
            }


        case ID_FILENEW:
            MessageBox(hwnd,
                _T("새 파일을 열겠습니까?"),
                _T("새 파일 선택"),
                MB_OKCANCEL);
            break;
        case ID_FILEOPEN:
            memset(&OFN, 0, sizeof(OPENFILENAME));
            OFN.lStructSize = sizeof(OPENFILENAME);
            OFN.hwndOwner = hwnd;
            OFN.lpstrFilter = filter;
            OFN.lpstrFile = lpstrFile;
            OFN.nMaxFile = 256;
            OFN.lpstrInitialDir = _T(".");
            if (GetOpenFileName(&OFN) != 0)
                OutFromFile(OFN.lpstrFile, hwnd);
            break;
       
        case ID_FILESAVE:
            memset(&SFN, 0, sizeof(OPENFILENAME));
            SFN.lStructSize = sizeof(OPENFILENAME);
            SFN.hwndOwner = hwnd;
            SFN.lpstrFilter = filter;
            SFN.lpstrFile = lpstrFile;
            SFN.nMaxFile = 256;
            SFN.lpstrInitialDir = _T(".");
            if (GetSaveFileName(&SFN) != 0)
            {
                _stprintf_s(str, _T("%s 파일로 저장하겠습니까?"), SFN.lpstrFile);
                MessageBox(hwnd, str, _T("저장하기 선택"), MB_OK);
                //저장 누르면 저장 될 수 있도록 실제 구현해오기
            }
            break;
        
        case ID_EDITUNDO:
            answer = MessageBox(hwnd,
                _T("파일을 저장하고 끝내겠습니까?"),
                _T("끝내기 선택"),
                MB_YESNOCANCEL);
            if (answer == IDYES || answer == IDNO)
                PostQuitMessage(0);
            break;
        case ID_EDITCOPY:
            answer = MessageBox(hwnd,
                _T("파일을 저장하고 끝내겠습니까?"),
                _T("끝내기 선택"),
                MB_YESNOCANCEL);
            if (answer == IDYES || answer == IDNO)
                PostQuitMessage(0);
            break;
        case ID_EDITPASTE:
            answer = MessageBox(hwnd,
                _T("파일을 저장하고 끝내겠습니까?"),
                _T("끝내기 선택"),
                MB_YESNOCANCEL);
            if (answer == IDYES || answer == IDNO)
                PostQuitMessage(0);
            break;
        case ID_INFORM:
            answer = MessageBox(hwnd,
                _T("파일을 저장하고 끝내겠습니까?"),
                _T("끝내기 선택"),
                MB_YESNOCANCEL);
            if (answer == IDYES || answer == IDNO)
                PostQuitMessage(0);
            break;
        case ID_EXIT:
            answer = MessageBox(hwnd,
                _T("파일을 저장하고 끝내겠습니까?"),
                _T("끝내기 선택"),
                MB_YESNOCANCEL);
            if (answer == IDYES || answer == IDNO)
                PostQuitMessage(0);
            break;
        case ID_COLORDLG:
            for (i = 0; i < 16; i++)
                tmp[i] = RGB(rand() % 256, rand() % 256, rand() % 256);
            memset(&COLOR, 0, sizeof(CHOOSECOLOR));
            COLOR.lStructSize = sizeof(CHOOSECOLOR);
            COLOR.hwndOwner = hwnd;
            COLOR.lpCustColors = tmp;
            COLOR.Flags = CC_FULLOPEN;
            if (ChooseColor(&COLOR) != 0) {
                color = COLOR.rgbResult;
                InvalidateRgn(hwnd, NULL, TRUE);
            }
            break;


        }
        break;

    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        hFont = CreateFontIndirect(&LogFont);
        OldFont = (HFONT)SelectObject(hdc, hFont);
        SetTextColor(hdc, fColor);
        TextOut(hdc, 10, 10, _T("HelloWorld"), 10);
        SelectObject(hdc, OldFont);
        DeleteObject(hFont);
        EndPaint(hwnd, &ps);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hwnd, iMsg, wParam, lParam);
}