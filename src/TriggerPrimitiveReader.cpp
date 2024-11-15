#include "daqdatareaders/TriggerPrimitiveReader.hpp"

namespace dunedaq {
namespace daqdatareaders {

void TriggerPrimitiveReader::filter_fragment_paths(const std::vector<std::string>& fragment_paths) {
  if (!m_fragment_paths.empty())
    m_fragment_paths.clear();

  for (std::string path : fragment_paths) {
    if (path.ends_with("Trigger_Primitive"))
      m_fragment_paths.push_back(path);
  }
}

std::vector<trgdataformats::TriggerPrimitive>
TriggerPrimitiveReader::read_fragment(const std::string& path) {
    return HDF5Reader::read_fragment<trgdataformats::TriggerPrimitive>(path);
}

std::vector<trgdataformats::TriggerPrimitive>
TriggerPrimitiveReader::read_all_fragments(const std::vector<std::string>& paths) {
    return HDF5Reader::read_all_fragments<trgdataformats::TriggerPrimitive>(paths);
}

} // namespace daqdatareaders
} // namespace dunedaq
