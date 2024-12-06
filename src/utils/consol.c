#include "consol.h"

void consol_set_color(color_e color, bool bright)
{

#ifdef  _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int win_color = (int)color;
    if (bright) win_color += 8;
    SetConsoleTextAttribute(hConsole, win_color);
#else
    int ansi_color = color;
    if (bright) {
        printf("\033[%d;1m", 30 + ansi_color);
    } else {
        printf("\033[%dm", 30 + ansi_color);
    }
#endif

}

void consol_reset_color()
{

#ifdef  _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 7);
#else
    printf("\033[0m");
#endif
    
}

usize consol_get_width()
{

#ifdef  _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.srWindow.Right - csbi.srWindow.Left;
#else
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_col;
#endif

}