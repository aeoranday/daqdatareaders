#include "daqdatareaders/TDEEthFrameReader.hpp"

namespace dunedaq {
namespace daqdatareaders {

void TDEEthFrameReader::filter_fragment_paths(const std::vector<std::string>& fragment_paths) {
  if (!m_fragment_paths.empty())
    m_fragment_paths.clear();

  for (std::string path : fragment_paths) {
    if (path.ends_with("TDEEth"))
      m_fragment_paths.push_back(path);
  }
}

} // namespace daqdatareaders
} // namespace dunedaq
