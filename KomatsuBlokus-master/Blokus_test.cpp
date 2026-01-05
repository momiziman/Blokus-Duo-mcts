
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
#include "block_data.h"
#include <algorithm>
#include <array>
#include <bits/stdc++.h>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <queue>
#include <random>
#include <stdio.h>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

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
    {"a",
     {{{{0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0}}},
      // influence (7x7)
      {{{0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 2, 1, 2, 0, 0},
        {0, 0, 1, 1, 1, 0, 0},
        {0, 0, 2, 1, 2, 0, 0},
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0}}},
      1}}, // score
    {"b",
     {{{{0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0}}},
      // influence (7x7)
      {{{0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 2, 1, 2, 0, 0},
        {0, 0, 1, 1, 1, 0, 0},
        {0, 0, 1, 1, 1, 0, 0},
        {0, 0, 2, 1, 2, 0, 0},
        {0, 0, 0, 0, 0, 0, 0}}},
      2}}, // score
    {"c",
     {{{{0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0}}},
      // influence (7x7)
      {{{0, 0, 0, 0, 0, 0, 0},
        {0, 0, 2, 1, 2, 0, 0},
        {0, 0, 1, 1, 1, 0, 0},
        {0, 0, 1, 1, 1, 0, 0},
        {0, 0, 1, 1, 1, 0, 0},
        {0, 0, 2, 1, 2, 0, 0},
        {0, 0, 0, 0, 0, 0, 0}}},
      3}}, // score
    {"d",
     {{{{0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 1, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0}}},
      // influence (7x7)
      {{{0, 0, 0, 0, 0, 0, 0},
        {0, 0, 2, 1, 2, 0, 0},
        {0, 0, 1, 1, 1, 2, 0},
        {0, 0, 1, 1, 1, 1, 0},
        {0, 0, 2, 1, 1, 2, 0},
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0}}},
      3}}, // score
    {"e",
     {{{{0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0}}},
      // influence (7x7)
      {{{0, 0, 0, 0, 0, 0, 0},
        {0, 0, 2, 1, 2, 0, 0},
        {0, 0, 1, 1, 1, 0, 0},
        {0, 0, 1, 1, 1, 0, 0},
        {0, 0, 1, 1, 1, 0, 0},
        {0, 0, 1, 1, 1, 0, 0},
        {0, 0, 2, 1, 2, 0, 0}}},
      4}}, // score
    {"f",
     {{{{0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 1, 1, 0, 0},
        {0, 0, 0, 0, 0}}},
      // influence (7x7)
      {{{0, 0, 0, 0, 0, 0, 0},
        {0, 0, 2, 1, 2, 0, 0},
        {0, 0, 1, 1, 1, 0, 0},
        {0, 2, 1, 1, 1, 0, 0},
        {0, 1, 1, 1, 1, 0, 0},
        {0, 2, 1, 1, 2, 0, 0},
        {0, 0, 0, 0, 0, 0, 0}}},
      4}}, // score
    {"g",
     {{{{0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 1, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0}}},
      // influence (7x7)
      {{{0, 0, 0, 0, 0, 0, 0},
        {0, 0, 2, 1, 2, 0, 0},
        {0, 0, 1, 1, 1, 2, 0},
        {0, 0, 1, 1, 1, 1, 0},
        {0, 0, 1, 1, 1, 2, 0},
        {0, 0, 2, 1, 2, 0, 0},
        {0, 0, 0, 0, 0, 0, 0}}},
      4}}, // score
    {"h",
     {{{{0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 1, 1, 0},
        {0, 0, 1, 1, 0},
        {0, 0, 0, 0, 0}}},
      // influence (7x7)
      {{{0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 2, 1, 1, 2, 0},
        {0, 0, 1, 1, 1, 1, 0},
        {0, 0, 1, 1, 1, 1, 0},
        {0, 0, 2, 1, 1, 2, 0},
        {0, 0, 0, 0, 0, 0, 0}}},
      4}}, // score
    {"i",
     {{{{0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 1, 1, 0, 0},
        {0, 0, 1, 1, 0},
        {0, 0, 0, 0, 0}}},
      // influence (7x7)
      {{{0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0},
        {0, 2, 1, 1, 2, 0, 0},
        {0, 1, 1, 1, 1, 2, 0},
        {0, 2, 1, 1, 1, 1, 0},
        {0, 0, 2, 1, 1, 2, 0},
        {0, 0, 0, 0, 0, 0, 0}}},
      4}}, // score
    {"j",
     {{{{0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0}}},
      // influence (7x7)
      {{{0, 0, 2, 1, 2, 0, 0},
        {0, 0, 1, 1, 1, 0, 0},
        {0, 0, 1, 1, 1, 0, 0},
        {0, 0, 1, 1, 1, 0, 0},
        {0, 0, 1, 1, 1, 0, 0},
        {0, 0, 1, 1, 1, 0, 0},
        {0, 0, 2, 1, 2, 0, 0}}},
      5}}, // score
    {"k",
     {{{{0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 1, 1, 0, 0},
        {0, 0, 0, 0, 0}}},
      // influence (7x7)
      {{{0, 0, 2, 1, 2, 0, 0},
        {0, 0, 1, 1, 1, 0, 0},
        {0, 0, 1, 1, 1, 0, 0},
        {0, 2, 1, 1, 1, 0, 0},
        {0, 1, 1, 1, 1, 0, 0},
        {0, 2, 1, 1, 2, 0, 0},
        {0, 0, 0, 0, 0, 0, 0}}},
      5}}, // score
    {"l",
     {{{{0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 1, 1, 0, 0},
        {0, 1, 0, 0, 0},
        {0, 0, 0, 0, 0}}},
      // influence (7x7)
      {{{0, 0, 2, 1, 2, 0, 0},
        {0, 0, 1, 1, 1, 0, 0},
        {0, 2, 1, 1, 1, 0, 0},
        {0, 1, 1, 1, 1, 0, 0},
        {0, 1, 1, 1, 2, 0, 0},
        {0, 2, 1, 2, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0}}},
      5}}, // score
    {"m",
     {{{{0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 1, 1, 0, 0},
        {0, 1, 1, 0, 0},
        {0, 0, 0, 0, 0}}},
      // influence (7x7)
      {{{0, 0, 0, 0, 0, 0, 0},
        {0, 0, 2, 1, 2, 0, 0},
        {0, 2, 1, 1, 1, 0, 0},
        {0, 1, 1, 1, 1, 0, 0},
        {0, 1, 1, 1, 1, 0, 0},
        {0, 2, 1, 1, 2, 0, 0},
        {0, 0, 0, 0, 0, 0, 0}}},
      5}}, // score
    {"n",
     {{{{0, 0, 0, 0, 0},
        {0, 1, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 1, 1, 0, 0},
        {0, 0, 0, 0, 0}}},
      // influence (7x7)
      {{{0, 0, 0, 0, 0, 0, 0},
        {0, 2, 1, 1, 2, 0, 0},
        {0, 1, 1, 1, 1, 0, 0},
        {0, 2, 1, 1, 1, 0, 0},
        {0, 1, 1, 1, 1, 0, 0},
        {0, 2, 1, 1, 2, 0, 0},
        {0, 0, 0, 0, 0, 0, 0}}},
      5}}, // score
    {"o",
     {{{{0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 1, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0}}},
      // influence (7x7)
      {{{0, 0, 0, 0, 0, 0, 0},
        {0, 0, 2, 1, 2, 0, 0},
        {0, 0, 1, 1, 1, 2, 0},
        {0, 0, 1, 1, 1, 1, 0},
        {0, 0, 1, 1, 1, 2, 0},
        {0, 0, 1, 1, 1, 0, 0},
        {0, 0, 2, 1, 2, 0, 0}}},
      5}}, // score
    {"p",
     {{{{0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 1, 1, 1, 0},
        {0, 0, 0, 0, 0}}},
      // influence (7x7)
      {{{0, 0, 0, 0, 0, 0, 0},
        {0, 0, 2, 1, 2, 0, 0},
        {0, 0, 1, 1, 1, 0, 0},
        {0, 2, 1, 1, 1, 2, 0},
        {0, 1, 1, 1, 1, 1, 0},
        {0, 2, 1, 1, 1, 2, 0},
        {0, 0, 0, 0, 0, 0, 0}}},
      5}}, // score
    {"q",
     {{{{0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 1, 1},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0}}},
      // influence (7x7)
      {{{0, 0, 2, 1, 2, 0, 0},
        {0, 0, 1, 1, 1, 0, 0},
        {0, 0, 1, 1, 1, 1, 2},
        {0, 0, 1, 1, 1, 1, 1},
        {0, 0, 2, 1, 1, 1, 2},
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0}}},
      5}}, // score
    {"r",
     {{{{0, 0, 0, 0, 0},
        {0, 1, 1, 0, 0},
        {0, 0, 1, 1, 0},
        {0, 0, 0, 1, 0},
        {0, 0, 0, 0, 0}}},
      // influence (7x7)
      {{{0, 0, 0, 0, 0, 0, 0},
        {0, 2, 1, 1, 2, 0, 0},
        {0, 1, 1, 1, 1, 2, 0},
        {0, 2, 1, 1, 1, 1, 0},
        {0, 0, 2, 1, 1, 1, 0},
        {0, 0, 0, 2, 1, 2, 0},
        {0, 0, 0, 0, 0, 0, 0}}},
      5}}, // score
    {"s",
     {{{{0, 0, 0, 0, 0},
        {0, 1, 0, 0, 0},
        {0, 1, 1, 1, 0},
        {0, 0, 0, 1, 0},
        {0, 0, 0, 0, 0}}},
      // influence (7x7)
      {{{0, 0, 0, 0, 0, 0, 0},
        {0, 2, 1, 2, 0, 0, 0},
        {0, 1, 1, 1, 1, 2, 0},
        {0, 1, 1, 1, 1, 1, 0},
        {0, 2, 1, 1, 1, 1, 0},
        {0, 0, 0, 2, 1, 2, 0},
        {0, 0, 0, 0, 0, 0, 0}}},
      5}}, // score
    {"t",
     {{{{0, 0, 0, 0, 0},
        {0, 1, 0, 0, 0},
        {0, 1, 1, 1, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0}}},
      // influence (7x7)
      {{{0, 0, 0, 0, 0, 0, 0},
        {0, 2, 1, 2, 0, 0, 0},
        {0, 1, 1, 1, 1, 2, 0},
        {0, 1, 1, 1, 1, 1, 0},
        {0, 2, 1, 1, 1, 2, 0},
        {0, 0, 2, 1, 2, 0, 0},
        {0, 0, 0, 0, 0, 0, 0}}},
      5}}, // score
    {"u",
     {{{{0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 1, 1, 1, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0}}},
      // influence (7x7)
      {{{0, 0, 0, 0, 0, 0, 0},
        {0, 0, 2, 1, 2, 0, 0},
        {0, 2, 1, 1, 1, 2, 0},
        {0, 1, 1, 1, 1, 1, 0},
        {0, 2, 1, 1, 1, 2, 0},
        {0, 0, 2, 1, 2, 0, 0},
        {0, 0, 0, 0, 0, 0, 0}}},
      5}}, // score

};

constexpr int TILE_NUMBER = 14;
constexpr int COLOR_NUM = 2;
constexpr int BOARD_SIZE = TILE_NUMBER + 2; // 壁を含めたサイズ

enum TileState {
  BLANK = 0,
  CANTSET = 1,
  ABLESET = 2,
  MYBLOCK = 3,
  OPBLOCK = 4
};

enum class Color { PLAYER1 = 0, PLAYER2 = 1 };

struct Position {
  int x;
  int y;
};

struct MCTSNode;                           // MCTSNode を前方宣言
void log_node_basic(const MCTSNode *node); // 関数の前方宣言

std::vector<std::string> BLOCK_IDS_BY_SIZE;

void init_block_ids_by_size() {
  BLOCK_IDS_BY_SIZE.clear();

  for (auto &[id, data] : block_table) {
    BLOCK_IDS_BY_SIZE.push_back(id);
  }

  std::sort(BLOCK_IDS_BY_SIZE.begin(), BLOCK_IDS_BY_SIZE.end(),
            [&](const std::string &a, const std::string &b) {
              return block_table.at(a).score > block_table.at(b).score;
            });
}

struct Block {
  vector<vector<int>> shape;
  vector<vector<int>> influence;

  Block(const BlockData &data) {
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
  vector<vector<int>> transpose(const vector<vector<int>> &mat) {
    size_t rows = mat.size();
    size_t cols = mat[0].size();
    vector<vector<int>> res(cols, vector<int>(rows));
    for (size_t i = 0; i < rows; ++i)
      for (size_t j = 0; j < cols; ++j)
        res[j][i] = mat[i][j];
    return res;
  }

  // 2次元ベクトルを90°回転（時計回りにk回）
  vector<vector<int>> rot90(const vector<vector<int>> &mat, int k) {
    k = ((k % 4) + 4) % 4; // 安全に0〜3に正規化
    vector<vector<int>> res = mat;
    for (int i = 0; i < k; ++i) {
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

  void rotate_block(int dir) {
    switch (dir) {
    case 0: // 初期向き
      break;

    case 1:                               // 裏向き
      shape = rot90(transpose(shape), 1); // Pythonでは -1
      influence = rot90(transpose(influence), 1);
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
      shape = rot90(transpose(shape), 3);
      influence = rot90(transpose(influence), 3);
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

BlockData getBlock(const std::string &id) {
  auto it = block_table.find(id);
  if (it == block_table.end()) {
    throw std::runtime_error("Error: block id '" + id + "' not found!");
  }
  return it->second;
}

struct Player {
  Color color;
  vector<string> used_blocks; // 使用済みブロックIDを記録
  int score = 0;
};

class Board {
public:
  inline static const int BLANK = 0;
  inline static const int CANTSET = 1;
  inline static const int ABLESET = 2;
  inline static const int BLOCK = 3;
  inline static const int OPBLOCK = 4;

  int TILE_NUMBER;

  // status[color][y][x] の形で入力される
  vector<vector<vector<int>>>
      status; // [COLOR_NUM][TILE_NUMBER+2][TILE_NUMBER+2]

  Board(int tile_number, const vector<vector<vector<int>>> &input_board)
      : TILE_NUMBER(tile_number), status(input_board) {}

  // ---------- settable_check ----------
  bool settable_check(Color color, const vector<vector<int>> &block_shape,
                      int x, int y) {
    int col = static_cast<int>(color);
    bool found_corner = false;

    int H = block_shape.size();
    int W = block_shape[0].size();

    for (int i = 0; i < H; ++i) {
      for (int j = 0; j < W; ++j) {
        if (block_shape[i][j] != CANTSET)
          continue;

        int access_y = y + i - 2;
        int access_x = x + j - 2;

        if (access_y < 0 || access_y >= TILE_NUMBER + 2 || access_x < 0 ||
            access_x >= TILE_NUMBER + 2)
          continue;

        int cell = status[col][access_y][access_x];

        // 自分ブロック / OPBLOCK / CANTSET は置けない
        if (cell == MYBLOCK || cell == CANTSET || cell == OPBLOCK)
          return false;

        if (cell == ABLESET)
          found_corner = true; // 角接触
      }
    }

    return found_corner;
  }

  // ---------- 合法手探索 ----------
  vector<pair<int, int>>
  search_settable_position(Color color,
                           const vector<vector<int>> &block_shape) {
    vector<pair<int, int>> res;
    int col = static_cast<int>(color);

    for (int y = 1; y <= TILE_NUMBER; ++y) {
      for (int x = 1; x <= TILE_NUMBER; ++x) {
        if (status[col][y][x] != CANTSET) {
          if (settable_check(color, block_shape, x, y))
            res.push_back({x, y});
        }
      }
    }

    return res;
  }

  vector<pair<int, int>> search_settable_position_near_ableset(
      Color color, const vector<vector<int>> &block_shape) {
    vector<pair<int, int>> res;
    int col = static_cast<int>(color);

    int H = block_shape.size();
    int W = block_shape[0].size();

    std::set<pair<int, int>> candidates; // 重複防止

    // --- ABLESET を起点に候補座標を収集 ---
    for (int y = 1; y <= TILE_NUMBER; ++y) {
      for (int x = 1; x <= TILE_NUMBER; ++x) {

        if (status[col][y][x] != ABLESET)
          continue;

        // 周囲5マス（中心含む）
        for (int dy = -2; dy <= 2; ++dy) {
          for (int dx = -2; dx <= 2; ++dx) {
            int ny = y + dy;
            int nx = x + dx;

            if (1 <= ny && ny <= TILE_NUMBER && 1 <= nx && nx <= TILE_NUMBER) {
              candidates.insert({nx, ny});
            }
          }
        }
      }
    }

    // --- 実際に置けるかチェック ---
    for (auto &[x, y] : candidates) {
      if (status[col][y][x] != CANTSET) {
        if (settable_check(color, block_shape, x, y)) {
          res.push_back({x, y});
        }
      }
    }

    return res;
  }

  // Boardクラス内
  void change_status(Color color, Block &block, const std::string &block_id,
                     int rotation, int x, int y, Player &player) {
    block.rotate_block(rotation);

    int col = static_cast<int>(color);
    int opp_col = 1 - col; // 相手の色
    int H = status[0].size();
    int W = status[0][0].size();

    auto in_bounds = [&](int yy, int xx) {
      return (0 <= yy && yy < H && 0 <= xx && xx < W);
    };

    // --- 自分の盤面に shape を MYBLOCK として反映 ---
    int S = block.shape.size();
    for (int r = 0; r < S; r++) {
      for (int c = 0; c < S; c++) {
        if (block.shape[r][c] != CANTSET)
          continue;

        int yy = y + r - 2;
        int xx = x + c - 2;
        if (!in_bounds(yy, xx))
          continue;

        status[col][yy][xx] = MYBLOCK;
      }
    }

    // --- 自分の盤面に influence を適用（ABLESET / CANTSET） ---
    int I = block.influence.size();
    for (int r = 0; r < I; r++) {
      for (int c = 0; c < I; c++) {
        int yy = y + r - 3;
        int xx = x + c - 3;
        if (!in_bounds(yy, xx))
          continue;

        // 既に MYBLOCK / OPBLOCK がある場合は上書きしない
        if (status[col][yy][xx] == MYBLOCK || status[col][yy][xx] == OPBLOCK)
          continue;

        if (block.influence[r][c] == CANTSET)
          status[col][yy][xx] = CANTSET;
        else if (block.influence[r][c] == ABLESET &&
                 status[col][yy][xx] == BLANK)
          status[col][yy][xx] = ABLESET;
      }
    }

    // --- 相手の盤面に MYBLOCK を OPBLOCK として反映 ---
    for (int r = 0; r < S; r++) {
      for (int c = 0; c < S; c++) {
        if (block.shape[r][c] != CANTSET)
          continue;

        int yy = y + r - 2;
        int xx = x + c - 2;
        if (!in_bounds(yy, xx))
          continue;

        // 相手盤面でも既に OPBLOCK があれば上書きしない
        if (status[opp_col][yy][xx] != OPBLOCK)
          status[opp_col][yy][xx] = OPBLOCK;
      }
    }

    // --- used_blocks に追加 ---
    player.used_blocks.push_back(block_id);

    // --- スコア加算 ---
    auto it = block_table.find(block_id);
    if (it != block_table.end())
      player.score += it->second.score;
  }

  void print_status(Color color) {
    int col = static_cast<int>(color);

    for (int y = 0; y < TILE_NUMBER + 2; y++) {
      for (int x = 0; x < TILE_NUMBER + 2; x++) {
        int cell = status[col][y][x];
        switch (cell) {
        case 0: // BLANK
          cout << ". ";
          break;
        case 1: // CANTSET
          cout << "# ";
          break;
        case 2: // ABLESET
          cout << "* ";
          break;
        case 3: // MYBLOCK
          cout << "\033[32m"
               << "3 "
               << "\033[0m"; // 緑
          break;
        case 4: // OPBLOCK
          cout << "\033[31m"
               << "4 "
               << "\033[0m"; // 赤
          break;
        default:
          cout << cell << " ";
          break;
        }
      }
      cout << endl;
    }
  }
};

// Player クラスを引数に取って合法手リストを返す関数
vector<tuple<string, int, int, int>>
get_all_legal_moves(Board &board, Color player_color, Player &player) {
  vector<tuple<string, int, int, int>> legal_moves;

  // 使用済みブロックを除いた未使用ブロックリストを作成
  vector<string> unused_blocks;
  for (auto &[id, _] : block_table) {
    if (find(player.used_blocks.begin(), player.used_blocks.end(), id) ==
        player.used_blocks.end()) {
      unused_blocks.push_back(id);
    }
  }

  // 未使用ブロックで合法手探索
  for (auto &block_id : unused_blocks) {
    BlockData data = getBlock(block_id);
    Block block(data);

    for (int rot = 0; rot < 8; ++rot) {
      Block tmp_block = block;
      tmp_block.rotate_block(rot);

      auto positions = board.search_settable_position_near_ableset(
          player_color, tmp_block.shape);
      // board.search_settable_position_near_ableset(player_color,tmp_block.shape);

      for (auto &[x, y] : positions) {
        legal_moves.emplace_back(block_id, x, y, rot);
      }
    }
  }

  return legal_moves;
}

// Player クラスの used_blocks を考慮して合法手リストを取得（x,yを除く）
vector<pair<string, int>>
get_legal_moves_no_pos(Board &board, Color player_color, Player &player) {
  vector<pair<string, int>> legal_moves;

  for (auto &[id, _] : block_table) {
    // 使用済みブロックはスキップ
    if (find(player.used_blocks.begin(), player.used_blocks.end(), id) !=
        player.used_blocks.end())
      continue;

    BlockData data = getBlock(id);
    Block block(data);

    for (int rot = 0; rot < 8; ++rot) {
      Block tmp_block = block;
      tmp_block.rotate_block(rot);

      // 置ける場所が1つでもあれば合法手とみなす
      auto positions =
          board.search_settable_position(player_color, tmp_block.shape);
      if (!positions.empty()) {
        legal_moves.emplace_back(id, rot);
      }
    }
  }

  return legal_moves;
}

vector<string> get_legal_block_types(Board &board, Color player_color,
                                     Player &player) {
  vector<string> legal_blocks;

  for (auto &[id, _] : block_table) {
    // 使用済みブロックはスキップ
    if (find(player.used_blocks.begin(), player.used_blocks.end(), id) !=
        player.used_blocks.end())
      continue;

    BlockData data = getBlock(id);
    Block block(data);

    bool can_place = false;

    // 8方向の回転をチェック
    for (int rot = 0; rot < 8; ++rot) {
      Block tmp_block = block;
      tmp_block.rotate_block(rot);

      auto positions =
          board.search_settable_position(player_color, tmp_block.shape);
      if (!positions.empty()) {
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
pair<int, int> random_playout(Board board, Player player1,
                              Player player2) // MCTS時は値渡し（コピー）でOK
{
  // プレイヤー順番
  Color current_color = Color::PLAYER1;

  static thread_local std::mt19937 gen((std::random_device())());

  while (true) {
    Player *current_player =
        (current_color == Color::PLAYER1) ? &player1 : &player2;

    // 現在プレイヤーの合法手を取得（コピーされた board/player を使う）
    auto legal_moves =
        get_all_legal_moves(board, current_color, *current_player);

    if (!legal_moves.empty()) {
      // ランダムに1手選択
      std::uniform_int_distribution<> dis(0, (int)legal_moves.size() - 1);
      int idx = dis(gen);

      auto [block_id, x, y, rot] = legal_moves[idx];
      BlockData data = getBlock(block_id);
      Block block(data);

      // 盤面を更新（スコアも更新される）
      board.change_status(current_color, block, block_id, rot, x, y,
                          *current_player);
    }

    // 次のプレイヤーに交代
    current_color =
        (current_color == Color::PLAYER1) ? Color::PLAYER2 : Color::PLAYER1;

    // 両プレイヤーとも合法手がなくなった場合、終了
    auto legal_p1 = get_all_legal_moves(board, Color::PLAYER1, player1);
    auto legal_p2 = get_all_legal_moves(board, Color::PLAYER2, player2);
    if (legal_p1.empty() && legal_p2.empty())
      break;
  }

  // 最終スコアを返す（コピーされた player1/player2 の score を返す）
  return {player1.score, player2.score};
}

struct MCTSNode {
  Board board;
  Player player1;
  Player player2;
  Color current_player;

  MCTSNode *parent;
  std::vector<MCTSNode *> children;

  int visit_count;
  double win_score;

  int depth;

  std::vector<std::tuple<std::string, int, int, int>> untried_moves;

  // このノードへの着手
  std::string move_block_id;
  int move_x, move_y, move_rot;

  MCTSNode(const Board &b, const Player &p1, const Player &p2, Color turn,
           MCTSNode *parent = nullptr)
      : board(b), player1(p1), player2(p2), current_player(turn),
        parent(parent), visit_count(0), win_score(0.0), move_x(-1), move_y(-1),
        move_rot(0), move_block_id("") {
    depth = (parent == nullptr) ? 0 : parent->depth + 1;
  }

  // --- Selection: UCB1 で子ノード選択 ---
  MCTSNode *select_child() {
    const double C = 1.41421356;
    double best_value = -1e18;
    MCTSNode *best = nullptr;

    for (auto child : children) {
      double ucb = (child->win_score / (child->visit_count + 1e-6)) +
                   C * std::sqrt(std::log(visit_count + 1) /
                                 (child->visit_count + 1e-6));

      if (ucb > best_value) {
        best_value = ucb;
        best = child;
      }
    }
    return best;
  }

  // --- Expansion: 未展開手から子ノードを生成 ---
  MCTSNode *expand_node() {
    if (untried_moves.empty()) {

      return this;
    }

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, (int)untried_moves.size() - 1);

    int idx = dis(gen);
    auto [block_id, x, y, rot] = untried_moves[idx];

    // 選んだ手を未展開リストから削除
    untried_moves.erase(untried_moves.begin() + idx);

    // 次ノード用の盤面/プレイヤー状態をコピー
    Board next_board = board;
    Player next_p1 = player1;
    Player next_p2 = player2;

    Player *cur = (current_player == Color::PLAYER1) ? &next_p1 : &next_p2;

    // 盤面更新（next_board に対して）
    BlockData data = getBlock(block_id);
    Block blk(data);
    next_board.change_status(current_player, blk, block_id, rot, x, y, *cur);

    // 次ターンを決定
    Color next_turn =
        (current_player == Color::PLAYER1) ? Color::PLAYER2 : Color::PLAYER1;

    // 子ノードを生成（parent=this）
    MCTSNode *child =
        new MCTSNode(next_board, next_p1, next_p2, next_turn, this);

    // 子ノードの着手情報を設定
    child->move_block_id = block_id;
    child->move_x = x;
    child->move_y = y;
    child->move_rot = rot;

    // 子を追加
    children.push_back(child);

    // 子ノードの未展開手（合法手）を計算して設定する
    Player *next_player =
        (next_turn == Color::PLAYER1) ? &child->player1 : &child->player2;
    child->untried_moves =
        get_all_legal_moves(child->board, next_turn, *next_player);

    return child;
  }

  // --- Simulation: playout の呼び出し ---
  double simulate() {

    Board sim_board = board;
    Player sim_p1 = player1;
    Player sim_p2 = player2;

    auto [score1, score2] = random_playout(sim_board, sim_p1, sim_p2);

    double result = (score1 > score2) ? 1.0 : (score1 == score2 ? 0.5 : 0.0);

    return result;
  }

  double fast_simulation(Board board, Player p1, Player p2, Color start_turn,
                         int max_steps = 30) {
    static thread_local std::mt19937 gen((std::random_device())());

    int pass_count = 0;

    Color turn = start_turn;

    for (int step = 0; step < max_steps; step++) {

      Player *cur = (turn == Color::PLAYER1) ? &p1 : &p2;

      // --- 未使用ブロック候補 ---
      std::vector<std::string> candidates;
      for (auto &id : BLOCK_IDS_BY_SIZE) {
        if (std::find(cur->used_blocks.begin(), cur->used_blocks.end(), id) ==
            cur->used_blocks.end()) {
          candidates.push_back(id);
        }
      }

      if (candidates.empty())
        break;

      bool placed = false;
      /* ====== ① ランダム試行 ====== */
      for (auto &block_id : candidates) {
        Block block(getBlock(block_id));

        std::array<int, 8> rots{0, 1, 2, 3, 4, 5, 6, 7};
        std::shuffle(rots.begin(), rots.end(), gen);

        for (int rot : rots) {
          Block tmp = block;
          tmp.rotate_block(rot);

          for (int trial = 0; trial < 5; trial++) {
            int x = gen() % board.TILE_NUMBER + 1;
            int y = gen() % board.TILE_NUMBER + 1;

            if (board.settable_check(turn, tmp.shape, x, y)) {
              board.change_status(turn, tmp, block_id, rot, x, y, *cur);
              placed = true;
              break;
            }
          }
          if (placed)
            break;
        }
        if (placed)
          break;
      }

      /* ====== ② 保険探索　====== */
      if (!placed) {
        for (auto &block_id : candidates) {
          Block block(getBlock(block_id));

          for (int rot = 0; rot < 8; rot++) {
            Block tmp = block;
            tmp.rotate_block(rot);

            // 盤面から既存ブロック近傍をランダムに10点抽出
            for (int trial = 0; trial < 10; trial++) {
              int x = 1 + gen() % board.TILE_NUMBER;
              int y = 1 + gen() % board.TILE_NUMBER;

              if (board.settable_check(turn, tmp.shape, x, y)) {
                board.change_status(turn, tmp, block_id, rot, x, y, *cur);
                placed = true;
                break;
              }
            }
            if (placed)
              break;
          }
          if (placed)
            break;
        }
      }

      /* ====== ③ パス判定ログ ====== */
      if (!placed) {
        pass_count++;

        /* std::cout << "[FAST_SIM] PASS: "
                  << ((turn == Color::PLAYER1) ? "PLAYER1" : "PLAYER2")
                  << " (step=" << step << ", candidates=" << candidates.size()
                  << ", pass_count=" << pass_count << ")\n"; */
      } else {
        pass_count = 0;
      }

      if (pass_count >= 2) {
        // std::cout << "[FAST_SIM] END: consecutive pass\n";
        break;
      }

      turn = (turn == Color::PLAYER1) ? Color::PLAYER2 : Color::PLAYER1;

      // どのブロックも置けなければパス
      turn = (turn == Color::PLAYER1) ? Color::PLAYER2 : Color::PLAYER1;
    }

    // --- 評価（超軽量） ---
    return (p1.score > p2.score) ? 1.0 : (p1.score == p2.score) ? 0.5 : 0.0;
  }

  // --- Backpropagation ---
  void backpropagate(double result) {

    MCTSNode *node = this;
    while (node != nullptr) {

      node->visit_count++;
      node->win_score += result;

      node = node->parent;
    }
  }
};

void delete_subtree(MCTSNode *node) {
  if (!node)
    return;
  for (auto child : node->children) {
    delete_subtree(child);
  }
  delete node;
}

// ============================
// MCTS 本体
// ============================
std::tuple<std::string, int, int, int> MCTS(Board root_board, Player root_p1,
                                            Player root_p2, Color root_turn,
                                            int iterations,
                                            int MAX_TREE_DEPTH) {
  // --- ルートノード作成 ---
  MCTSNode *root = new MCTSNode(root_board, root_p1, root_p2, root_turn);

  // ルートの未展開手のセット
  if (root_turn == Color::PLAYER1)
    root->untried_moves =
        get_all_legal_moves(root_board, Color::PLAYER1, root_p1);
  else
    root->untried_moves =
        get_all_legal_moves(root_board, Color::PLAYER2, root_p2);

  if (root->untried_moves.empty()) {
    std::cout << "[MCTS] No moves available.\n";
    return {"", -1, -1, 0};
  }

  while (!root->untried_moves.empty()) {
    root->expand_node();
  }

  std::random_device rd;
  std::mt19937 gen(rd());

  // ============================
  // MCTSループ
  // ============================
  for (int iter = 0; iter < iterations; iter++) {

    MCTSNode *node = root;
    // 1. Selection
    cout << "[MCTS] Iteration " << iter + 1 << "/" << iterations << "\n";
    while (node->depth < MAX_TREE_DEPTH && node->untried_moves.empty() &&
           !node->children.empty()) {
      node = node->select_child();
    }

    // cout << "[MCTS] Expansion phase.\n";
    // 2. Expansion
    if (node->depth < MAX_TREE_DEPTH && !node->untried_moves.empty()) {
      node = node->expand_node();
    }

    // cout << "[MCTS] Simulation phase.\n";
    // 3. Simulation
    double result = node->fast_simulation(node->board, node->player1,
                                          node->player2, node->current_player);

    // cout << "[MCTS] Backpropagation phase.\n";
    // 4. Backpropagation
    node->backpropagate(result);
  }

  // ============================
  // 最良手を選択
  // ============================
  MCTSNode *best_child = nullptr;
  int best_visit = -1;

  cout << "[MCTS] Selecting best move from root children.\n";
  for (auto child : root->children) {
    if (child->visit_count > best_visit) {
      best_visit = child->visit_count;
      best_child = child;
    }
  }

  if (best_child == nullptr) {
    std::cout << "[MCTS] ERROR: no best child.\n";
    return {"", -1, -1, 0};
  }

  std::string best_block_id = best_child->move_block_id;
  int best_x = best_child->move_x;
  int best_y = best_child->move_y;
  int best_rot = best_child->move_rot;

  cout << "[MCTS] Best move: " << best_child->move_block_id << " ("
       << best_child->move_x << "," << best_child->move_y
       << ") rot=" << best_child->move_rot << "\n";
  // 最良手を返す

  cout << "delete subtree." << endl;
  // ツリー解放
  delete_subtree(root);
  cout << "deleted subtree." << endl;

  return {best_block_id, best_x, best_y, best_rot};
}

int main() {
  const int TILE_NUMBER = 14;
  const int MAX_TREE_DEPTH = 6;
  int iterations = 300; // 試行回数

  init_block_ids_by_size();

  // --- 盤面初期化 ---
  vector<vector<vector<int>>> input_board = {
      // ...（あなたの貼った盤面をそのまま使用）
      // PLAYER1
      {{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
       {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
       {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
       {1, 0, 0, 0, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 1},
       {1, 0, 0, 2, 1, 3, 1, 2, 0, 0, 0, 0, 0, 0, 0, 1},
       {1, 0, 0, 1, 3, 3, 3, 1, 0, 0, 0, 0, 0, 0, 0, 1},
       {1, 0, 0, 2, 1, 3, 1, 2, 0, 0, 0, 0, 0, 0, 0, 1},
       {1, 0, 0, 0, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 1},
       {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
       {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 1},
       {1, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 0, 0, 0, 1},
       {1, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 1},
       {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
       {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
       {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
       {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}},
      // PLAYER2
      {{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
       {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
       {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
       {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
       {1, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
       {1, 0, 0, 0, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 1},
       {1, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
       {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
       {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 2, 0, 0, 1},
       {1, 0, 0, 0, 0, 0, 0, 0, 2, 1, 1, 3, 1, 0, 0, 1},
       {1, 0, 0, 0, 0, 0, 0, 0, 1, 3, 3, 3, 1, 0, 0, 1},
       {1, 0, 0, 0, 0, 0, 0, 0, 1, 3, 1, 1, 2, 0, 0, 1},
       {1, 0, 0, 0, 0, 0, 0, 0, 2, 1, 2, 0, 0, 0, 0, 1},
       {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
       {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
       {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}}};

  Board board(TILE_NUMBER, input_board);

  // --- テスト用ブロック ---
  std::string block_id = "t"; // 任意（例：5マスブロック）
  Block block(getBlock(block_id));
  block.rotate_block(0);

  cout << "=== Board (PLAYER1) ===\n";
  board.print_status(Color::PLAYER1);

  // --- 通常探索 ---
  auto full_positions =
      board.search_settable_position(Color::PLAYER1, block.shape);

  // --- ABLESET 周囲探索 ---
  auto near_positions =
      board.search_settable_position_near_ableset(Color::PLAYER1, block.shape);

  cout << "\n[Full search] positions = " << full_positions.size() << "\n";
  for (auto &[x, y] : full_positions) {
    cout << "(" << x << "," << y << ") ";
  }
  cout << "\n";

  cout << "\n[Near ABLESET search] positions = " << near_positions.size()
       << "\n";
  for (auto &[x, y] : near_positions) {
    cout << "(" << x << "," << y << ") ";
  }
  cout << "\n";

  return 0;
}
