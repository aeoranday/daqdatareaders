/**
 * Abstract reader class for TPC raw data.
 *
 */


#ifndef DAQDATAREADERS_TPCDATAREADER_HPP_
#define DAQDATAREADERS_TPCDATAREADER_HPP_

#include "daqdatareaders/HDF5Reader.hpp"
#include "detdataformats/DAQEthHeader.hpp"

#include <immintrin.h>

#include <cstdint>
#include <tuple>
#include <vector>

namespace dunedaq {
namespace daqdatareaders {

template <class T>
class TPCDataFrameReader : public HDF5Reader {
  __m256i expand_frame(const __m256i& regi);

  public:
    static const int s_num_channels_per_frame = T::s_num_channels;
    static const int s_num_samples_per_frame = T::s_time_samples_per_frame;
    static const int s_chunk_size = s_num_channels_per_frame * s_num_samples_per_frame;

    TPCDataFrameReader(const std::string& file_name) : HDF5Reader(file_name) {}

    virtual void filter_fragment_paths(const std::vector<std::string>& fragment_paths) = 0;

    std::vector<uint16_t> read_fragment(const std::string& path);

    std::tuple<int, int, int, int> get_fragment_det_crate_slot_stream(const std::string& path);

    std::vector<uint16_t> read_all_fragments(const std::vector<std::string>& paths);

    void fill_adcs(const T& frame, std::span<uint16_t, s_chunk_size> adc_span);
};

} // namespace daqdatareaders
} // namespace dunedaq

#include "daqdatareaders/details/TPCDataFrameReader.hxx"

#endif
