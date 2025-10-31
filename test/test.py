import ctypes
import numpy as np
import os

dll_path = os.path.abspath("test.dll")
test = ctypes.cdll.LoadLibrary(dll_path)

copy1d=test.copy
copy1d.argtypes=(ctypes.POINTER(ctypes.c_double),ctypes.POINTER(ctypes.c_double),ctypes.c_int)
copy1d.restype=ctypes.c_bool

arr1=np.random.rand(10)
arr2=np.zeros_like(arr1)
copy1d(arr1.ctypes.data_as(ctypes.POINTER(ctypes.c_double)),arr2.ctypes.data_as(ctypes.POINTER(ctypes.c_double)),len(arr1))

for i in range(len(arr1)):
    print('arr1[{0:d}]={1:f} arr2[{0:d}]={2:f}'.format(i,arr1[i],arr2[i]))

test.printboard(arr1,len(arr1))
