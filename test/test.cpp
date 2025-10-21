#include <algorithm>
#include <array>
#include <chrono>
#include <iostream>
#include <queue>
#include <random>
#include <string>
#include <vector>
#include <stdio.h>

using std::array, std::vector;
using std::cout, std::endl, std::cin;

extern "C"
{
    void printboard(int *board, int boardlen)
    {
        for (int i = 0; i < boardlen; i++)
        {
            printf("%d", board[i]);
        }
    }
}
