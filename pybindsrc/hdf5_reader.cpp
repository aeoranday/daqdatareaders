/**
 * @file hdf5_reader.cpp
 *
 * This is part of the DUNE DAQ Software Suite, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "daqdatareaders/HDF5Reader.hpp"
#include "trgdataformats/TriggerPrimitive.hpp"

#include "pybind11/pybind11.h"
#include "pybind11/stl.h"

namespace py = pybind11;

namespace dunedaq::daqdatareaders::python {

void
register_hdf5_reader(py::module& m)
{
  py::class_<HDF5Reader> hdf5_reader(m, "HDF5Reader");
//  hdf5_reader
//    .def(py::init<const std::string &>())
//    .def("read_tp_fragment", &HDF5Reader::read_fragment<trgdataformats::TriggerPrimitive>)
//    .def("read_tp_all_fragments", &HDF5Reader::read_all_fragments<trgdataformats::TriggerPrimitive>);
}

} // namespace dunedaq::daqdatareaders::python

