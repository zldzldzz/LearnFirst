#include <windows.h>
#include <TCHAR.H>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg,
    WPARAM wParam, LPARAM lParam);
BOOL CALLBACK Dlg6_4Proc(HWND hDlg, UINT iMsg,
    WPARAM wParam, LPARAM lParam);

HINSTANCE hInst;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpszCmdLine, int nCmdShow)
{
    HWND     hwnd;
    MSG     msg;
    WNDCLASS WndClass;
    hInst = hInstance;

    WndClass.style = CS_HREDRAW | CS_VREDRAW;
    WndClass.lpfnWndProc = WndProc;
    WndClass.cbClsExtra = 0;
    WndClass.cbWndExtra = 0;
    WndClass.hInstance = hInstance;
    WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    WndClass.lpszMenuName = NULL;  // 메뉴 제거
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
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg,
    WPARAM wParam, LPARAM lParam)
{
    switch (iMsg)
    {

    case WM_CREATE:
        // 창이 열리자마자 다이얼로그 띄우기 (예시)
        break;
    case WM_LBUTTONDOWN:  // ⬅ 클릭 이벤트 추가
        DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG6_7), hwnd, (DLGPROC)Dlg6_4Proc);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

BOOL CALLBACK Dlg6_4Proc(HWND hDlg, UINT iMsg,
    WPARAM wParam, LPARAM lParam)
{
    static HWND hList;
    static int selection;
    TCHAR name[20];

    switch (iMsg)
    {
    case WM_INITDIALOG:
        hList = GetDlgItem(hDlg, IDC_LIST_NAME);
        return 1;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDC_BUTTON_INSERT:
            GetDlgItemText(hDlg, IDC_EDIT_NAME, name, 20);
            if (_tcscmp(name, _T("")))
                SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)name);
            return 0;
        case IDC_BUTTON_DELETE:
            SendMessage(hList, LB_DELETESTRING, selection, 0);
            return 0;
        case IDC_LIST_NAME:
            if (HIWORD(wParam) == LBN_SELCHANGE)
                selection = (int)SendMessage(hList, LB_GETCURSEL, 0, 0);
            break;
        case IDCLOSE:
            EndDialog(hDlg, 0);
            return 0;
        case IDCANCEL:
            EndDialog(hDlg, 0);
            return 0;
        }
        break;
    }
    return 0;
}
