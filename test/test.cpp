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

extern "C" __declspec(dllexport) void printboard(int *board, int tile_number)
{
    int SIZE = tile_number + 2;

    for (int color = 0; color < 2; color++)
    {
        printf("=== COLOR %d ===\n", color);

        for (int y = 0; y < SIZE; y++)
        {
            for (int x = 0; x < SIZE; x++)
            {

                int index = color * SIZE * SIZE + y * SIZE + x;
                printf("%d ", board[index]);
            }
            printf("\n");
        }
        printf("\n");
    }
}
