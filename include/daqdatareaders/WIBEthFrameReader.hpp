/**
 * Reader class for WIBEthFrames.
 *
 */

#ifndef DAQDATAREADERS_WIBETHFRAMEREADER_HPP_
#define DAQDATAREADERS_WIBETHFRAMEREADER_HPP_

#include "daqdatareaders/TPCDataFrameReader.hpp"
#include "fddetdataformats/WIBEthFrame.hpp"

#include <vector>
#include <string>

namespace dunedaq {
namespace daqdatareaders {

class WIBEthFrameReader : public TPCDataFrameReader<fddetdataformats::WIBEthFrame> {
  public:
    void filter_fragment_paths(const std::vector<std::string>& fragment_paths) override;

    WIBEthFrameReader(const std::string& file_name) : TPCDataFrameReader<fddetdataformats::WIBEthFrame>(file_name) {
      filter_fragment_paths(m_h5_file->get_all_fragment_dataset_paths());
    }
};

} // namespace daqdatareaders
} // namespace dunedaq

#endif // DAQDATAREADERS_WIBETHFRAMEREADER_HPP_
