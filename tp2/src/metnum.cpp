#include <pybind11/pybind11.h>
#include <pybind11/iostream.h>
#include <pybind11/eigen.h>
#include "knn.h"
#include "pca.h"
#include "eigen.h"
#include "types.h"

namespace py=pybind11;
typedef py::call_guard<py::scoped_ostream_redirect, py::scoped_estream_redirect> callguard;

// el primer argumento es el nombre...
PYBIND11_MODULE(metnum, m) {
    py::class_<KNNClassifier>(m, "KNNClassifier")
        .def(py::init<unsigned int>())
        .def("fit", &KNNClassifier::fit)
        .def("load", &KNNClassifier::load, callguard())
        .def("predict", &KNNClassifier::predict, callguard());

    py::class_<PCA>(m, "PCA")
        .def(py::init<unsigned int>())
        .def("fit", &PCA::fit, callguard())
        .def("getTC", &PCA::getTC)
        .def("setTC", &PCA::setTC)
        .def("transform", &PCA::transform);
    m.def(
        "power_iteration", &power_iteration,
        "Function that calculates eigenvector",
        py::arg("X"),
        py::arg("num_iter")=5000,
        py::arg("epsilon")=1e-16
    );
    m.def(
        "get_first_eigenvalues", &get_first_eigenvalues,
        "Function that calculates eigenvector",
        py::arg("X"),
        py::arg("num"),
        py::arg("num_iter")=5000,
        py::arg("epsilon")=1e-16
    );

}
