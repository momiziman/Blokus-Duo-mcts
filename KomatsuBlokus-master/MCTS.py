from dataclasses import dataclass
import numpy as np


@dataclass
class Config():
    simulation_times: int = 10    # 1stepで実行するシミュレーション回数
    expansion_threshold: int = 5  # 展開する閾値
    gamma: float = 1.0            # 割引率
    uct_c: float = np.sqrt(2.0)   # UCTの探索パラメータ

class Parameter():
    def __init__(self, *args):
        self.N = {}  # 訪問回数
        self.W = {}  # 累計報酬

    # 新しい状態の初期化
    # self.config.action_num に環境側の取りうるアクション数が入っています
    def init_state(self, state):
        if state not in self.N:
            self.W[state] = [0 for _ in range(self.config.action_num)]
            self.N[state] = [0 for _ in range(self.config.action_num)]
