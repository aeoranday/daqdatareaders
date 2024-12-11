/**
 * @file trigger_primitive_reader.cpp
 *
 * This is part of the DUNE DAQ Software Suite, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "daqdatareaders/WIBEthFrameReader.hpp"
#include "daqdatareaders/HDF5Reader.hpp"

#include "pybind11/pybind11.h"
#include "pybind11/numpy.h"
#include "pybind11/stl.h"

namespace py = pybind11;

namespace dunedaq::daqdatareaders::python {

void
register_wibeth_frame_reader(py::module& m)
{
  py::class_<WIBEthFrameReader, HDF5Reader> wibeth_frame_reader(m, "WIBEthFrameReader");
  wibeth_frame_reader
    .def(py::init<const std::string &>())
    .def_property_readonly("wibeth_fragment_paths", &WIBEthFrameReader::get_fragment_paths)
    .def("get_fragment_crate_slot_stream", &WIBEthFrameReader::get_fragment_crate_slot_stream)
    .def("read_fragment", &WIBEthFrameReader::read_fragment)
    .def("read_all_fragments", &WIBEthFrameReader::read_all_fragments);
}

} // namespace dunedaq::daqdatareaders::python
