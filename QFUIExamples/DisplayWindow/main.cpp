#include <windows.h>

#include "QFWindow.h"
#include "QFUI.h"
#include "Gdiplus.h"

#pragma comment (lib,"Gdiplus.lib")

QFWindow g_qfw;
HMODULE m_hm =NULL;
// 窗口过程函数，处理窗口消息
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
        
    default:

        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

#ifdef _DEBUG



    AllocConsole();
    FILE* stream;
    freopen_s(&stream, "CON", "r", stdin);//重定向输入流
    freopen_s(&stream, "CON", "w", stdout);//重定向输入流
#endif 






    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    // 注册窗口类
    

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc; // 窗口过程函数
    wc.hInstance = hInstance;
    wc.lpszClassName = g_qfw.GetWindowsClass();

    RegisterClass(&wc);

    // 创建窗口
    HWND hwnd = CreateWindowEx(
        0,                              // 扩展风格
        wc.lpszClassName,                     // 窗口类名
        g_qfw.GetWindowsTitle(),                // 窗口标题
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,            // 窗口样式
        CW_USEDEFAULT, CW_USEDEFAULT,   // 初始位置
        CW_USEDEFAULT, CW_USEDEFAULT,   // 初始大小
        NULL,                           // 父窗口
        NULL,                           // 菜单
        hInstance,                      // 应用程序实例句柄
        NULL                            // 其他参数
    );


    if (hwnd == NULL) {
        return 0;
    }
    

    // 显示窗口
    ShowWindow(hwnd, nCmdShow);

    //typedef QWND* (*CQWDM)(HDC winddc, QRect  rf);
    //m_hm=LoadLibraryA("QWNDHandle.dll");
    //CQWDM cqwdm = (CQWDM)GetProcAddress(m_hm, "CreateQWMange");

   
    //HDC wdc = GetDC(hwnd);
    //RECT rect;
    //GetWindowRect(hwnd, &rect);
    //QWND* p = cqwdm(wdc, QRect(0, 0, rect.right - rect.left, rect.bottom - rect.top));
    //QWND* p = cqwdm(wdc, QRect(0, 0, 100, 100));

    //DRAW draw = (DRAW)GetProcAddress(m_hm, "Draw");

    QFUSE::BindPanelWindow(hwnd);
    QWMange* pbuff = (QWMange*)QFUSE::GetQWManger();
    //pbuff->SetBKBrush(NULL);
    pbuff->SetBKPicture(L"./picture.png");
    QTabs* qt = (QTabs*)QFUSE::CreateQWND("QFTabs", QRect(100, 10, 800, 600));
    QFUSE::AttachCtrl(qt);
    //给标签控件添加五个标签
    for (int i = -1; ++i < 5;) {
        qt->AddTab((L"测试"+std::to_wstring(i)).c_str(), L"Tabico.png");

    }


    //创建一个按钮
    QButton* qb = (QButton*)QFUSE::CreateQWND("QFButton", QRect(10, 10, 100, 50));
    //向标签控件关联一个控件，参数1：控件指针，参数2：标签索引
    qt->AttachCtrl(qb,0);

    //创建一个线性图表
    QLineChart* qlc= (QLineChart*)QFUSE::CreateQWND("QFLineChart", QRect(10, 10, 700, 550));
    //给线性图标增加一个函数图像
    std::vector<QPointF> srcPF;
    for (QREAL x = 0.0; x < 20; x += 3.14 / 8) {
        srcPF.push_back(QPointF(x, std::sin(x)));
    }
    qlc->AddCurve(srcPF.data(), srcPF.size());
    //向标签控件关联一个控件，参数1：控件指针，参数2：标签索引
    qt->AttachCtrl(qlc, 1);
   
    //创建一个报表控件
    QReportForm* wnd = (QReportForm*)QFUSE::CreateQWND("QFReportForm", QRect(10, 10, 700, 550));
    wnd->InsertTitle(L"唱", 100, -1);
    wnd->InsertTitle(L"跳", 100, -1);
    wnd->InsertTitle(L"rap", 200, -1);
    wnd->InsertRecorder(L"半年");
    for (int i = 0; ++i < 21;) {
        std::wstring wstr(L"年半");
        wstr = std::to_wstring(i) + wstr;
        wnd->InsertRecorder(wstr.c_str(), -1);
    }
    for (int i = -1; ++i < 20;) {
        wnd->SetRecorderText(i, 1, L"练习生");
        wnd->SetRecorderText(i, 2, L"精通");
    }
    wnd->IsIsDrag(true);    //是否可以拖动
    qt->AttachCtrl(wnd, 2);

    qt->SetFontSize(18);//设置标签字体大小
    
    QEdit* qed = (QEdit*)QFUSE::CreateQWND("QFEdit", QRect(10, 10, 400, 50));

    qt->AttachCtrl(qed, 3);


   

    // 主消息循环
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        //draw(p, wdc);
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    QFUSE::UnBindPanelWindow();
    Gdiplus::GdiplusShutdown(gdiplusToken);
#ifdef _DEBUG
    FreeConsole();
#endif
    //FreeLibrary(m_hm);
    return 0;
}
