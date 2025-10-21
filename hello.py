import ctypes
import os

# DLLの絶対パスを指定（MinGWだと相対パスがうまく通らない場合があります）
dll_path = os.path.abspath("hello.dll")
print("Loading:", dll_path)

lib = ctypes.CDLL(dll_path)
lib.hello()
