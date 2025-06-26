/**
 * Reader class for WIBEthFrames.
 *
 */

#ifndef DAQDATAREADERS_WIBETHFRAMEREADER_HPP_
#define DAQDATAREADERS_WIBETHFRAMEREADER_HPP_

#include "daqdatareaders/HDF5Reader.hpp"
#include "detdataformats/DAQEthHeader.hpp"
#include "fddetdataformats/WIBEthFrame.hpp"

#include <immintrin.h>

#include <cstdint>
#include <tuple>
#include <vector>

namespace dunedaq {
namespace daqdatareaders {

class WIBEthFrameReader : public HDF5Reader {
  __m256i expand_frame(const __m256i& regi);


  public:
    static const int s_num_channels_per_frame = fddetdataformats::WIBEthFrame::s_num_channels;
    static const int s_num_samples_per_frame = fddetdataformats::WIBEthFrame::s_time_samples_per_frame;
    static const int s_chunk_size = s_num_channels_per_frame * s_num_samples_per_frame;

    WIBEthFrameReader(const std::string& file_name) : HDF5Reader(file_name) {
      filter_fragment_paths(m_h5_file->get_all_fragment_dataset_paths());
    }

    void filter_fragment_paths(const std::vector<std::string>& fragment_paths) override;

    std::vector<uint16_t> read_fragment(const std::string& path);

    std::tuple<int, int, int> get_fragment_crate_slot_stream(const std::string& path);

    std::vector<uint16_t> read_all_fragments(const std::vector<std::string>& paths);

    void fill_adcs(const fddetdataformats::WIBEthFrame& frame, std::span<uint16_t, s_chunk_size> adc_span);
};

} // namespace daqdatareaders
} // namespace dunedaq

#endif // DAQDATAREADERS_WIBETHFRAMEREADER_HPP_
