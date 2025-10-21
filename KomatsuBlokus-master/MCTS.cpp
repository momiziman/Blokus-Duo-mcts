#include <algorithm>
#include <array>
#include <chrono>
#include <iostream>
#include <queue>
#include <random>
#include <string>
#include <vector>

using std::array, std::vector;
using std::cout, std::endl, std::cin;

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

class State
{
private:
public:
    // 現在のターン
    int t_ = 0;
    State() {}

    int make_board()
    {
        int board[16][16];
        for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < 16; j++)
            {
                board[i][j] = 1;
                board[j][i] = 1;
                board[15 - i][j] = 1;
                board[j][15 - i] = 1;
            }
        }
    }

    int legal_moves(int board)
    {
    }

    // ゲームが終了したか判定する
    bool isDone() const
    {
        // ターン以外に判定する処理があればここに書く
        return this->t_ >= END_TURN;
    }

    // 指定したactionで1ターン進める
    void advance(const int action)
    {
        // 問題に合わせた処理
        ++this->t_; // ターンを進めるのを忘れずに(実装方針次第ではここで管理しなくてもよい)
    }

    // ランダムに盤面を更新する
    void randomUpdate()
    {
        // parameter をなんらかの方法で乱数生成する処理をここに書く
        update(parameter);
    }

    // 指定したパラメータで盤面を更新する
    void update(int parameter)
    {
        // 問題に合わせた処理。引数の数は1個とは限らない
    }

    // スコアを取得する
    double getScore() const
    {
        double score = 0;
        // 問題に合わせた処理
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
    double playout(State state)
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
