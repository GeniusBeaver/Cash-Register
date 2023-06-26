#include <string>
#include "lenta.h"

bool isStart = false;

lenta Niva;

void UpdatePowerAndStatus(HWND hWnd)
{
    SetWindowTextA(GetDlgItem(hWnd, IDC_CLIENTS), (" " + std::to_string(Niva.getClients())).c_str());
    SetWindowTextA(GetDlgItem(hWnd, IDC_NOW_CLIENTS), (" " + std::to_string(Niva.checkCl())).c_str());

    SetWindowTextA(GetDlgItem(hWnd, IDC_CASSA_TIME_1), (Niva.checkWork(0).c_str()));
    SetWindowTextA(GetDlgItem(hWnd, IDC_CASSA_TIME_2), (Niva.checkWork(1).c_str()));
    SetWindowTextA(GetDlgItem(hWnd, IDC_CASSA_TIME_3), (Niva.checkWork(2).c_str()));

    SetWindowTextA(GetDlgItem(hWnd, IDC_CASSA_1), ((" " + std::to_string(Niva.checkTime(0))).c_str()));
    SetWindowTextA(GetDlgItem(hWnd, IDC_CASSA_2), ((" " + std::to_string(Niva.checkTime(1))).c_str()));
    SetWindowTextA(GetDlgItem(hWnd, IDC_CASSA_3), ((" " + std::to_string(Niva.checkTime(2))).c_str()));
}


//основной цикл сообщений
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
        //создание элементов управления на форме
        CreateWindowEx(0, L"STATIC", L"All clients:", WS_CHILD | WS_VISIBLE, 10, 10, 120, 20, hWnd, (HMENU)IDC_STATIC, nullptr, nullptr);
        CreateWindowEx(0, L"STATIC", L"Now clients:", WS_CHILD | WS_VISIBLE, 10, 40, 120, 20, hWnd, (HMENU)IDC_STATIC, nullptr, nullptr);
        CreateWindowEx(0, L"STATIC", L"", WS_CHILD | WS_VISIBLE, 140, 10, 120, 20, hWnd, (HMENU)IDC_CLIENTS, nullptr, nullptr);
        CreateWindowEx(0, L"STATIC", L"", WS_CHILD | WS_VISIBLE, 140, 40, 120, 20, hWnd, (HMENU)IDC_NOW_CLIENTS, nullptr, nullptr);

        CreateWindowEx(0, L"STATIC", L"CASSA 1:", WS_CHILD | WS_VISIBLE, 10, 70, 120, 20, hWnd, (HMENU)IDC_STATIC, nullptr, nullptr);
        CreateWindowEx(0, L"STATIC", L"CASSA 2:", WS_CHILD | WS_VISIBLE, 10, 100, 120, 20, hWnd, (HMENU)IDC_STATIC, nullptr, nullptr);
        CreateWindowEx(0, L"STATIC", L"CASSA 3:", WS_CHILD | WS_VISIBLE, 10, 130, 120, 20, hWnd, (HMENU)IDC_STATIC, nullptr, nullptr);
        CreateWindowEx(0, L"STATIC", L"", WS_CHILD | WS_VISIBLE, 140, 70, 120, 20, hWnd, (HMENU)IDC_CASSA_1, nullptr, nullptr);
        CreateWindowEx(0, L"STATIC", L"", WS_CHILD | WS_VISIBLE, 140, 100, 120, 20, hWnd, (HMENU)IDC_CASSA_2, nullptr, nullptr);
        CreateWindowEx(0, L"STATIC", L"", WS_CHILD | WS_VISIBLE, 140, 130, 120, 20, hWnd, (HMENU)IDC_CASSA_3, nullptr, nullptr);

        CreateWindowEx(0, L"STATIC", L"", WS_CHILD | WS_VISIBLE, 270, 70, 90, 20, hWnd, (HMENU)IDC_CASSA_TIME_1, nullptr, nullptr);
        CreateWindowEx(0, L"STATIC", L"", WS_CHILD | WS_VISIBLE, 270, 100, 90, 20, hWnd, (HMENU)IDC_CASSA_TIME_2, nullptr, nullptr);
        CreateWindowEx(0, L"STATIC", L"", WS_CHILD | WS_VISIBLE, 270, 130, 90, 20, hWnd, (HMENU)IDC_CASSA_TIME_3, nullptr, nullptr);

        CreateWindowEx(0, L"BUTTON", L" ON", WS_CHILD | WS_VISIBLE, 370, 70, 50, 20, hWnd, (HMENU)IDC_BUTTON_1, nullptr, nullptr);
        CreateWindowEx(0, L"BUTTON", L" ON", WS_CHILD | WS_VISIBLE, 370, 100, 50, 20, hWnd, (HMENU)IDC_BUTTON_2, nullptr, nullptr);
        CreateWindowEx(0, L"BUTTON", L" ON", WS_CHILD | WS_VISIBLE, 370, 130, 50, 20, hWnd, (HMENU)IDC_BUTTON_3, nullptr, nullptr);

        Niva.startwork();

        SetTimer(hWnd, 1, 1000, NULL);

        break;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDC_BUTTON_1:
            if (Niva.changeWork(0))
                SetWindowText(GetDlgItem(hWnd, IDC_BUTTON_1), L"  ON");
            else
                SetWindowText(GetDlgItem(hWnd, IDC_BUTTON_1), L" OFF");

            break;

        case IDC_BUTTON_2:
            if (Niva.changeWork(1))
                SetWindowText(GetDlgItem(hWnd, IDC_BUTTON_2), L"  ON");
            else
                SetWindowText(GetDlgItem(hWnd, IDC_BUTTON_2), L" OFF");

            break;

        case IDC_BUTTON_3:
            if (Niva.changeWork(2))
                SetWindowText(GetDlgItem(hWnd, IDC_BUTTON_3), L"  ON");
            else
                SetWindowText(GetDlgItem(hWnd, IDC_BUTTON_3), L" OFF");

            break;

        }
        break;

    case WM_TIMER:
        //if (wParam == ID_TIMER1)
            UpdatePowerAndStatus(hWnd);

        break;

    case WM_CLOSE:
        //остановка потоков и освобождение ресурсов
        DestroyWindow(hWnd);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wcex;
    HWND hWnd;
    MSG msg;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = L"MainWindowClass";
    wcex.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
    RegisterClassEx(&wcex);

    hWnd = CreateWindowEx(0, L"MainWindowClass", L"Super Lenta", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 450, 200, nullptr, nullptr, hInstance, nullptr);
    if (!hWnd)
    {
        return false;
    }

    //отображение и обновление главного окна
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    //основной цикл сообщений
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}