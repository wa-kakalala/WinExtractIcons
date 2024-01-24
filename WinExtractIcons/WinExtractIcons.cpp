// WinExtractIcons.cpp : 定义应用程序的入口点。
#define _CRT_SECURE_NO_WARNINGS
#include "framework.h"
#include "WinExtractIcons.h"

#define ID_BTN_PREV   3301
#define ID_BTN_NEXT   3302
#define MAX_LOADSTRING 100

#define NORMAL_STATE   0
#define SELECT_STATE   1

// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 按钮控件
HWND btn_prev;
HWND btn_next;
HBITMAP prevBmp[2];
HBITMAP nextBmp[2];
HBITMAP bgBmp;

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
// 自定义函数
void DrawButton(LPARAM lParam);


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    AllocConsole();
    FILE * fp = freopen("conout$", "w", stdout);
    // TODO: 在此处放置代码。

    // 初始化全局字符串
    LoadString(hInstance, IDS_APP_TITLE, (LPTSTR)szTitle, MAX_LOADSTRING);
    LoadString(hInstance, IDC_WINEXTRACTICONS, (LPTSTR)szWindowClass, MAX_LOADSTRING);
    prevBmp[NORMAL_STATE] = LoadBitmap(hInstance,  MAKEINTRESOURCE(ID_PREV_NORMAL));
    prevBmp[SELECT_STATE] = LoadBitmap(hInstance,  MAKEINTRESOURCE(ID_PREV_SELECT));
    nextBmp[NORMAL_STATE] = LoadBitmap(hInstance,  MAKEINTRESOURCE(ID_NEXT_NORMAL));
    nextBmp[SELECT_STATE] = LoadBitmap(hInstance,  MAKEINTRESOURCE(ID_NEXT_SELECT));
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINEXTRACTICONS));

    MSG msg;

    // 主消息循环:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    DeleteObject(prevBmp[NORMAL_STATE]);
    DeleteObject(prevBmp[SELECT_STATE]);
    DeleteObject(nextBmp[NORMAL_STATE]);
    DeleteObject(nextBmp[SELECT_STATE]);
    return (int) msg.wParam;
}

//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINEXTRACTICONS));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINEXTRACTICONS);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(szWindowClass,szTitle, WS_OVERLAPPEDWINDOW,
       (GetSystemMetrics(SM_CXSCREEN)-640)/2, (GetSystemMetrics(SM_CYSCREEN)-480)/2, 640, 480, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    switch (message){
    case WM_CREATE: {
        btn_prev = CreateWindowA((LPSTR)("Button"), (LPSTR)("按钮一"), WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
            640/2 -100-20, 480-100, 32, 32, hWnd, (HMENU)ID_BTN_PREV, ((LPCREATESTRUCT)lParam)->hInstance, NULL );
        btn_next = CreateWindowA((LPSTR)"Button", (LPSTR)"按钮一", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
            640/2 + 100-29, 480 -100, 32, 32, hWnd, (HMENU)ID_BTN_NEXT, ((LPCREATESTRUCT)lParam)->hInstance, NULL );
        break;
    }
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_DRAWITEM: {
        PDRAWITEMSTRUCT pdr = (PDRAWITEMSTRUCT)(lParam);
        switch (pdr->CtlType) {
            case ODT_BUTTON :{
                DrawButton(lParam);
                break;
            }
        }
        break;;
    }
    case WM_PAINT:{
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 在此处添加使用 hdc 的任何绘图代码...

            EndPaint(hWnd, &ps);

    }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

/********************************Self Coding Begin**************************************/

void DrawButton(LPARAM lParam) {
    PDRAWITEMSTRUCT pdr = (PDRAWITEMSTRUCT)(lParam);
    HDC Bhdc = CreateCompatibleDC(pdr->hDC);
    HGDIOBJ oldBMP = NULL;
    switch (pdr->CtlID) {
        case ID_BTN_PREV: {
            if (pdr->itemState & ODS_SELECTED) {
                oldBMP = SelectObject(Bhdc, prevBmp[SELECT_STATE]);
            }else {
                oldBMP = SelectObject(Bhdc, prevBmp[NORMAL_STATE]);
            }
            StretchBlt(pdr->hDC, pdr->rcItem.left , pdr->rcItem.top , 32, 32, Bhdc, 0, 0, 200, 200, SRCCOPY);
            SelectObject(Bhdc, oldBMP);
            break;
        }
        case ID_BTN_NEXT:{
            if (pdr->itemState & ODS_SELECTED) {
                oldBMP = SelectObject(Bhdc, nextBmp[SELECT_STATE]);
            }else {
                oldBMP = SelectObject(Bhdc, nextBmp[NORMAL_STATE]);
            }
            StretchBlt(pdr->hDC, pdr->rcItem.left , pdr->rcItem.top , 32, 32, Bhdc, 0, 0, 200, 200, SRCCOPY);
            SelectObject(Bhdc, oldBMP);
            break;
        }
    }
    
    DeleteDC(Bhdc);
}

/********************************Self Coding  End **************************************/