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
#include "block_data.h"
#include <tuple>
// エイリアス(よくわからない人はAPG4bに従いusing namespace std;をしてください)
using std::array, std::vector;
using std::cout, std::endl, std::cin;
using std::deque;
using std::pair;
using std::swap;

constexpr int TILE_NUMBER = 14;
constexpr int BOARD_SIZE = TILE_NUMBER + 2; // 壁を含めたサイズ

enum TileState
{
    EMPTY = 0,
    CANTSET = -1,
    ABLESET = 1
};

struct Position
{
    int x;
    int y;
};

// 定数
constexpr int END_TURN = 100; // ゲーム終了ターン

// ブロック定義
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

// Block構造体
struct Block
{
    vector<vector<int>> shape;
    vector<vector<int>> influence;

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

extern "C"
{
    // 時間を管理するクラス（ゲームで学ぶ探索アルゴリズム実践入門のサンプルとは管理方法が異なる）
    class TimeKeeper
    {
    private:
        std::chrono::high_resolution_clock::time_point start_time_;
        std::chrono::high_resolution_clock::time_point before_time_;
        int64_t time_threshold_;
        int64_t end_turn_;
        int64_t turn_;

    public:
        // ターン制の問題で全ターン含めての制限時間（ミリ秒）と最大ターン数を指定し、
        // インスタンスをつくる。
        TimeKeeper(const int64_t &time_threshold, const int64_t end_turn)
            : start_time_(std::chrono::high_resolution_clock::now()),
              time_threshold_(time_threshold),
              end_turn_(end_turn),
              turn_(0)
        {
            // メンバイニシャライザで初期化されたstart_time_の値を使うため、before_time_はメンバイニシャライザではなくコピーをする
            before_time_ = start_time_;
        }

        // ターンとターン開始時間を更新する
        void setTurn(const int t)
        {
            turn_ = t;
            this->before_time_ = std::chrono::high_resolution_clock::now();
        }

        // 各ターンに割り振られた制限時間を超過したか判定する。
        bool isTimeOver()
        {
            auto now = std::chrono::high_resolution_clock::now();
            auto whole_diff = now - this->start_time_;
            auto whole_count =
                std::chrono::duration_cast<std::chrono::milliseconds>(whole_diff)
                    .count();
            auto last_diff = now - this->before_time_;
            auto last_count =
                std::chrono::duration_cast<std::chrono::milliseconds>(last_diff)
                    .count();

            auto remaining_time = time_threshold_ - whole_count;
            auto now_threshold = remaining_time / (end_turn_ - this->turn_);
            return last_count >= now_threshold;
        }
    };

    class Board
    {
        std::vector<std::vector<std::vector<int>>> status;
        bool settable_check(int color, const std::vector<std::vector<int>> &block_shape, int x, int y);
        std::vector<std::pair<int, int>> search_settable_position(int color, const Block &block);
    };

    class State
    {
    public:
        Board board;
        std::vector<Block> blocks;
        int current_player;

        // 現在のターン
        int t_ = 0;
        State() {}

        // ゲームが終了したか判定する
        bool isDone() const { return this->t_ >= END_TURN; }

        bool settable_check(
            const std::vector<std::vector<int>> &board,
            int color,
            const std::array<std::array<int, 5>, 5> &blockShape,
            int x,
            int y)
        {
            // 盤面範囲チェックを含めた合法判定
            bool canPlace = false;
            for (int dy = 0; dy < 5; ++dy)
            {
                for (int dx = 0; dx < 5; ++dx)
                {
                    if (blockShape[dy][dx] == 0)
                        continue;
                    int by = y + dy - 2;
                    int bx = x + dx - 2;
                    // 盤面外
                    if (by < 0 || bx < 0 || by >= BOARD_SIZE || bx >= BOARD_SIZE)
                        return false;
                    if (board[by][bx] == CANTSET)
                        return false;
                    if (board[by][bx] == ABLESET)
                        canPlace = true;
                }
            }
            return canPlace;
        }

        std::vector<Position> search_settable_position(
            const std::vector<std::vector<int>> &board,
            int color,
            const std::array<std::array<int, 5>, 5> &blockShape)
        {
            std::vector<Position> settable_positions;

            for (int y = 1; y <= TILE_NUMBER; ++y)
            {
                for (int x = 1; x <= TILE_NUMBER; ++x)
                {
                    if (board[y][x] != CANTSET &&
                        settable_check(board, color, blockShape, x, y))
                    {
                        settable_positions.push_back({x, y});
                    }
                }
            }
            return settable_positions;
        }

        struct MoveCandidate
        {
            std::string block_id;
            int rotation;
            std::vector<Position> positions;
        };

        std::vector<MoveCandidate> any_block_settable_check(
            const std::vector<std::vector<int>> &board,
            int color,
            const std::unordered_map<std::string, BlockData> &block_table)
        {
            std::vector<MoveCandidate> result;

            for (const auto &[block_name, data] : block_table)
            {
                for (int rot = 0; rot < 8; ++rot)
                { // 8方向の回転＋反転
                    auto rotated = rotate_block(data.shape, rot);
                    auto positions = search_settable_position(board, color, rotated);
                    if (!positions.empty())
                    {
                        result.push_back({block_name, rot, positions});
                    }
                }
            }
            return result;
        }

        // 指定したactionで1ターン進める
        void advance(const int action)
        {
            // 合法手から手を選び盤面を進める。
            ++this->t_; // ターンを進めるのを忘れずに(実装方針次第ではここで管理しなくてもよい)
        }

        // ランダムに盤面を更新する
        void randomUpdate()
        {
            int reverse_t = END_TURN - this->t_;
            int p = 1;
            if (reverse_t != 0)
            {
                p = mt() % reverse_t + 1;
            }
            update(p);
        }

        // 指定したパラメータで盤面を更新する
        void update(int pt)
        {
            // 盤面更新
        }

        // スコアを取得する
        double getScore() const
        {
            return score;
        }
    };

    // モンテカルロ法に関連する名前空間
    namespace montecarlo
    {

        // ランダムに行動を決定する
        int play1turn(const State &state)
        {
            return legal_actions[mt() % (legal_actions.size())];
        }

        // ランダムプレイアウトをして勝敗スコアを計算する
        double playout(
            State
                state // const&にすると再帰中にディープコピーが必要になるため、高速化のためポインタにする。(constでない参照でも可)
        )
        {
            state.advance(play1turn(state));
            int before_turn = state.t_;
            while (!state.isDone())
            {
                state.randomUpdate();
                state.advance(play1turn(state));
            }
            return state.getScore();
        }

        // 制限時間(ms)を指定して行動を決定する
        int primitiveMontecalro(TimeKeeper &time_keeper, const State base_state)
        {
            std::vector<double> w(legal_actions.size());
            for (int simulate_cnt = 0;; simulate_cnt++)
            {
                if (time_keeper.isTimeOver())
                {
                    break;
                }
                for (int d = 0; d < legal_actions.size(); d++)
                {
                    auto state =
                        base_state; // 現在の状況に戻す。参照ではなくコピーである点に注意
                    state.advance(d);
                    state.randomUpdate();
                    if (state.isDone())
                    {
                        w[d] += state.getScore();
                    }
                    else
                    {
                        w[d] += playout(state);
                    }
                }
            }
            int ret = -1;
            double best = -1;
            for (int d = 0; d < legal_actions.size(); d++)
            {
                if (w[d] > best)
                {
                    ret = d;
                    best = w[d];
                }
            }
            return ret;
        }
    } // namespace montecarlo

    int main()
    {
        // END_TURN分のキャンディの種類を入力から受け取る
        for (int t = 0; t < END_TURN; t++)
        {
            cin >> future_candies[t];
        }

        // メインループで使用するstateを初期化する
        State state{};

        // 制限時間を1950ミリ秒で設定する。（2秒ちょうどだとオーバーヘッドで超過するため）
        TimeKeeper time_keeper{1950, END_TURN};

        // 処理のメインループ
        for (int t = 0; t < END_TURN; t++)
        {
            // ターンの開始をtime_keeperに記録する
            time_keeper.setTurn(t);
            // 今回追加されるキャンディの位置を入力から受け取る
            int pt;
            cin >> pt;
            // 入力から受け取った位置にキャンディを追加する
            state.update(pt);
            // 今回の操作を考える
            int action = montecarlo::primitiveMontecalro(time_keeper, state);
            // 今回の操作を出力する
            cout << ACTION_CHARS[action] << endl;
            // 今回の操作によってstateを更新する
            state.advance(action);
        }
        return 0;
    }
}
