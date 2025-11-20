
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
#include "block_data.h"
using std::array, std::vector;
using std::cout, std::endl, std::cin;
using std::deque;
using std::pair;
using std::swap;
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

class Board
{
public:
    enum
    {
        BLANK = 0,
        CANTSET = 1,
        ABLESET = 2
    };

    int TILE_NUMBER;
    vector<Color> COLOR_LIST;

    array<vector<vector<int>>, COLOR_NUM> status;

    // ---------- コンストラクタ（外部盤面を受け取る版） ----------
    Board(int tile_number, const vector<Color> &color_list, const array<vector<vector<int>>, COLOR_NUM> &input_board)
        : TILE_NUMBER(tile_number), COLOR_LIST(color_list)
    {
        status = input_board;
    }

    // ---------- settable_check ----------
    bool settable_check(Color color, const vector<vector<int>> &block_shape, int x, int y)
    {
        int col = static_cast<int>(color);
        int shape_h = block_shape.size();
        int shape_w = block_shape[0].size();

        for (int i = 0; i < shape_h; ++i)
        {
            for (int j = 0; j < shape_w; ++j)
            {
                if (block_shape[i][j] == CANTSET)
                {
                    int access_y = y + i - shape_h / 2;
                    int access_x = x + j - shape_w / 2;

                    // 盤面外なら置けない
                    if (access_y < 0 || access_y >= TILE_NUMBER + 2 ||
                        access_x < 0 || access_x >= TILE_NUMBER + 2)
                    {
                        // デバッグ出力
                        std::cout << "[settable_check] Out of range! i=" << i << " j=" << j
                                  << " access_y=" << access_y << " access_x=" << access_x << std::endl;
                        return false;
                    }

                    if (status[col][access_y][access_x] == CANTSET)
                        return false;
                }
            }
        }

        for (int i = 0; i < shape_h; ++i)
        {
            for (int j = 0; j < shape_w; ++j)
            {
                if (block_shape[i][j] == CANTSET)
                {
                    int access_y = y + i - shape_h / 2;
                    int access_x = x + j - shape_w / 2;

                    if (status[col][access_y][access_x] == ABLESET)
                        return true;
                }
            }
        }
        return false;
    }

    // ---------- search_settable_position ----------
    vector<pair<int, int>> search_settable_position(Color color, const vector<vector<int>> &block_shape)
    {
        vector<pair<int, int>> res;
        int col = static_cast<int>(color);

        int shape_h = block_shape.size();
        int shape_w = block_shape[0].size();

        // ブロックが盤面外にはみ出ない範囲でループ
        for (int x = 1; x <= TILE_NUMBER; ++x)
        {
            for (int y = 1; y <= TILE_NUMBER; ++y)
            {
                // 中心座標基準でチェック
                int access_top = y - shape_h / 2;
                int access_left = x - shape_w / 2;
                int access_bottom = y + (shape_h - 1) / 2;
                int access_right = x + (shape_w - 1) / 2;

                if (access_top < 0 || access_left < 0 || access_bottom >= TILE_NUMBER + 2 || access_right >= TILE_NUMBER + 2)
                    continue;

                if (status[col][y][x] != CANTSET)
                {
                    if (settable_check(color, block_shape, x, y))
                        res.push_back({x, y});
                }
            }
        }

        return res;
    }

    // ---------- change_status ----------
    void change_status(Color color,
                       const vector<vector<int>> &block_shape,
                       const vector<vector<int>> &block_influence,
                       int x, int y)
    {
        int col = static_cast<int>(color);

        // 自分のボードに影響を反映
        for (int i = 0; i < block_influence.size(); ++i)
        {
            for (int j = 0; j < block_influence[i].size(); ++j)
            {
                if (block_influence[i][j] == CANTSET)
                    status[col][y + i - 3][x + j - 3] = CANTSET;
                else if (block_influence[i][j] == ABLESET && status[col][y + i - 3][x + j - 3] == BLANK)
                    status[col][y + i - 3][x + j - 3] = ABLESET;
            }
        }

        // 他のプレイヤーのボードに影響
        for (int other = 0; other < COLOR_NUM; ++other)
        {
            if (other == col)
                continue;
            for (int i = 0; i < block_shape.size(); ++i)
            {
                for (int j = 0; j < block_shape[i].size(); ++j)
                {
                    if (block_shape[i][j] == CANTSET)
                        status[other][y + i - 2][x + j - 2] = CANTSET;
                }
            }
        }
    }
};

int main()
{
    // 定数
    int TILE_NUMBER = 5;
    array<vector<vector<int>>, COLOR_NUM> board_input;

    // 盤面初期化（全てBLANK + 外枠はCANTSET）
    for (int c = 0; c < COLOR_NUM; ++c)
    {
        board_input[c].assign(TILE_NUMBER + 2, vector<int>(TILE_NUMBER + 2, Board::BLANK));

        for (int i = 0; i < TILE_NUMBER + 2; ++i)
        {
            board_input[c][0][i] = Board::CANTSET;
            board_input[c][TILE_NUMBER + 1][i] = Board::CANTSET;
        }
        for (int i = 1; i <= TILE_NUMBER; ++i)
        {
            board_input[c][i][0] = Board::CANTSET;
            board_input[c][i][TILE_NUMBER + 1] = Board::CANTSET;
        }
    }

    // 初期置ける位置をセット（テスト用）
    board_input[0][1][1] = Board::ABLESET;                     // プレイヤー1
    board_input[1][TILE_NUMBER][TILE_NUMBER] = Board::ABLESET; // プレイヤー2

    // Board クラス生成
    vector<Color> colors = {Color::PLAYER1, Color::PLAYER2};
    Board board(TILE_NUMBER, colors, board_input);

    // テスト用ブロック (3x3 のL字型)
    vector<vector<int>> block_shape = {
        {Board::CANTSET, Board::BLANK, Board::BLANK},
        {Board::CANTSET, Board::CANTSET, Board::CANTSET},
        {Board::BLANK, Board::BLANK, Board::BLANK}};

    // settable_check テスト
    cout << "Player1, position (1,1): ";
    if (board.settable_check(Color::PLAYER1, block_shape, 1, 1))
        cout << "SETTABLE" << endl;
    else
        cout << "CANNOT SET" << endl;

    cout << "Player2, position (5,5): ";
    if (board.settable_check(Color::PLAYER2, block_shape, 5, 5))
        cout << "SETTABLE" << endl;
    else
        cout << "CANNOT SET" << endl;

    // search_settable_position テスト
    auto positions = board.search_settable_position(Color::PLAYER1, block_shape);
    cout << "Player1 possible positions:" << endl;
    for (auto [x, y] : positions)
    {
        cout << "(" << x << "," << y << ")" << endl;
    }

    return 0;
}
