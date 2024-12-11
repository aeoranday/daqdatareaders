#include "daqdatareaders/WIBEthFrameReader.hpp"

namespace dunedaq {
namespace daqdatareaders {

void WIBEthFrameReader::filter_fragment_paths(const std::vector<std::string>& fragment_paths) {
  if (!m_fragment_paths.empty())
    m_fragment_paths.clear();

  for (std::string path : fragment_paths) {
    if (path.ends_with("WIBEth"))
      m_fragment_paths.push_back(path);
  }
}

  }
}

std::vector<fddetdataformats::WIBEthFrame>
WIBEthFrameReader::read_fragment(const std::string& path) {
    return HDF5Reader::read_fragment<fddetdataformats::WIBEthFrame>(path);
}

std::vector<fddetdataformats::WIBEthFrame>
WIBEthFrameReader::read_all_fragments(const std::vector<std::string>& paths) {
    return HDF5Reader::read_all_fragments<fddetdataformats::WIBEthFrame>(paths);
}

} // namespace daqdatareaders
} // namespace dunedaq
