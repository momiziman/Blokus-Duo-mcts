// 毎回乱数を使う原始モンテカルロ法

// VSCODEの拡張機能C/C++の設定
// "C_Cpp.clang_format_sortIncludes": true,
// "C_Cpp.clang_format_style": "Google" でフォーマット済み
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
#include <algorithm>
#include <array>
#include <chrono>
#include <iostream>
#include <queue>
#include <random>
#include <string>
#include <vector>
#include <unordered_map>
#include <tuple>
// エイリアス(よくわからない人はAPG4bに従いusing namespace std;をしてください)
using std::array, std::vector;
using std::cout, std::endl, std::cin;
using std::deque;
using std::pair;
using std::swap;

vector<vector<int>> rot90(const vector<vector<int>> &mat, int k)
{
    k = ((k % 4) + 4) % 4; // 安全に0〜3に正規化
    vector<vector<int>> res = mat;
    for (int i = 0; i < k; ++i)
    {
        size_t rows = res.size();
        size_t cols = res[0].size();
        vector<vector<int>> tmp(cols, vector<int>(rows));
        for (size_t r = 0; r < rows; ++r)
            for (size_t c = 0; c < cols; ++c)
                tmp[c][rows - 1 - r] = res[r][c];
        res = tmp;
    }
    return res;
}

int main()
{
    vector<vector<int>> shape = {
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 1, 1, 1, 1},
        {0, 1, 0, 0, 0},
        {0, 0, 0, 0, 0}};

    auto rotated = rot90(shape, 2);

    for (auto &row : rotated)
    {
        for (auto v : row)
            cout << v << " ";
        cout << endl;
    }

    return 0;
}
