/**
 * @file registrators.hpp
 *
 * Declaration of functions to register Python bindings to C++ objects
 *
 * This is part of the DUNE DAQ Software Suite, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */


#ifndef DAQDATAREADERS_PYBINDSRC_REGISTRATORS_HPP_
#define DAQDATAREADERS_PYBINDSRC_REGISTRATORS_HPP_

#include <pybind11/pybind11.h>

namespace dunedaq::daqdatareaders::python {

  void register_hdf5_reader(pybind11::module&);
  void register_trigger_primitive_reader(pybind11::module&);
  void register_wibeth_frame_reader(pybind11::module&);
  void register_tdeeth_frame_reader(pybind11::module&);

}

#endif // DAQDATAREADERS_PYBINDSRC_REGISTRATORS_HPP_
