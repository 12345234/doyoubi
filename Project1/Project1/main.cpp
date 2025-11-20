#include <Windows.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uint, WPARAM wparam, LPARAM lparam)
{
    switch (uint)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_PAINT:

        return 0;
    case WM_KEYDOWN:

        return 0;
    }

    return DefWindowProc(hwnd, uint, wparam, lparam);
}

int WINAPI WinMain(
    HINSTANCE hInstance,      // アプリケーションの識別番号
    HINSTANCE hPrevInstance,  // 基本使わなくていい
    LPSTR lpCmdLine,          // コマンドライン引数（起動時のオプション）
    int nCmdShow              // ウィンドウの表示方法（最大化、最小化など）
)
{
    // ここにメインの処理を書く
    // 1. ウィンドウクラス登録
    WNDCLASS wc{};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "GameWindow";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    RegisterClass(&wc);
    // 2. ウィンドウ作成
    HWND hwnd = CreateWindow(
        "GameWindow",
        "My Game",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,CW_USEDEFAULT,
        800,600,
        NULL,NULL,
        hInstance,
        NULL
        );
    ShowWindow(hwnd, nCmdShow);
    
    //ゲームループ
    bool check = true;
    while (check)
    { 
        // 3. メッセージループ
        MSG msg{};
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)//アプリを閉じたら
            {
                check = false;
            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        
    }
    
    return 0;
}