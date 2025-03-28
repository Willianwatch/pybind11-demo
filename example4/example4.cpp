#include <pybind11/pybind11.h>

namespace py = pybind11;

int add(int i, int j) {
    return i + j;
}

PYBIND11_MODULE(example, m) {
    m.doc() = "pybind11 example plugin"; // optional module docstring
    m.def("add", &add, "A function which adds two numbers", py::arg("i")=1, py::arg("j")=1);
    m.attr("the_answer") = 42;
    py::object world = py::cast("World");
    m.attr("what") = world;
}