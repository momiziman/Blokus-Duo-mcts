
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

#include <vector>
#include <utility>
#include <iostream>

using std::cout;
using std::endl;
using std::pair;
using std::vector;

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

        for (int i = 0; i < block_shape.size(); ++i)
        {
            for (int j = 0; j < block_shape[i].size(); ++j)
            {
                int access_y = y + i - 2;
                int access_x = x + j - 2;

                // 範囲外アクセスはスキップ
                if (access_y < 0 || access_y >= TILE_NUMBER + 2 || access_x < 0 || access_x >= TILE_NUMBER + 2)
                    continue;

                if (block_shape[i][j] == CANTSET &&
                    status[col][access_y][access_x] == CANTSET)
                    return false;
            }
        }

        for (int i = 0; i < block_shape.size(); ++i)
        {
            for (int j = 0; j < block_shape[i].size(); ++j)
            {
                int access_y = y + i - 2;
                int access_x = x + j - 2;

                if (access_y < 0 || access_y >= TILE_NUMBER + 2 || access_x < 0 || access_x >= TILE_NUMBER + 2)
                    continue;

                if (block_shape[i][j] == CANTSET &&
                    status[col][access_y][access_x] == ABLESET)
                    return true;
            }
        }

        return false;
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
};

int main()
{
    const int TILE_NUMBER = 14;

    // 盤面データを作成（2色用）
    vector<vector<vector<int>>> input_board(2,
                                            vector<vector<int>>(TILE_NUMBER + 2, vector<int>(TILE_NUMBER + 2, Board::BLANK)));

    // 外枠をCANTSETに
    for (int col = 0; col < 2; ++col)
    {
        for (int i = 0; i < TILE_NUMBER + 2; ++i)
        {
            input_board[col][0][i] = Board::CANTSET;
            input_board[col][TILE_NUMBER + 1][i] = Board::CANTSET;
            input_board[col][i][0] = Board::CANTSET;
            input_board[col][i][TILE_NUMBER + 1] = Board::CANTSET;
        }
    }

    // スタート地点
    input_board[0][5][5] = Board::ABLESET;                             // PLAYER1
    input_board[1][TILE_NUMBER - 4][TILE_NUMBER - 4] = Board::ABLESET; // PLAYER2

    // Boardインスタンス生成（TILE_NUMBER と盤面だけ）
    Board board(TILE_NUMBER, input_board);

    // サンプルブロック（2x2のL字型）
    vector<vector<int>> block_shape = {
        {Board::CANTSET, Board::BLANK},
        {Board::CANTSET, Board::CANTSET}};

    // --- デバッグ出力: 盤面 ---
    for (int col = 0; col < 2; ++col)
    {
        cout << "Board for PLAYER" << col + 1 << ":" << endl;
        for (int y = 0; y < TILE_NUMBER + 2; ++y)
        {
            for (int x = 0; x < TILE_NUMBER + 2; ++x)
                cout << setw(2) << input_board[col][y][x] << " ";
            cout << endl;
        }
        cout << endl;
    }

    // --- デバッグ出力: ブロック ---
    cout << "Block shape:" << endl;
    for (int i = 0; i < block_shape.size(); ++i)
    {
        for (int j = 0; j < block_shape[i].size(); ++j)
            cout << setw(2) << block_shape[i][j] << " ";
        cout << endl;
    }
    cout << endl;

    // PLAYER1の合法手探索
    auto positions = board.search_settable_position(Color::PLAYER1, block_shape);
    cout << "PLAYER1 possible positions:" << endl;
    for (auto &p : positions)
    {
        cout << "(" << p.first << "," << p.second << ")" << endl;
    }

    // PLAYER2の合法手探索
    positions = board.search_settable_position(Color::PLAYER2, block_shape);
    cout << "PLAYER2 possible positions:" << endl;
    for (auto &p : positions)
    {
        cout << "(" << p.first << "," << p.second << ")" << endl;
    }

    return 0;
}
