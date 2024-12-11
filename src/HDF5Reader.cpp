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

}
}
