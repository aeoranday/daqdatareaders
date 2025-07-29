/**
 * Reader class for TDEEthFrames.
 *
 */

#ifndef DAQDATAREADERS_TDEETHFRAMEREADER_HPP_
#define DAQDATAREADERS_TDEETHFRAMEREADER_HPP_

#include "daqdatareaders/TPCDataFrameReader.hpp"
#include "fddetdataformats/TDEEthFrame.hpp"

#include <vector>
#include <string>

namespace dunedaq {
namespace daqdatareaders {

class TDEEthFrameReader : public TPCDataFrameReader<fddetdataformats::TDEEthFrame> {
  public:
    void filter_fragment_paths(const std::vector<std::string>& fragment_paths) override;

    TDEEthFrameReader(const std::string& file_name) : TPCDataFrameReader<fddetdataformats::TDEEthFrame>(file_name) {
      filter_fragment_paths(m_h5_file->get_all_fragment_dataset_paths());
    }
};

} // namespace daqdatareaders
} // namespace dunedaq

#endif // DAQDATAREADERS_TDEETHFRAMEREADER_HPP_
