#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>  // 需要支持numpy数组操作[3](@ref)


// 声明原始C函数（需确保编译时链接原始库）
extern "C" {
    #include "dpc_fw.h"
}

// 定义Python可调用的Frame结构体
namespace py = pybind11;

struct PyFrame {
    int width;
    int height;
    py::array_t<unsigned short> data;  // 使用numpy数组封装指针[3](@ref)

    // 将Python的numpy数组转换为C指针
    unsigned short* get_data() const {
        return static_cast<unsigned short*>(data.request().ptr);
    }
};

// 封装dpc_fw函数
PYBIND11_MODULE(dpc_module, m) {
    // 注册Frame结构体
    py::class_<PyFrame>(m, "Frame")
        .def(py::init<>())
        .def_readwrite("width", &PyFrame::width)
        .def_readwrite("height", &PyFrame::height)
        .def_readwrite("data", &PyFrame::data);

    // 绑定dpc_fw函数
    m.def("dpc_fw", [](const PyFrame& frame, int gamma_val) {
        Frame c_frame = {frame.width, frame.height, frame.get_data()};
        return dpc_fw(c_frame, gamma_val);
    }, py::arg("frame"), py::arg("gamma_val"));
}