#include <windows.h>

#include "QFWindow.h"
#include "QFUI.h"
#include "Gdiplus.h"

#pragma comment (lib,"Gdiplus.lib")

QFWindow g_qfw;
HMODULE m_hm =NULL;
// ���ڹ��̺�������������Ϣ
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
    freopen_s(&stream, "CON", "r", stdin);//�ض���������
    freopen_s(&stream, "CON", "w", stdout);//�ض���������
#endif 






    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    // ע�ᴰ����
    

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc; // ���ڹ��̺���
    wc.hInstance = hInstance;
    wc.lpszClassName = g_qfw.GetWindowsClass();

    RegisterClass(&wc);

    // ��������
    HWND hwnd = CreateWindowEx(
        0,                              // ��չ���
        wc.lpszClassName,                     // ��������
        g_qfw.GetWindowsTitle(),                // ���ڱ���
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,            // ������ʽ
        CW_USEDEFAULT, CW_USEDEFAULT,   // ��ʼλ��
        CW_USEDEFAULT, CW_USEDEFAULT,   // ��ʼ��С
        NULL,                           // ������
        NULL,                           // �˵�
        hInstance,                      // Ӧ�ó���ʵ�����
        NULL                            // ��������
    );


    if (hwnd == NULL) {
        return 0;
    }
    

    // ��ʾ����
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
    //����ǩ�ؼ���������ǩ
    for (int i = -1; ++i < 5;) {
        qt->AddTab((L"����"+std::to_wstring(i)).c_str(), L"Tabico.png");

    }


    //����һ����ť
    QButton* qb = (QButton*)QFUSE::CreateQWND("QFButton", QRect(10, 10, 100, 50));
    //���ǩ�ؼ�����һ���ؼ�������1���ؼ�ָ�룬����2����ǩ����
    qt->AttachCtrl(qb,0);

    //����һ������ͼ��
    QLineChart* qlc= (QLineChart*)QFUSE::CreateQWND("QFLineChart", QRect(10, 10, 700, 550));
    //������ͼ������һ������ͼ��
    std::vector<QPointF> srcPF;
    for (QREAL x = 0.0; x < 20; x += 3.14 / 8) {
        srcPF.push_back(QPointF(x, std::sin(x)));
    }
    qlc->AddCurve(srcPF.data(), srcPF.size());
    //���ǩ�ؼ�����һ���ؼ�������1���ؼ�ָ�룬����2����ǩ����
    qt->AttachCtrl(qlc, 1);
   
    //����һ������ؼ�
    QReportForm* wnd = (QReportForm*)QFUSE::CreateQWND("QFReportForm", QRect(10, 10, 700, 550));
    wnd->InsertTitle(L"��", 100, -1);
    wnd->InsertTitle(L"��", 100, -1);
    wnd->InsertTitle(L"rap", 200, -1);
    wnd->InsertRecorder(L"����");
    for (int i = 0; ++i < 21;) {
        std::wstring wstr(L"���");
        wstr = std::to_wstring(i) + wstr;
        wnd->InsertRecorder(wstr.c_str(), -1);
    }
    for (int i = -1; ++i < 20;) {
        wnd->SetRecorderText(i, 1, L"��ϰ��");
        wnd->SetRecorderText(i, 2, L"��ͨ");
    }
    wnd->IsIsDrag(true);    //�Ƿ�����϶�
    qt->AttachCtrl(wnd, 2);

    qt->SetFontSize(18);//���ñ�ǩ�����С
    
    QEdit* qed = (QEdit*)QFUSE::CreateQWND("QFEdit", QRect(10, 10, 400, 50));

    qt->AttachCtrl(qed, 3);


   

    // ����Ϣѭ��
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
