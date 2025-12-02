import ctypes
import numpy as np
import os
from enum import Enum

class Color(Enum):
    GREEN  = 0
    YELLOW = 1

COLOR_LIST = [Color.GREEN, Color.YELLOW]

TILE_NUMBER = 14
BLANK   = 0
CANTSET = 1
ABLESET = 2

def make_board():
    board = np.zeros((2, TILE_NUMBER+2, TILE_NUMBER+2), dtype=np.int32)

    # 枠を作成
    for c in range(2):
        board[c, 0, :] = CANTSET
        board[c, TILE_NUMBER+1, :] = CANTSET
        board[c, :, 0] = CANTSET
        board[c, :, TILE_NUMBER+1] = CANTSET

    return board

# DLL 読み込み
dll_path = os.path.abspath("test.dll")
test = ctypes.cdll.LoadLibrary(dll_path)

# C++ 関数の型定義
test.printboard.argtypes = [
    ctypes.POINTER(ctypes.c_int),
    ctypes.c_int
]

# ---------- Python → C++ データ変換 ----------
board_np = make_board()
flat_board = board_np.flatten()   # 3D → 1D に変換
board_ptr = flat_board.ctypes.data_as(ctypes.POINTER(ctypes.c_int))

# C++ の printboard を呼ぶ
test.printboard(board_ptr, TILE_NUMBER)
