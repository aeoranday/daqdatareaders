/**
 * Reader class for WIBEthFrames.
 *
 */

#ifndef DAQDATAREADERS_WIBETHFRAMEREADER_HPP_
#define DAQDATAREADERS_WIBETHFRAMEREADER_HPP_

#include "daqdatareaders/HDF5Reader.hpp"
#include "fddetdataformats/WIBEthFrame.hpp"

#include <vector>

namespace dunedaq {
namespace daqdatareaders {

class WIBEthFrameReader : public HDF5Reader {
  std::vector<std::string> m_wibeth_paths;

  public:
    WIBEthFrameReader(const std::string& file_name) : HDF5Reader(file_name) {
      filter_fragment_paths(m_h5_file->get_all_fragment_dataset_paths());
    }

    void filter_fragment_paths(const std::vector<std::string>& fragment_paths) override;

    std::vector<fddetdataformats::WIBEthFrame> read_fragment(const std::string& path);

    std::vector<fddetdataformats::WIBEthFrame> read_all_fragments(const std::vector<std::string>& paths);
};

} // namespace daqdatareaders
} // namespace dunedaq

#endif // DAQDATAREADERS_WIBETHFRAMEREADER_HPP_
