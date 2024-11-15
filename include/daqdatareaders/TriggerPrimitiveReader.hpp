/**
 * Reader class for TriggerPrimitives.
 *
 */

#ifndef DAQDATAREADERS_TRIGGERPRIMITIVEREADER_HPP_
#define DAQDATAREADERS_TRIGGERPRIMITIVEREADER_HPP_

#include "daqdatareaders/HDF5Reader.hpp"
#include "trgdataformats/TriggerPrimitive.hpp"

#include <vector>

namespace dunedaq {
namespace daqdatareaders {

class TriggerPrimitiveReader : public HDF5Reader {
  public:
    TriggerPrimitiveReader(const std::string& file_name) : HDF5Reader(file_name) {
      filter_fragment_paths(m_h5_file->get_all_fragment_dataset_paths());
    }

    void filter_fragment_paths(const std::vector<std::string>& fragment_paths) override;

    std::vector<trgdataformats::TriggerPrimitive> read_fragment(const std::string& path);

    std::vector<trgdataformats::TriggerPrimitive> read_all_fragments(const std::vector<std::string>& paths);
};

} // namespace daqdatareaders
} // namespace dunedaq

#endif // DAQDATAREADERS_TRIGGERPRIMITIVEREADER_HPP_
