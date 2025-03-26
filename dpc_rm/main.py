import ctypes
import numpy as np

class Frame(ctypes.Structure):
    _fields_ = [
        ("width", ctypes.c_int),
        ("height", ctypes.c_int),
        ("data", ctypes.POINTER(ctypes.c_ushort)),  # ushort对应c_ushort
        ("shape", ctypes.c_int * 2)       # 对应C的int shape[2]
    ]
    def __init__(self, width: int, height: int, data: np.ndarray, shape: tuple = None):
        self.width = width
        self.height = height
        self.data = data.ctypes.data_as(ctypes.POINTER(ctypes.c_ushort))  # 直接传递数组首地址指针
        self.shape = (width, height) if shape is None else shape

# 加载动态库（根据平台调整后缀）
lib = ctypes.CDLL('./build/libdpc_fw.so')  # Linux
# lib = ctypes.CDLL('./dpc_fw.dll')  # Windows

# 设置函数参数类型
lib.dpc_fw.argtypes = [Frame, ctypes.c_int]
lib.dpc_fw.restype = ctypes.c_int

lib.dpc_fw_1.argtypes = [ctypes.POINTER(Frame), ctypes.c_int]
lib.dpc_fw_1.restype = ctypes.c_int

"""
方法一：使用 ctypes 创建动态数组
# 创建自定义 ushort 数组（例如长度为 100）
array_length = 100
# 初始化数组元素（示例：前 10 个元素设为 1，其余为 0）
ushort_array = (ctypes.c_ushort * array_length)(*([1] * 10 + [0] * (array_length - 10))))

# 获取数组首元素的指针
data_ptr = ctypes.cast(ushort_array, ctypes.POINTER(ctypes.c_ushort))
"""
"""
方法二：通过指针直接操作内存
# 创建未初始化的数组（长度为 100）
array_length = 100
ushort_array = (ctypes.c_ushort * array_length)()

# 手动设置数组元素（示例：设置前 10 个元素为 1）
for i in range(10):
    ushort_array[i] = 1
"""

# 创建数组和指针
array_length = 100
ushort_array_np = np.array([i for i in range(array_length)], dtype=np.uint16)
data_ptr = ushort_array_np.ctypes.data_as(ctypes.POINTER(ctypes.c_ushort))

# 示例调用
frame = Frame(
    width=10, 
    height=10, 
    data=ushort_array_np, # 直接传递数组首地址指针
    shape=(5, 20)
)
result = lib.dpc_fw(frame, 10)
print(f"Result: {result}")

result1 = lib.dpc_fw_1(ctypes.byref(frame), 10)
print(f"Result1: {result1}")