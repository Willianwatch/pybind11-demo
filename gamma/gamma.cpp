#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#include <pybind11/numpy.h>

namespace py = pybind11;

//*2d numpy数组相加
py::array_t<double> _add_arrays_2d(py::array_t<double>& arr1, py::array_t<double>& arr2) {
    py::buffer_info buf1 = arr1.request();
    py::buffer_info buf2 = arr2.request();

    if (buf1.ndim != 2 || buf2.ndim != 2)
        throw std::runtime_error("numpy.ndarray dims must be 2!");
    if ((buf1.shape[0] != buf2.shape[0]) || (buf1.shape[1] != buf2.shape[1]))
        throw std::runtime_error("two array shape must be match!");
  
    auto result = py::array_t<double>(buf1.size);//申请内存
    result.resize({ buf1.shape[0],buf1.shape[1] });//转换为2d矩阵
    py::buffer_info buf_result = result.request();
 
    double* ptr1 = (double*)buf1.ptr;//指针访问读写 numpy.ndarray
    double* ptr2 = (double*)buf2.ptr;
    double* ptr_result = (double*)buf_result.ptr;
 
    for (int i = 0; i < buf1.shape[0]; i++){
        for (int j = 0; j < buf1.shape[1]; j++){
            auto value1 = ptr1[i*buf1.shape[1] + j];
            auto value2 = ptr2[i*buf2.shape[1] + j];
            ptr_result[i*buf_result.shape[1] + j] = value1 + value2;
        }
    }
 
    return result;
} 

py::array_t<float> aibnr_preroc(py::array_t<float>& arr1, int manual_blc, int user_blc, int ispdgain) {
    py::buffer_info buf1 = arr1.request();

    if (buf1.ndim != 2)
        throw std::runtime_error("numpy.ndarray dims must be 2!");
  
    auto result = py::array_t<double>(buf1.size);//申请内存
    result.resize({ buf1.shape[0],buf1.shape[1] });//转换为2d矩阵
    py::buffer_info buf_result = result.request();
 
    double* ptr1 = (double*)buf1.ptr;//指针访问读写 numpy.ndarray
    double* ptr_result = (double*)buf_result.ptr;
 
    for (int i = 0; i < buf1.shape[0]; i++){
        for (int j = 0; j < buf1.shape[1]; j++){
            auto value1 = ptr1[i*buf1.shape[1] + j];
            ptr_result[i*buf_result.shape[1] + j] = (value1 - manual_blc) * (float)ispdgain + user_blc;
        }
    }
 
    return result;
} 

PYBIND11_MODULE(gamma, m) {
    m.doc() = "Simple demo using numpy"; // optional module docstring
    m.def("_add_arrays_2d", &_add_arrays_2d);
    m.def("aibnr_preroc", &aibnr_preroc);
}