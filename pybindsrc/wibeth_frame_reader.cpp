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
    .def("get_attribute_names", &WIBEthFrameReader::get_attribute_names)
    .def("get_attribute", py::overload_cast<const std::string&>(&WIBEthFrameReader::get_attribute<std::string>))
    .def("get_int_attribute", py::overload_cast<const std::string&>(&WIBEthFrameReader::get_attribute<int>))
    .def("get_fragment_timestamp", &WIBEthFrameReader::get_fragment_timestamp)
    .def("get_fragment_det_crate_slot_stream", &WIBEthFrameReader::get_fragment_det_crate_slot_stream)
    .def("get_trigger_number", &WIBEthFrameReader::get_trigger_number)
    .def("read_fragment",
        [](WIBEthFrameReader& self, std::string& path) {
          std::vector<uint16_t> adcs = self.read_fragment(path);
          const size_t num_samples = adcs.size() / WIBEthFrameReader::s_num_channels_per_frame;

          auto adc_ptr = new std::vector<uint16_t>(std::move(adcs));

          py::capsule free_when_done(adc_ptr, [](void *f) {
              delete reinterpret_cast<std::vector<uint16_t>*>(f);
          });

          const size_t shape[2] = {WIBEthFrameReader::s_num_channels_per_frame, num_samples};
          const size_t stride[2] = {sizeof(uint16_t) * num_samples, sizeof(uint16_t)};

          return py::array_t<uint16_t>(shape, stride, adc_ptr->data(), free_when_done);
        }
    )
    .def("read_all_fragments", &WIBEthFrameReader::read_all_fragments);
}

} // namespace dunedaq::daqdatareaders::python
