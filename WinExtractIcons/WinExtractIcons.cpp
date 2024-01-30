// WinExtractIcons.cpp : 定义应用程序的入口点。
#define _CRT_SECURE_NO_WARNINGS
#include "framework.h"
#include "WinExtractIcons.h"
#include <vector>


#define ID_BTN_PREV    3301
#define ID_BTN_NEXT    3302
#define MAX_LOADSTRING 100

#define NORMAL_STATE   0
#define SELECT_STATE   1

// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 按钮控件
HWND btn_prev     ;
HWND btn_next     ;
HBITMAP prevBmp[2];
HBITMAP nextBmp[2];
HBITMAP bgBmp     ;

// 存储ICON资源
typedef struct VecIcon_s {
    std::vector<HICON> vecIcon;
    int icon_w;
    int icon_h;
    int disp_r;
    int disp_c;
    int disp_marign;
}VecIcon_s;

VecIcon_s vecIcon_s;

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
// 自定义函数
void DrawButton(LPARAM lParam);
void CmdOpenProc(HWND hWnd);
void DrawIcon(HDC hdc);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    // AllocConsole();
    // FILE * fp = freopen("conout$", "w", stdout);
    // TODO: 在此处放置代码。

    // 初始化全局字符串
    LoadString(hInstance, IDS_APP_TITLE, (LPTSTR)szTitle, MAX_LOADSTRING);
    LoadString(hInstance, IDC_WINEXTRACTICONS, (LPTSTR)szWindowClass, MAX_LOADSTRING);
    prevBmp[NORMAL_STATE] = LoadBitmap(hInstance,  MAKEINTRESOURCE(ID_PREV_NORMAL));
    prevBmp[SELECT_STATE] = LoadBitmap(hInstance,  MAKEINTRESOURCE(ID_PREV_SELECT));
    nextBmp[NORMAL_STATE] = LoadBitmap(hInstance,  MAKEINTRESOURCE(ID_NEXT_NORMAL));
    nextBmp[SELECT_STATE] = LoadBitmap(hInstance,  MAKEINTRESOURCE(ID_NEXT_SELECT));

    vecIcon_s.disp_marign = 5;
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

   HWND hWnd = CreateWindowW(szWindowClass,szTitle, WS_OVERLAPPEDWINDOW ,
       (GetSystemMetrics(SM_CXSCREEN)-640)/2, (GetSystemMetrics(SM_CYSCREEN)-480)/2, 640, 480, nullptr, nullptr, hInstance, nullptr);
   // ^ WS_THICKFRAME
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
            case IDM_OPEN: {
                CmdOpenProc(hWnd);
                break;
            }
            case IDM_SAVE:
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
            DrawIcon(hdc);
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

void CmdOpenProc(HWND hWnd) {
    TCHAR strFilename[MAX_PATH] = { 0 };//用于接收文件名
    OPENFILENAME ofn = { 0 };
    ofn.lStructSize = sizeof(OPENFILENAME);//结构体大小
    ofn.hwndOwner = hWnd;                  //拥有着窗口句柄，为NULL表示对话框是非模态的，实际应用中一般都要有这个句柄
    ofn.lpstrFilter = TEXT("所有文件\0*.*\0\0");//设置过滤
    ofn.nFilterIndex = 1;                                                  //过滤器索引
    ofn.lpstrFile = strFilename;            //接收返回的文件名，注意第一个字符需要为NULL
    ofn.nMaxFile = sizeof(strFilename);     //缓冲区长度
    ofn.lpstrInitialDir = "../resources/icons/";             //初始目录
    ofn.lpstrTitle = TEXT("请选择一个文件!");//使用系统默认标题留空即可
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;//文件、目录必须存在，隐藏只读选项
    if (!GetOpenFileName(&ofn)){
        MessageBox(NULL, TEXT("请选择一个文件!"), NULL, MB_ICONERROR);
        return;
    }
    DWORD errorCode = 0;
    HMODULE hIconLib = LoadLibrary(_T(strFilename));
    if (hIconLib == nullptr){
        MessageBox(NULL, TEXT("读取资源文件失败!"), NULL, MB_ICONERROR);
        return;
    }

    vecIcon_s.vecIcon.clear();
    vecIcon_s.disp_c = 0;
    vecIcon_s.disp_r = 0;
    vecIcon_s.icon_h = 0;
    vecIcon_s.icon_w = 0;

    int idx = 1 ;
    HICON hIcon ;
    ICONINFO iconInfo;
    BITMAP bm;
    BOOL fResult;
    int icon_h_temp;
    do {
        hIcon = (HICON)LoadImage(hIconLib, MAKEINTRESOURCE(idx++), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE);
        if (hIcon != nullptr){
            vecIcon_s.vecIcon.push_back(hIcon);
            fResult = GetIconInfo(hIcon, &iconInfo);
            fResult = GetObject(iconInfo.hbmMask, sizeof(bm), &bm) == sizeof(bm);
            vecIcon_s.icon_w = (vecIcon_s.icon_w < bm.bmWidth)? bm.bmWidth: vecIcon_s.icon_w;
            icon_h_temp = iconInfo.hbmColor ? bm.bmHeight : bm.bmHeight / 2;
            vecIcon_s.icon_h = (vecIcon_s.icon_h > icon_h_temp) ? vecIcon_s.icon_h : icon_h_temp;
        }else {
            break;
        }
    } while (true);
    if (vecIcon_s.vecIcon.size() == 0) return;
    
    vecIcon_s.disp_c = 620 / (vecIcon_s.icon_w + 2 * vecIcon_s.disp_marign);
    vecIcon_s.disp_r = 360 / (vecIcon_s.icon_h + 2 * vecIcon_s.disp_marign);
    
    // 窗口重绘
    // https://blog.csdn.net/woyaowenzi/article/details/4604904
    InvalidateRect(hWnd, NULL, true);
    UpdateWindow(hWnd); // 更新窗口
}

void CmdSaveProc(void) {
    
    

}

void DrawIcon(HDC hdc) {
    int show_idx = 0;
    for (int idx_r = 0; idx_r < vecIcon_s.disp_r; idx_r++) {
        for (int idx_c = 0; idx_c < vecIcon_s.disp_c; idx_c++) {
            show_idx = idx_r * vecIcon_s.disp_r + idx_c;
            if (show_idx >= vecIcon_s.vecIcon.size()) break;
            DrawIcon(hdc, idx_c * ( vecIcon_s.icon_w + vecIcon_s.disp_marign ), 
                idx_r * (vecIcon_s.icon_h + vecIcon_s.disp_marign), vecIcon_s.vecIcon[show_idx]);
        }
    }
}

/********************************Self Coding  End **************************************/