import ctypes
import numpy as np

class Frame(ctypes.Structure):
    _fields_ = [
        ("width", ctypes.c_int),
        ("height", ctypes.c_int),
        ("data", ctypes.POINTER(ctypes.c_ushort))  # ushort对应c_ushort
    ]

# 加载动态库（根据平台调整后缀）
lib = ctypes.CDLL('./build/libdpc_fw.so')  # Linux
# lib = ctypes.CDLL('./dpc_fw.dll')  # Windows

# 设置函数参数类型
lib.dpc_fw.argtypes = [Frame, ctypes.c_int]
lib.dpc_fw.restype = ctypes.c_int

# 创建数组和指针
array_length = 100
ushort_array_np = np.array([i for i in range(array_length)], dtype=np.uint16)
data_ptr = ushort_array_np.ctypes.data_as(ctypes.POINTER(ctypes.c_ushort))

# 示例调用
frame = Frame(
    width=10, 
    height=10, 
    data=data_ptr  # 直接传递数组首地址指针
)
result = lib.dpc_fw(frame, 10)
# print(f"Result: {result}")