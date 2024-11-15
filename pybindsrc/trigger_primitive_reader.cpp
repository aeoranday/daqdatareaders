/**
 * @file trigger_primitive_reader.cpp
 *
 * This is part of the DUNE DAQ Software Suite, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "daqdatareaders/TriggerPrimitiveReader.hpp"
#include "daqdatareaders/HDF5Reader.hpp"

#include "pybind11/pybind11.h"
#include "pybind11/stl.h"

namespace py = pybind11;

namespace dunedaq::daqdatareaders::python {

void
register_trigger_primitive_reader(py::module& m)
{
  py::class_<TriggerPrimitiveReader, HDF5Reader> tp_reader(m, "TriggerPrimitiveReader");
  tp_reader
    .def(py::init<const std::string &>())
    .def("read_fragment", &TriggerPrimitiveReader::read_fragment)
    .def("read_all_fragments", &TriggerPrimitiveReader::read_all_fragments);
}

} // namespace dunedaq::daqdatareaders::python
