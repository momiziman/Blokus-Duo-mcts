#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
#include "block_data.h"
#include <algorithm>
#include <array>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <map>
#include <queue>
#include <random>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

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

int main() {
  // ブロック"t"を取得
  const auto &data = block_table.at("n");
  Block block(data);

  // 各方向に回転して表示
  for (int dir = 0; dir < 8; ++dir) {
    Block test_block(data); // 毎回初期状態から
    test_block.rotate_block(dir);

    cout << "=== Direction " << dir << " ===" << endl;

    cout << "Shape:" << endl;
    for (const auto &row : test_block.shape) {
      for (int v : row)
        cout << v << " ";
      cout << endl;
    }

    cout << endl;
  }

  return 0;
}
