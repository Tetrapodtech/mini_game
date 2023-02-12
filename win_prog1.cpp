#include <windows.h>
#include <windowsx.h>
#include <cstdio>

/* Prototype declaration */
LRESULT CALLBACK WinProc(HWND, UINT, WPARAM, LPARAM);
BOOL InitApp(HINSTANCE, int);

/* Global variable */
HWND hWnd;
char szWinName[] = "Windows_Program_1";

int WINAPI WinMain(HINSTANCE hThisInst, HINSTANCE hPrevInst, LPSTR lpszArgs, int nWinMode) 
{
    MSG msg;

    /* Define, Register, and Display WINDOW */
    if (!InitApp(hThisInst, nWinMode)) return (FALSE);

    /* Loop of Message */
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg); /* Enable to use keyboard */
        DispatchMessage(&msg); /* Back to WINDOWS */
    }

    return msg.wParam;

}

/* WindowProcedure */
LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    HDC hdc; /* Device context */
    PAINTSTRUCT ps; /* Structure for Repainting*/

    char title[] = "BONE OF THE GAME";
    char subtitle[] = "This is so simple block breaker. PRESS SPACE KEY to start.";

    char str[] = "===="; /* Displayed string */
    char str2[] = "+";

    char enemy[] = "****";
    char broken_enemy[] = "    ";

    static int x = 305;
    static int y = 400;
    static int point = 0;
    static int count_bomb = 100;

    static int title_or_not = 1;
    static int use_bomb = 0;

    static int block_broken = 0;

    char buf[20];
    char buf2[20];
    char buf3[50];
    char buf4[20];
    char buf5[50];
    switch (message) {
        case WM_KEYDOWN: 
        switch(wParam) {
            case VK_ESCAPE:
            title_or_not = 1;
            break;

            case VK_SPACE:
            title_or_not = 0;
            break;

            case VK_UP:
            point += 100;
            use_bomb = 1;
            count_bomb -= 1;
            if ((count_bomb < 0) || (block_broken == 1)) {
                count_bomb = 0;
                point -= 100;
            }
            break;

            case VK_LEFT: 
            x -= 5;
            point += 10;
            if ((x < 0) || (count_bomb == 0) || (block_broken == 1)) {
                x = 0;
                point -= 10;
            }
            break;

            case VK_RIGHT: 
            x += 5;
            point += 10;
            if ((x > 580) || (count_bomb == 0) || (block_broken == 1)) {
                x = 580;
                point -= 10;
            }
            break;

        }
        InvalidateRect(hWnd, NULL, 1);
        break;
        case WM_PAINT:
        snprintf(buf, 20, "SCORE: %09d", point);
        snprintf(buf2, 20, "BOMB: %03d", count_bomb);
        snprintf(buf3, 50, "PRESS ESC KEY to back to the title. SCORE: %d", point);
        snprintf(buf5, 50, "GAME CLEARED! You did it! SCORE: %d", point);
        hdc = BeginPaint(hWnd, &ps); /* Start drawing */

        if (title_or_not) {
        point = 0;
        x = 305;
        y = 400;
        use_bomb = 0;
        count_bomb = 100;
        block_broken = 0;

        // title
        TextOut(hdc, 315, 209, title, strlen(title));
        TextOut(hdc, 50, 239, subtitle, strlen(subtitle));
        } else if (block_broken == 1) {
        TextOut(hdc, 50, 239, buf5, strlen(buf5));
        } else if (count_bomb == 0) {
        TextOut(hdc, 50, 239, buf3, strlen(buf3));
        }
           else {
         // own machine
        TextOut(hdc, x, y, str, strlen(str));

        // using bomb
        if (use_bomb) {

            if ((380-1 < x) && (x < 380+81)) {
                block_broken = 1;
            }
            // write_bomb
            if (count_bomb > 0) {
            for (int i = 0; i < 20; i++) TextOut(hdc, x, y-20*i, str2, strlen(str2));
            }
            use_bomb = 0;
        }

        // display enemy(block)
        if (!block_broken) {
            TextOut(hdc, 380, 20, enemy, strlen(enemy));
        } else {
            TextOut(hdc, 380, 20, broken_enemy, strlen(broken_enemy));
        }

        // your score
        TextOut(hdc, 0, 0, buf, strlen(buf));

        // how many times you used bombs
        TextOut(hdc, 0, 20, buf2, strlen(buf2));
        }


        EndPaint(hWnd, &ps); /* End drawing */
        break;
        case WM_DESTROY: /* Click "CLOSE" */
        PostQuitMessage(0); /* WM_QUIT message */
        break;
        default: /* Otherwise... */
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

/* InitApp(Define, Register, and display WINDOW.): 1(normal), 0(abnormal)*/
BOOL InitApp(HINSTANCE hThisInst, int nWinMode) {
    WNDCLASSEX wc;

    /* Define CLASS of WINDOW */
    wc.hInstance = hThisInst; /* HANDLE */
    wc.lpszClassName = szWinName; /* Name of CLASS of WINDOW */
    wc.lpfnWndProc = WinProc; /* FUNCTION of WINDOW */
    wc.style = 0; /* STYLE of WINDOW */
    wc.cbSize = sizeof(WNDCLASSEX); /* SIZE of WNDCLASSEX */
    wc.hIcon = LoadIcon(hThisInst, IDI_APPLICATION); /* LARGE ICON */
    wc.hIconSm = LoadIcon(hThisInst, IDI_WINLOGO); /* SMALL ICON */
    wc.hCursor = LoadCursor(NULL, IDC_ARROW); /* STYLE of CURSOR */
    wc.lpszMenuName = NULL; /* MENU */
    wc.cbClsExtra = 0; /* EXTRA */
    wc.cbWndExtra = 0;  /* NECESSARY INFORMATION */
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); /* BACKGROUND COLOR */

    /* Register CLASS of WINDOW */
    if(!RegisterClassEx(&wc)) return (FALSE);

    /* Create window */
    hWnd = CreateWindowEx(
        0, /* STYLE of EXTRA WINDOW */
        szWinName, /* NAME of CLASS of WINDOW */
        "bone of the game", /* TITLE of WINDOW */
        WS_OVERLAPPEDWINDOW, /* STYLE of WINDOW */
        0, /* X coordinate(Left end) */
        0, /* Y coordinate(Left end) */
        640, /* WIDTH of WINDOW */
        480, /* HEIGHT of WINDOW */
        NULL, /* PARENT */
        NULL, /* MENU */
        hThisInst, /* HANDLE */
        NULL /* Extra argument */
    );

    ShowWindow(hWnd, nWinMode); /* Display WINDOW */
    UpdateWindow(hWnd);

    return (TRUE);
}