#ifndef SPARKLABS_SCRIPTS_PYBINDINGS_H
#define SPARKLABS_SCRIPTS_PYBINDINGS_H

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>

namespace py = pybind11;

namespace SparkLabs {

class PythonBindings {
public:
    static void RegisterAll(py::module& m);

private:
    static void RegisterCoreTypes(py::module& m);
    static void RegisterMathTypes(py::module& m);
    static void RegisterEngineTypes(py::module& m);
    static void RegisterWorkflowTypes(py::module& m);
    static void RegisterNPCTypes(py::module& m);
    static void RegisterGameplayTypes(py::module& m);
};

}

PYBIND11_MODULE(sparklabs, m) {
    m.doc() = "SparkLabs Engine - AI-Native Game Engine Python Bindings";

    py::class_<SparkLabs::PythonBindings>(m, "PythonBindings")
        .def_static("register_all", &SparkLabs::PythonBindings::RegisterAll);

    SparkLabs::PythonBindings::RegisterAll(m);
}

#endif
