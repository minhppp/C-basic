// Snake.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

/*
* ý tưởng trò chơi snake basic
* gồm: +1 map chính
*      +1 rắn 
*      +1 bộ đếm điểm
*      +di chuyển bằng 2 bộ phím aswd và ↑↓←→
*      +đụng tường, tự cán => game over
*      +khi >100 điểm => tăng tốc 
* Sử dụng mảng 1 chiều, đồ họa console
*/

#include <iostream>
#include <windows.h>
#include "mylibb.h"

#define MAX_count 1000

using namespace std;
int score = 0;
int fruitX = 0, fruitY = 0; // Vị trí thức ăn

//int x, y, fruitX, fruitY, score;
int localX[MAX_count] = { 0 };
int localY[MAX_count] = { 0 };
int countstart = 3;
//enum eDirecton { LEFT, RIGHT, DOWN, UP };
//eDirecton dir;
int count ;
//------------------//
void gameplay(); //hàm game chính
void draw_map(); //vẽ map game
void disableScroll(); // khóa cuộn màn hình console
void make_Snake();
void draw_Snake();
void Input(int& x, int& y, int& check);
void Move_Snake(int x, int y);
void clear_Snake();
bool GameOver(int x, int y);
void create_fruit();
void show_score();
void debug_snake();
int main()//package 
{   
    //if (//gameover = false) {
        disableScroll();
        gameplay();

    //}
    //else {
    //    cout << "Game Over";
    //}
    _getch();// dong khi nhan 1 nut bat ky va xoa chu trong man hinh console
    return 0;
}
void disableScroll() {
    // Lấy handle của console
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // Lấy thông tin về buffer hiện tại
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);

    // Thiết lập kích thước của buffer bằng kích thước của cửa sổ
    SMALL_RECT windowSize = consoleInfo.srWindow;
    COORD bufferSize;
    bufferSize.X = windowSize.Right - windowSize.Left + 1;
    bufferSize.Y = windowSize.Bottom - windowSize.Top + 1;

    // Cập nhật kích thước buffer để trùng với kích thước cửa sổ
    SetConsoleScreenBufferSize(hConsole, bufferSize);
}
void draw_map()
{
    // Vẽ 2 thanh ngang trục Y
    for (int x = 10; x < 110; x++) 
    {
        gotoXY(x, 7);
        cout << "#";
        gotoXY(x, 27);
        cout << "#";
    }
    // Vẽ 2 thanh dọc trục Y
    for (int y = 7; y <= 27; y++)
    {
        gotoXY(10, y);
        cout << "#";
        gotoXY(110, y);
        cout << "#";
    }
}
void make_Snake() {
    int x_make = 60; // Điểm đầu tiên của rắn (đầu rắn)
    int y_make = 17; // Cố định trục Y ban đầu
    for (int i = 0; i <= countstart; i++) {
        localX[i] = x_make - i; // Các phần tử tiếp theo nằm bên trái đầu rắn
        localY[i] = y_make;     // Trục Y giữ nguyên
    }
}
void draw_Snake() {
    for (int i = 0; i <= countstart; i++) {
        gotoXY(localX[i], localY[i]);
        if (i == 0) {
            cout << "0"; // Đầu rắn
        }
        else {
            cout << "o"; // Thân rắn
        }
    }
}
/*void Move_Snake(int x, int y) {
    // Di chuyển các phần tử 
    for (int i = countstart; i > 0; i--) {
        localX[i] = localX[i - 1];
        localY[i] = localY[i - 1];
    }
    // Cập nhật vị trí đầu rắn
    localX[0] = x;
    localY[0] = y;
}*/
void Move_Snake(int& x, int& y, int check) {
    // Di chuyển các phần tử 
    for (int i = countstart; i > 0; i--) {
        localX[i] = localX[i - 1];
        localY[i] = localY[i - 1];
    }

    // Cập nhật vị trí đầu rắn dựa trên điều khiển
    switch (check) {
    case 0: x--; break; // Trái
    case 1: x++; break; // Phải
    case 2: y--; break; // Lên
    case 3: y++; break; // Xuống
    }

    localX[0] = x;
    localY[0] = y;
}
void Input(int& x, int& y, int& check) 
{
    if (_kbhit()) {
        switch (_getch()) {
        case 'a': // Di chuyển trái
            if (check != 1) {
                check = 0;
                x--;
            }
            break;
        case 'd': // Di chuyển phải
            if (check != 0) {
                check = 1;
                x++;
            }
            break;
        case 'w': // Di chuyển lên
            if (check != 3) {
                check = 2;
                y--;
            }
            break;
        case 's': // Di chuyển xuống
            if (check != 2) {
                check = 3;
                y++;
            }
            break;
        }
    }
}
void clear_Snake() {
    for (int i = 0; i <= countstart; i++) {
        gotoXY(localX[i], localY[i]);
        cout << " ";
    }
}
bool GameOver(int x, int y) {
    // Kiểm tra va chạm với tường
    if (x < 10 || x > 110 || y < 7 || y > 27) {
        return true;
    }
    // Kiểm tra va chạm với thân rắn
    for (int i = 1; i <= countstart; i++) { // Bỏ qua đầu rắn (localX[0], localY[0])
        if (x == localX[i] && y == localY[i]) {
            return true;
        }
    }
    return false;
}
void create_fruit()
{
    bool valid = false;
    while (!valid) {
        fruitX = rand() % (109 - 9 + 1) + 10;
        fruitY = rand() % (26 - 6 + 1) + 7;
        valid = true;

        // Kiểm tra xem vị trí thức ăn có trùng với cơ thể rắn không
        for (int i = 0; i <= countstart; i++) {
            if (fruitX == localX[i] && fruitY == localY[i]) {
                valid = false; // Nếu trùng, tạo lại
                break;
            }
        }
    }
    gotoXY(fruitX, fruitY);
    cout << "*";
}

void show_score() 
{
    gotoXY(5, 5);
    cout << "Score: " << score;
}
void debug_snake() {
    /*for (int i = 0; i <= countstart; i++) {
        cout << "Part " << i << ": (" << localX[i] << ", " << localY[i] << ")\n";
    }
    cout << "Fruit: (" << fruitX << ", " << fruitY << ")\n";
    */
}

void gameplay() {
    int x = 60, y = 17; // Vị trí ban đầu của đầu rắn
    int check = 3; // 0: Trái, 1: Phải, 2: Lên, 3: Xuống
    int speed =100; // Tốc độ ban đầu
    make_Snake();
    //debug_snake();
    create_fruit();
    draw_map();
    while (true) {
        clear_Snake();  // Xóa rắn cũ
        Input(x, y, check); // Nhập điều khiển từ bàn phím
        if (GameOver(x, y)) { // Kiểm tra kết thúc
            gotoXY(50, 15);
            cout << "GAME OVER!";
            break;
        }
        if (x == fruitX && y == fruitY) { // Ăn thức ăn
            countstart++;
            create_fruit();
            score += 10; // Tăng điểm
            if (score < 100) speed = speed + 100; // Tăng tốc
            else if (score < 200 || score >100) speed = speed +200;
        }
        show_score();     // Hiển thị điểm số
        Move_Snake(x, y,check); // Di chuyển rắn
        draw_Snake();     // Vẽ lại rắn
        Sleep(speed);     // Tốc độ di chuyển
    }
}



