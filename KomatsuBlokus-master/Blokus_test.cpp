
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
#include <algorithm>
#include <array>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <queue>
#include <random>
#include <string>
#include <vector>
#include <unordered_map>
#include <utility>
#include <tuple>
#include <map>
#include "block_data.h"
#include <bits/stdc++.h>
using namespace std;
using std::array, std::vector;
using std::cout, std::endl, std::cin;
using std::deque;
using std::pair;
using std::swap;
using std::vector;
using namespace std;

const std::unordered_map<std::string, BlockData> block_table = {
    // shape (5x5)
    {"a", {{{{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 1, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}}},
           // influence (7x7)
           {{{0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0}, {0, 0, 2, 1, 2, 0, 0}, {0, 0, 1, 1, 1, 0, 0}, {0, 0, 2, 1, 2, 0, 0}, {0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0}}},
           1}}, // score
    {"b", {{{{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 1, 0, 0}, {0, 0, 1, 0, 0}, {0, 0, 0, 0, 0}}},
           // influence (7x7)
           {{{0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0}, {0, 0, 2, 1, 2, 0, 0}, {0, 0, 1, 1, 1, 0, 0}, {0, 0, 1, 1, 1, 0, 0}, {0, 0, 2, 1, 2, 0, 0}, {0, 0, 0, 0, 0, 0, 0}}},
           2}}, // score
    {"c", {{{{0, 0, 0, 0, 0}, {0, 0, 1, 0, 0}, {0, 0, 1, 0, 0}, {0, 0, 1, 0, 0}, {0, 0, 0, 0, 0}}},
           // influence (7x7)
           {{{0, 0, 0, 0, 0, 0, 0}, {0, 0, 2, 1, 2, 0, 0}, {0, 0, 1, 1, 1, 0, 0}, {0, 0, 1, 1, 1, 0, 0}, {0, 0, 1, 1, 1, 0, 0}, {0, 0, 2, 1, 2, 0, 0}, {0, 0, 0, 0, 0, 0, 0}}},
           3}}, // score
    {"d", {{{{0, 0, 0, 0, 0}, {0, 0, 1, 0, 0}, {0, 0, 1, 1, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}}},
           // influence (7x7)
           {{{0, 0, 0, 0, 0, 0, 0}, {0, 0, 2, 1, 2, 0, 0}, {0, 0, 1, 1, 1, 2, 0}, {0, 0, 1, 1, 1, 1, 0}, {0, 0, 2, 1, 1, 2, 0}, {0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0}}},
           3}}, // score
    {"e", {{{{0, 0, 0, 0, 0}, {0, 0, 1, 0, 0}, {0, 0, 1, 0, 0}, {0, 0, 1, 0, 0}, {0, 0, 1, 0, 0}}},
           // influence (7x7)
           {{{0, 0, 0, 0, 0, 0, 0}, {0, 0, 2, 1, 2, 0, 0}, {0, 0, 1, 1, 1, 0, 0}, {0, 0, 1, 1, 1, 0, 0}, {0, 0, 1, 1, 1, 0, 0}, {0, 0, 1, 1, 1, 0, 0}, {0, 0, 2, 1, 2, 0, 0}}},
           4}}, // score
    {"f", {{{{0, 0, 0, 0, 0}, {0, 0, 1, 0, 0}, {0, 0, 1, 0, 0}, {0, 1, 1, 0, 0}, {0, 0, 0, 0, 0}}},
           // influence (7x7)
           {{{0, 0, 0, 0, 0, 0, 0}, {0, 0, 2, 1, 2, 0, 0}, {0, 0, 1, 1, 1, 0, 0}, {0, 2, 1, 1, 1, 0, 0}, {0, 1, 1, 1, 1, 0, 0}, {0, 2, 1, 1, 2, 0, 0}, {0, 0, 0, 0, 0, 0, 0}}},
           4}}, // score
    {"g", {{{{0, 0, 0, 0, 0}, {0, 0, 1, 0, 0}, {0, 0, 1, 1, 0}, {0, 0, 1, 0, 0}, {0, 0, 0, 0, 0}}},
           // influence (7x7)
           {{{0, 0, 0, 0, 0, 0, 0}, {0, 0, 2, 1, 2, 0, 0}, {0, 0, 1, 1, 1, 2, 0}, {0, 0, 1, 1, 1, 1, 0}, {0, 0, 1, 1, 1, 2, 0}, {0, 0, 2, 1, 2, 0, 0}, {0, 0, 0, 0, 0, 0, 0}}},
           4}}, // score
    {"h", {{{{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 1, 1, 0}, {0, 0, 1, 1, 0}, {0, 0, 0, 0, 0}}},
           // influence (7x7)
           {{{0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0}, {0, 0, 2, 1, 1, 2, 0}, {0, 0, 1, 1, 1, 1, 0}, {0, 0, 1, 1, 1, 1, 0}, {0, 0, 2, 1, 1, 2, 0}, {0, 0, 0, 0, 0, 0, 0}}},
           4}}, // score
    {"i", {{{{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 1, 1, 0, 0}, {0, 0, 1, 1, 0}, {0, 0, 0, 0, 0}}},
           // influence (7x7)
           {{{0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0}, {0, 2, 1, 1, 2, 0, 0}, {0, 1, 1, 1, 1, 2, 0}, {0, 2, 1, 1, 1, 1, 0}, {0, 0, 2, 1, 1, 2, 0}, {0, 0, 0, 0, 0, 0, 0}}},
           4}}, // score
    {"j", {{{{0, 0, 1, 0, 0}, {0, 0, 1, 0, 0}, {0, 0, 1, 0, 0}, {0, 0, 1, 0, 0}, {0, 0, 1, 0, 0}}},
           // influence (7x7)
           {{{0, 0, 2, 1, 2, 0, 0}, {0, 0, 1, 1, 1, 0, 0}, {0, 0, 1, 1, 1, 0, 0}, {0, 0, 1, 1, 1, 0, 0}, {0, 0, 1, 1, 1, 0, 0}, {0, 0, 1, 1, 1, 0, 0}, {0, 0, 2, 1, 2, 0, 0}}},
           5}}, // score
    {"k", {{{{0, 0, 1, 0, 0}, {0, 0, 1, 0, 0}, {0, 0, 1, 0, 0}, {0, 1, 1, 0, 0}, {0, 0, 0, 0, 0}}},
           // influence (7x7)
           {{{0, 0, 2, 1, 2, 0, 0}, {0, 0, 1, 1, 1, 0, 0}, {0, 0, 1, 1, 1, 0, 0}, {0, 2, 1, 1, 1, 0, 0}, {0, 1, 1, 1, 1, 0, 0}, {0, 2, 1, 1, 2, 0, 0}, {0, 0, 0, 0, 0, 0, 0}}},
           5}}, // score
    {"l", {{{{0, 0, 1, 0, 0}, {0, 0, 1, 0, 0}, {0, 1, 1, 0, 0}, {0, 1, 0, 0, 0}, {0, 0, 0, 0, 0}}},
           // influence (7x7)
           {{{0, 0, 2, 1, 2, 0, 0}, {0, 0, 1, 1, 1, 0, 0}, {0, 2, 1, 1, 1, 0, 0}, {0, 1, 1, 1, 1, 0, 0}, {0, 1, 1, 1, 2, 0, 0}, {0, 2, 1, 2, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0}}},
           5}}, // score
    {"m", {{{{0, 0, 0, 0, 0}, {0, 0, 1, 0, 0}, {0, 1, 1, 0, 0}, {0, 1, 1, 0, 0}, {0, 0, 0, 0, 0}}},
           // influence (7x7)
           {{{0, 0, 0, 0, 0, 0, 0}, {0, 0, 2, 1, 2, 0, 0}, {0, 2, 1, 1, 1, 0, 0}, {0, 1, 1, 1, 1, 0, 0}, {0, 1, 1, 1, 1, 0, 0}, {0, 2, 1, 1, 2, 0, 0}, {0, 0, 0, 0, 0, 0, 0}}},
           5}}, // score
    {"n", {{{{0, 0, 0, 0, 0}, {0, 1, 1, 0, 0}, {0, 0, 1, 0, 0}, {0, 1, 0, 0, 0}, {0, 0, 0, 0, 0}}},
           // influence (7x7)
           {{{0, 0, 0, 0, 0, 0, 0}, {0, 2, 1, 1, 2, 0, 0}, {0, 1, 1, 1, 1, 0, 0}, {0, 2, 1, 1, 1, 0, 0}, {0, 1, 1, 1, 1, 0, 0}, {0, 2, 1, 1, 2, 0, 0}, {0, 0, 0, 0, 0, 0, 0}}},
           5}}, // score
    {"o", {{{{0, 0, 0, 0, 0}, {0, 0, 1, 0, 0}, {0, 0, 1, 1, 0}, {0, 0, 1, 0, 0}, {0, 0, 1, 0, 0}}},
           // influence (7x7)
           {{{0, 0, 0, 0, 0, 0, 0}, {0, 0, 2, 1, 2, 0, 0}, {0, 0, 1, 1, 1, 2, 0}, {0, 0, 1, 1, 1, 1, 0}, {0, 0, 1, 1, 1, 2, 0}, {0, 0, 1, 1, 1, 0, 0}, {0, 0, 2, 1, 2, 0, 0}}},
           5}}, // score
    {"p", {{{{0, 0, 0, 0, 0}, {0, 0, 1, 0, 0}, {0, 0, 1, 0, 0}, {0, 1, 1, 1, 0}, {0, 0, 0, 0, 0}}},
           // influence (7x7)
           {{{0, 0, 0, 0, 0, 0, 0}, {0, 0, 2, 1, 2, 0, 0}, {0, 0, 1, 1, 1, 0, 0}, {0, 2, 1, 1, 1, 2, 0}, {0, 1, 1, 1, 1, 1, 0}, {0, 2, 1, 1, 1, 2, 0}, {0, 0, 0, 0, 0, 0, 0}}},
           5}}, // score
    {"q", {{{{0, 0, 1, 0, 0}, {0, 0, 1, 0, 0}, {0, 0, 1, 1, 1}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}}},
           // influence (7x7)
           {{{0, 0, 2, 1, 2, 0, 0}, {0, 0, 1, 1, 1, 0, 0}, {0, 0, 1, 1, 1, 1, 2}, {0, 0, 1, 1, 1, 1, 1}, {0, 0, 2, 1, 1, 1, 2}, {0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0}}},
           5}}, // score
    {"r", {{{{0, 0, 0, 0, 0}, {0, 1, 1, 0, 0}, {0, 0, 1, 1, 0}, {0, 0, 0, 1, 0}, {0, 0, 0, 0, 0}}},
           // influence (7x7)
           {{{0, 0, 0, 0, 0, 0, 0}, {0, 2, 1, 1, 2, 0, 0}, {0, 1, 1, 1, 1, 2, 0}, {0, 2, 1, 1, 1, 1, 0}, {0, 0, 2, 1, 1, 1, 0}, {0, 0, 0, 2, 1, 2, 0}, {0, 0, 0, 0, 0, 0, 0}}},
           5}}, // score
    {"s", {{{{0, 0, 0, 0, 0}, {0, 1, 0, 0, 0}, {0, 1, 1, 1, 0}, {0, 0, 0, 1, 0}, {0, 0, 0, 0, 0}}},
           // influence (7x7)
           {{{0, 0, 0, 0, 0, 0, 0}, {0, 2, 1, 2, 0, 0, 0}, {0, 1, 1, 1, 1, 2, 0}, {0, 1, 1, 1, 1, 1, 0}, {0, 2, 1, 1, 1, 1, 0}, {0, 0, 0, 2, 1, 2, 0}, {0, 0, 0, 0, 0, 0, 0}}},
           5}}, // score
    {"t", {{{{0, 0, 0, 0, 0}, {0, 1, 0, 0, 0}, {0, 1, 1, 1, 0}, {0, 0, 1, 0, 0}, {0, 0, 0, 0, 0}}},
           // influence (7x7)
           {{{0, 0, 0, 0, 0, 0, 0}, {0, 2, 1, 2, 0, 0, 0}, {0, 1, 1, 1, 1, 2, 0}, {0, 1, 1, 1, 1, 1, 0}, {0, 2, 1, 1, 1, 2, 0}, {0, 0, 2, 1, 2, 0, 0}, {0, 0, 0, 0, 0, 0, 0}}},
           5}}, // score
    {"u", {{{{0, 0, 0, 0, 0}, {0, 0, 1, 0, 0}, {0, 1, 1, 1, 0}, {0, 0, 1, 0, 0}, {0, 0, 0, 0, 0}}},
           // influence (7x7)
           {{{0, 0, 0, 0, 0, 0, 0}, {0, 0, 2, 1, 2, 0, 0}, {0, 2, 1, 1, 1, 2, 0}, {0, 1, 1, 1, 1, 1, 0}, {0, 2, 1, 1, 1, 2, 0}, {0, 0, 2, 1, 2, 0, 0}, {0, 0, 0, 0, 0, 0, 0}}},
           5}}, // score

};

constexpr int TILE_NUMBER = 14;
constexpr int COLOR_NUM = 2;
constexpr int BOARD_SIZE = TILE_NUMBER + 2; // 壁を含めたサイズ

enum TileState
{
    BLANK = 0,
    CANTSET = 1,
    ABLESET = 2
};

enum class Color
{
    PLAYER1 = 0,
    PLAYER2 = 1
};

struct Position
{
    int x;
    int y;
};

struct Block
{
    vector<vector<int>> shape;
    vector<vector<int>> influence;

    Block(const BlockData &data)
    {
        // std::array → vector に変換
        shape.resize(5, vector<int>(5));
        for (int i = 0; i < 5; ++i)
            for (int j = 0; j < 5; ++j)
                shape[i][j] = data.shape[i][j];

        influence.resize(7, vector<int>(7));
        for (int i = 0; i < 7; ++i)
            for (int j = 0; j < 7; ++j)
                influence[i][j] = data.influence[i][j];
    }

    // 2次元ベクトルを転置
    vector<vector<int>> transpose(const vector<vector<int>> &mat)
    {
        size_t rows = mat.size();
        size_t cols = mat[0].size();
        vector<vector<int>> res(cols, vector<int>(rows));
        for (size_t i = 0; i < rows; ++i)
            for (size_t j = 0; j < cols; ++j)
                res[j][i] = mat[i][j];
        return res;
    }

    // 2次元ベクトルを90°回転（時計回りにk回）
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

    void rotate_block(int dir)
    {
        switch (dir)
        {
        case 0: // 初期向き
            break;

        case 1:                                 // 裏向き
            shape = rot90(transpose(shape), 3); // Pythonでは -1
            influence = rot90(transpose(influence), 3);
            break;

        case 2: // 初期向きから90°時計回り
            shape = rot90(shape, 1);
            influence = rot90(influence, 1);
            break;

        case 3: // 裏向きから90°反時計回り
            shape = transpose(shape);
            influence = transpose(influence);
            break;

        case 4: // 初期向きから180°時計回り
            shape = rot90(shape, 2);
            influence = rot90(influence, 2);
            break;

        case 5: // 裏向きから180°反時計回り
            shape = rot90(transpose(shape), 1);
            influence = rot90(transpose(influence), 1);
            break;

        case 6: // 初期向きから270°時計回り
            shape = rot90(shape, 3);
            influence = rot90(influence, 3);
            break;

        case 7: // 裏向きから270°反時計回り
            shape = rot90(transpose(shape), 2);
            influence = rot90(transpose(influence), 2);
            break;
        }
    }
};

BlockData getBlock(const std::string &id)
{
    auto it = block_table.find(id);
    if (it == block_table.end())
    {
        throw std::runtime_error("Error: block id '" + id + "' not found!");
    }
    return it->second;
}

struct Player
{
    Color color;
    vector<string> used_blocks; // 使用済みブロックIDを記録
    int score = 0;
};

class Board
{
public:
    inline static const int BLANK = 0;
    inline static const int CANTSET = 1;
    inline static const int ABLESET = 2;

    int TILE_NUMBER;

    // status[color][y][x] の形で入力される
    vector<vector<vector<int>>> status; // [COLOR_NUM][TILE_NUMBER+2][TILE_NUMBER+2]

    Board(int tile_number, const vector<vector<vector<int>>> &input_board)
        : TILE_NUMBER(tile_number), status(input_board) {}

    // ---------- settable_check ----------
    bool settable_check(Color color, const vector<vector<int>> &block_shape, int x, int y)
    {
        int col = static_cast<int>(color);
        bool found_corner = false; // ABLESET に触れたかどうか

        int H = block_shape.size();
        int W = block_shape[0].size();

        for (int i = 0; i < H; ++i)
        {
            for (int j = 0; j < W; ++j)
            {
                if (block_shape[i][j] != CANTSET)
                    continue;

                int access_y = y + i - 2;
                int access_x = x + j - 2;

                // 範囲外は無視
                if (access_y < 0 || access_y >= TILE_NUMBER + 2 ||
                    access_x < 0 || access_x >= TILE_NUMBER + 2)
                    continue;

                int cell = status[col][access_y][access_x];

                if (cell == CANTSET)
                    return false; // 隣接 or 自身にぶつかる → 置けない

                if (cell == ABLESET)
                    found_corner = true; // 角接触
            }
        }

        return found_corner;
    }

    // ---------- 合法手探索 ----------
    vector<pair<int, int>> search_settable_position(Color color, const vector<vector<int>> &block_shape)
    {
        vector<pair<int, int>> res;
        int col = static_cast<int>(color);

        for (int y = 1; y <= TILE_NUMBER; ++y)
        {
            for (int x = 1; x <= TILE_NUMBER; ++x)
            {
                if (status[col][y][x] != CANTSET)
                {
                    if (settable_check(color, block_shape, x, y))
                        res.push_back({x, y});
                }
            }
        }

        return res;
    }

    // Boardクラス内
    void change_status(Color color,
                       Block &block, // Blockオブジェクトを直接渡す
                       const std::string &block_id,
                       int rotation, // 追加: 0〜7 の回転番号
                       int x, int y,
                       Player &player)
    {
        // 回転を適用
        block.rotate_block(rotation);

        // --- 自分の盤面へ influence を適用 ---
        int H = status[0].size();
        int W = status[0][0].size();

        int I = block.influence.size();
        int S = block.shape.size();

        auto in_bounds = [&](int yy, int xx)
        {
            return (0 <= yy && yy < H && 0 <= xx && xx < W);
        };

        for (int r = 0; r < I; r++)
        {
            for (int c = 0; c < I; c++)
            {
                int yy = y + r - 3;
                int xx = x + c - 3;

                if (!in_bounds(yy, xx))
                    continue;

                if (block.influence[r][c] == CANTSET)
                    status[(int)color][yy][xx] = CANTSET;
                else if (block.influence[r][c] == ABLESET && status[(int)color][yy][xx] == BLANK)
                    status[(int)color][yy][xx] = ABLESET;
            }
        }

        // --- 相手盤面へ shape を適用 ---
        for (int opponent = 0; opponent < 2; opponent++)
        {
            if (opponent == (int)color)
                continue;

            for (int r = 0; r < S; r++)
            {
                for (int c = 0; c < S; c++)
                {
                    if (block.shape[r][c] != CANTSET)
                        continue;

                    int yy = y + r - 2;
                    int xx = x + c - 2;

                    if (!in_bounds(yy, xx))
                        continue;

                    status[opponent][yy][xx] = CANTSET;
                }
            }
        }

        // --- used_blocks に追加 ---
        player.used_blocks.push_back(block_id);

        // --- スコア加算 ---
        auto it = block_table.find(block_id);
        if (it != block_table.end())
        {
            player.score += it->second.score; // BlockData の score を加算
        }
    }

    void print_status(Color color)
    {
        int col = static_cast<int>(color);

        for (int y = 0; y < TILE_NUMBER + 2; y++)
        {
            for (int x = 0; x < TILE_NUMBER + 2; x++)
            {
                cout << status[col][y][x] << " ";
            }
            cout << endl;
        }
    }
};

// Player クラスを引数に取って合法手リストを返す関数
vector<tuple<string, int, int, int>> get_all_legal_moves(Board &board, Color player_color, Player &player)
{
    vector<tuple<string, int, int, int>> legal_moves;

    // 使用済みブロックを除いた未使用ブロックリストを作成
    vector<string> unused_blocks;
    for (auto &[id, _] : block_table)
    {
        if (find(player.used_blocks.begin(), player.used_blocks.end(), id) == player.used_blocks.end())
        {
            unused_blocks.push_back(id);
        }
    }

    // 未使用ブロックで合法手探索
    for (auto &block_id : unused_blocks)
    {
        BlockData data = getBlock(block_id);
        Block block(data);

        for (int rot = 0; rot < 8; ++rot)
        {
            Block tmp_block = block;
            tmp_block.rotate_block(rot);

            auto positions = board.search_settable_position(player_color, tmp_block.shape);

            for (auto &[x, y] : positions)
            {
                legal_moves.emplace_back(block_id, x, y, rot);
            }
        }
    }

    return legal_moves;
}

// Player クラスの used_blocks を考慮して合法手リストを取得（x,yを除く）
vector<pair<string, int>> get_legal_moves_no_pos(Board &board, Color player_color, Player &player)
{
    vector<pair<string, int>> legal_moves;

    for (auto &[id, _] : block_table)
    {
        // 使用済みブロックはスキップ
        if (find(player.used_blocks.begin(), player.used_blocks.end(), id) != player.used_blocks.end())
            continue;

        BlockData data = getBlock(id);
        Block block(data);

        for (int rot = 0; rot < 8; ++rot)
        {
            Block tmp_block = block;
            tmp_block.rotate_block(rot);

            // 置ける場所が1つでもあれば合法手とみなす
            auto positions = board.search_settable_position(player_color, tmp_block.shape);
            if (!positions.empty())
            {
                legal_moves.emplace_back(id, rot);
            }
        }
    }

    return legal_moves;
}

vector<string> get_legal_block_types(Board &board, Color player_color, Player &player)
{
    vector<string> legal_blocks;

    for (auto &[id, _] : block_table)
    {
        // 使用済みブロックはスキップ
        if (find(player.used_blocks.begin(), player.used_blocks.end(), id) != player.used_blocks.end())
            continue;

        BlockData data = getBlock(id);
        Block block(data);

        bool can_place = false;

        // 8方向の回転をチェック
        for (int rot = 0; rot < 8; ++rot)
        {
            Block tmp_block = block;
            tmp_block.rotate_block(rot);

            auto positions = board.search_settable_position(player_color, tmp_block.shape);
            if (!positions.empty())
            {
                can_place = true;
                break; // 1つでも置ければ十分
            }
        }

        if (can_place)
            legal_blocks.push_back(id);
    }

    return legal_blocks;
}

// ランダムプレイアウト関数
pair<int, int> random_playout(Board board, Player player1, Player player2)
{
    // プレイヤー順番
    Color current_color = Color::PLAYER1;

    std::random_device rd;
    std::mt19937 gen(rd());

    while (true)
    {
        Player *current_player = (current_color == Color::PLAYER1) ? &player1 : &player2;

        // 現在プレイヤーの合法手を取得
        auto legal_moves = get_all_legal_moves(board, current_color, *current_player);

        if (!legal_moves.empty())
        {
            // ランダムに1手選択
            std::uniform_int_distribution<> dis(0, legal_moves.size() - 1);
            int idx = dis(gen);

            auto [block_id, x, y, rot] = legal_moves[idx];
            BlockData data = getBlock(block_id);
            Block block(data);

            // 盤面を更新（スコアも更新される前提）
            board.change_status(current_color, block, block_id, rot, x, y, *current_player);
        }

        // 次のプレイヤーに交代
        current_color = (current_color == Color::PLAYER1) ? Color::PLAYER2 : Color::PLAYER1;

        // 両プレイヤーとも合法手がなくなった場合、終了
        auto legal_p1 = get_all_legal_moves(board, Color::PLAYER1, player1);
        auto legal_p2 = get_all_legal_moves(board, Color::PLAYER2, player2);
        if (legal_p1.empty() && legal_p2.empty())
            break;
    }

    // 最終スコアを返す
    return {player1.score, player2.score};
}

int main()
{
    // memo // player.used_blocks.push_back(block_id);

    const int TILE_NUMBER = 14;

    // --- 盤面初期化 ---
    vector<vector<vector<int>>> input_board = {
        // PLAYER1
        {
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 2, 1, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 2, 1, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}},
        // PLAYER2
        {
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 2, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 2, 1, 1, 1, 1, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 2, 1, 2, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}}};

    // --- Board生成 ---
    Board board(TILE_NUMBER, input_board);

    // --- プレイヤー初期化 ---
    Player player1{Color::PLAYER1, {"u"}};
    Player player2{Color::PLAYER2, {"s"}};

    // --- プレイヤー1の合法手取得 ---
    auto legal_moves_p1 = get_all_legal_moves(board, player1.color, player1);

    if (!legal_moves_p1.empty())
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, legal_moves_p1.size() - 1);
        int idx = dis(gen);

        auto [block_id, x, y, rot] = legal_moves_p1[idx];
        BlockData data = getBlock(block_id);
        Block block(data);

        board.change_status(player1.color, block, block_id, rot, x, y, player1);
        cout << "Board status (Player1):\n";
        board.print_status(player1.color);

        cout << "Player1 placed block " << block_id << " at (" << x << "," << y << ") with rotation " << rot << "\n";
    }
    else
    {
        cout << "Player1 has no legal moves.\n";
    }

    // --- プレイヤー2の合法手取得 ---
    auto legal_moves_p2 = get_all_legal_moves(board, player2.color, player2);

    if (!legal_moves_p2.empty())
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, legal_moves_p2.size() - 1);
        int idx = dis(gen);

        auto [block_id, x, y, rot] = legal_moves_p2[idx];
        BlockData data = getBlock(block_id);
        Block block(data);

        board.change_status(player2.color, block, block_id, rot, x, y, player2);

        cout << "Player2 placed block " << block_id << " at (" << x << "," << y << ") with rotation " << rot << "\n";
    }
    else
    {
        cout << "Player2 has no legal moves.\n";
    }

    // --- 盤面表示 ---
    cout << "Board status (Player2):\n";
    board.print_status(player2.color);

    // --- used_blocks 確認 ---
    cout << "Player1 used blocks: ";
    for (auto &bid : player1.used_blocks)
        cout << bid << " ";
    cout << endl;

    cout << "Player2 used blocks: ";
    for (auto &bid : player2.used_blocks)
        cout << bid << " ";
    cout << endl;

    return 0;
}
