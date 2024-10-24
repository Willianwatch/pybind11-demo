#include <string>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
namespace py = pybind11;

struct Pet {
    Pet(const std::string &name, unsigned char age) : name(name), age(age) { }
    void setName(const std::string &name_) { name = name_; }
    const std::string &getName() const { return name; }
    void setAge(unsigned char age_) { age = age_; }
    unsigned char getAge() const { return age; }
    void set(unsigned char age_) { age = age_; }
    void set(const std::string &name_) { name = name_; }

    std::string name;
private:
    unsigned char age;
};

struct Dog : Pet {
    Dog(const std::string &name, unsigned char age) : Pet(name, age) { }
    std::string bark() const { return "woof!"; }
};

struct PolymorphicPet //在C++中，一个类至少有一个虚函数才会被视为多态类型。pybind11会自动识别这种多态机制。
{
    /* data */
    virtual ~PolymorphicPet() = default;
};

struct PolymorphicDog : PolymorphicPet 
{
    /* data */
    std::string bark() const { return "woof!"; }
};


class Matrix {
public:
    Matrix(size_t rows, size_t cols) : m_rows(rows), m_cols(cols) {
        m_data = new float[rows*cols];
    }
    float *data() { return m_data; }
    size_t rows() const { return m_rows; }
    size_t cols() const { return m_cols; }
private:
    size_t m_rows, m_cols;
    float *m_data;
};

//非常重要的例子
py::array_t<double> add_arrays(py::array_t<double> input1, py::array_t<double> input2) {
    py::buffer_info buf1 = input1.request(), buf2 = input2.request();

    if (buf1.ndim != 1 || buf2.ndim != 1)
        throw std::runtime_error("Number of dimensions must be one");

    if (buf1.size != buf2.size)
        throw std::runtime_error("Input shapes must match");

    /* No pointer is passed, so NumPy will allocate the buffer */
    auto result = py::array_t<double>(buf1.size);

    py::buffer_info buf3 = result.request();

    double *ptr1 = static_cast<double *>(buf1.ptr);
    double *ptr2 = static_cast<double *>(buf2.ptr);
    double *ptr3 = static_cast<double *>(buf3.ptr);

    for (size_t idx = 0; idx < buf1.shape[0]; idx++)
        ptr3[idx] = ptr1[idx] + ptr2[idx];

    return result;
}

const uint8_t buffer[] = {
    0, 1, 2, 3,
    4, 5, 6, 7
};

PYBIND11_MODULE(example, m) {
    py::class_<Pet>(m, "Pet", py::dynamic_attr()) //py::class_的构造函数添加py::dynamic_attr标识可以让C++类支持动态属性
        .def(py::init<const std::string &, unsigned char>())
        .def("setName", &Pet::setName)
        .def("getName", &Pet::getName)
        .def("setAge", &Pet::setAge)
        .def("getAge", &Pet::getAge)
        .def("set", static_cast<void (Pet::*)(unsigned char)>(&Pet::set), "Set the pet's age")
        .def("set", static_cast<void (Pet::*)(const std::string &)>(&Pet::set), "Set the pet's name")
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

    py::class_<PolymorphicPet>(m, "PolymorphicPet");
    py::class_<PolymorphicDog, PolymorphicPet>(m, "PolymorphicDog")
        .def(py::init<>())
        .def("bark", &PolymorphicDog::bark);
    m.def("pet_store2", []() { return std::unique_ptr<PolymorphicPet>(new PolymorphicDog); });
    /**
    >>> p = example.pet_store2()
    >>> type(p)
    PolymorphicDog  # automatically downcast
    >>> p.bark()
    u'woof!'

     */

    py::class_<Matrix>(m, "Matrix", py::buffer_protocol())
        .def_buffer([](Matrix &m) -> py::buffer_info {
                return py::buffer_info(
                    m.data(),                               /* Pointer to buffer */
                    sizeof(float),                          /* Size of one scalar */
                    py::format_descriptor<float>::format(), /* Python struct-style format descriptor */
                    2,                                      /* Number of dimensions */
                    { m.rows(), m.cols() },                 /* Buffer dimensions */
                    { sizeof(float) * m.cols(),             /* Strides (in bytes) for each index */
                    sizeof(float) }
                );
            });

    m.def("add_arrays", &add_arrays, "Add two NumPy arrays");

    m.def("sum_3d", [](py::array_t<double> x) {
        auto r = x.unchecked<3>(); // x must have ndim = 3; can be non-writeable
        double sum = 0;
        for (py::ssize_t i = 0; i < r.shape(0); i++)
            for (py::ssize_t j = 0; j < r.shape(1); j++)
                for (py::ssize_t k = 0; k < r.shape(2); k++)
                    sum += r(i, j, k);
        return sum;
    });
    m.def("increment_3d", [](py::array_t<double> x) {
        auto r = x.mutable_unchecked<3>(); // Will throw if ndim != 3 or flags.writeable is false
        for (py::ssize_t i = 0; i < r.shape(0); i++)
            for (py::ssize_t j = 0; j < r.shape(1); j++)
                for (py::ssize_t k = 0; k < r.shape(2); k++)
                    r(i, j, k) += 1.0;
    }, py::arg().noconvert());

    m.def("get_memoryview2d", []() {
        return py::memoryview::from_buffer(
            buffer,                                    // buffer pointer
            { 2, 4 },                                  // shape (rows, cols)
            { sizeof(uint8_t) * 4, sizeof(uint8_t) }   // strides in bytes
        );
    });
    m.def("get_memoryview1d", []() {
        return py::memoryview::from_memory(
            buffer,               // buffer pointer
            sizeof(uint8_t) * 8   // buffer size
        );
    });

}