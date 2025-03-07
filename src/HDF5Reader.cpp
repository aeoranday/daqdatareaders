#include "daqdatareaders/HDF5Reader.hpp"

namespace dunedaq {
namespace daqdatareaders {

//HDF5Reader::HDF5Reader(const std::string& file_name) {
//  m_file_name = file_name;
//  m_h5_file = std::make_unique<hdf5libs::HDF5RawDataFile>(m_file_name);
//}

std::vector<std::string>
HDF5Reader::get_fragment_paths() {
  return m_fragment_paths;
}

std::vector<std::string>
HDF5Reader::get_attribute_names() {
  return m_h5_file->get_attribute_names();
}

int
HDF5Reader::get_trigger_number(const std::string& path) {
  const std::unique_ptr<daqdataformats::Fragment> frag = m_h5_file->get_frag_ptr(path);
  return frag->get_trigger_number();
}

}
}
