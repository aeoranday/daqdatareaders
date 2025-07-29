/**
 * @file trigger_primitive_reader.cpp
 *
 * This is part of the DUNE DAQ Software Suite, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "daqdatareaders/TDEEthFrameReader.hpp"
#include "daqdatareaders/HDF5Reader.hpp"

#include "pybind11/pybind11.h"
#include "pybind11/numpy.h"
#include "pybind11/stl.h"

namespace py = pybind11;

namespace dunedaq::daqdatareaders::python {

void
register_tdeeth_frame_reader(py::module& m)
{
  py::class_<TDEEthFrameReader, HDF5Reader> tdeeth_frame_reader(m, "TDEEthFrameReader");
  tdeeth_frame_reader
    .def(py::init<const std::string &>())
    .def_property_readonly("tdeeth_fragment_paths", &TDEEthFrameReader::get_fragment_paths)
    .def("get_attribute_names", &TDEEthFrameReader::get_attribute_names)
    .def("get_attribute", py::overload_cast<const std::string&>(&TDEEthFrameReader::get_attribute<std::string>))
    .def("get_int_attribute", py::overload_cast<const std::string&>(&TDEEthFrameReader::get_attribute<int>))
    .def("get_fragment_crate_slot_stream", &TDEEthFrameReader::get_fragment_crate_slot_stream)
    .def("get_trigger_number", &TDEEthFrameReader::get_trigger_number)
    .def("read_fragment",
        [](TDEEthFrameReader& self, std::string& path) {
          std::vector<uint16_t> adcs = self.read_fragment(path);
          const size_t num_samples = adcs.size() / TDEEthFrameReader::s_num_channels_per_frame;

          auto adc_ptr = new std::vector<uint16_t>(std::move(adcs));

          py::capsule free_when_done(adc_ptr, [](void *f) {
              delete reinterpret_cast<std::vector<uint16_t>*>(f);
          });

          const size_t shape[2] = {TDEEthFrameReader::s_num_channels_per_frame, num_samples};
          const size_t stride[2] = {sizeof(uint16_t) * num_samples, sizeof(uint16_t)};

          return py::array_t<uint16_t>(shape, stride, adc_ptr->data(), free_when_done);
        }
    )
    .def("read_all_fragments", &TDEEthFrameReader::read_all_fragments);
}

} // namespace dunedaq::daqdatareaders::python
