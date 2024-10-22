#include <string>
#include <pybind11/pybind11.h>
namespace py = pybind11;

struct Pet {
    Pet(const std::string &name, unsigned char age) : name(name), age(age) { }
    void setName(const std::string &name_) { name = name_; }
    const std::string &getName() const { return name; }
    void setAge(unsigned char age_) { age = age_; }
    unsigned char getAge() const { return age; }

    std::string name;
private:
    unsigned char age;
};

struct Dog : Pet {
    Dog(const std::string &name, unsigned char age) : Pet(name, age) { }
    std::string bark() const { return "woof!"; }
};

PYBIND11_MODULE(example, m) {
    py::class_<Pet>(m, "Pet", py::dynamic_attr()) //py::class_的构造函数添加py::dynamic_attr标识可以让C++类支持动态属性
        .def(py::init<const std::string &, unsigned char>())
        .def("setName", &Pet::setName)
        .def("getName", &Pet::getName)
        .def("setAge", &Pet::setAge)
        .def("getAge", &Pet::getAge)
        .def_readwrite("name", &Pet::name)
        .def_property("age", &Pet::getAge, &Pet::setAge) //使用class_::def_property()(只读成员使用class_::def_property_readonly())来定义并私有成员，并生成相应的setter和geter方法
        .def("__repr__",
            [](const Pet &a) {
                return "<example.Pet named '" + a.name + "'>";
            });

    // Method 1: template parameter:
    py::class_<Dog, Pet /* <- specify C++ parent type */>(m, "Dog") //将C++基类作为派生类class_的模板参数
        .def(py::init<const std::string &, unsigned char>())
        .def("bark", &Dog::bark);

    // Method 2: pass parent class_ object:
    // py::class_<Dog>(m, "Dog", Pet /* <- specify Python parent type */) //将基类名作为class_的参数绑定到派生类
    //     .def(py::init<const std::string &, unsigned char>())
    //     .def("bark", &Dog::bark);
}