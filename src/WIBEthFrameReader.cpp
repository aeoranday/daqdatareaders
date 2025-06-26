#include "daqdatareaders/WIBEthFrameReader.hpp"

namespace dunedaq {
namespace daqdatareaders {

__m256i
WIBEthFrameReader::expand_frame(const __m256i& regi) {
  __m256i odd = _mm256_permutevar8x32_epi32(regi, _mm256_setr_epi32(1, 0, 1, 2, 3, 4, 5, 6));

  __m256i even = _mm256_sllv_epi64(regi, _mm256_setr_epi64x(6, 14, 22, 30));
  odd = _mm256_srlv_epi64(odd, _mm256_setr_epi64x(30, 22, 14, 6));

  __m256i both  = _mm256_blend_epi32(even, odd, 0b01010101);
  __m256i right = _mm256_and_si256(_mm256_set1_epi32(0xFFFFu), both);
  __m256i left  = _mm256_and_si256(_mm256_set1_epi32(0x3FFF0000u), both);

  right = _mm256_srli_epi32(right, 2);
  return _mm256_or_si256(left, right);
}

void WIBEthFrameReader::filter_fragment_paths(const std::vector<std::string>& fragment_paths) {
  if (!m_fragment_paths.empty())
    m_fragment_paths.clear();

  for (std::string path : fragment_paths) {
    if (path.ends_with("WIBEth"))
      m_fragment_paths.push_back(path);
  }
}

void
WIBEthFrameReader::fill_adcs(const fddetdataformats::WIBEthFrame& frame, std::span<uint16_t, s_chunk_size> adc_span) {
  const fddetdataformats::WIBEthFrame::word_t (*words_ptr)[fddetdataformats::WIBEthFrame::s_bits_per_adc] = frame.adc_words;

  uint16_t* adc_ptr = adc_span.data();

  const int register_alignment = fddetdataformats::WIBEthFrame::s_bits_per_adc * 16;

  for (int t = 0; t < s_num_samples_per_frame; t++) {
    const fddetdataformats::WIBEthFrame::word_t *time_sample = *(words_ptr + t);

    char* cursor = (char*) time_sample;

    for (int p = 0; p < s_num_channels_per_frame / 16; p++) {
      if (p == 3)
        cursor -= 4;

      __m256i regi = _mm256_loadu_si256((__m256i*)cursor);

      if (p == 3)
        regi = _mm256_permutevar8x32_epi32(regi, _mm256_setr_epi32(1, 2, 3, 4, 5, 6, 7, 0));

      __m256i expanded_frame = expand_frame(regi);
      _mm256_storeu_si256(reinterpret_cast<__m256i*>(&adc_ptr[t*64 + p*16]), expanded_frame);

      cursor += register_alignment / 8;
    }
  }
}

std::tuple<int, int, int>
WIBEthFrameReader::get_fragment_crate_slot_stream(const std::string& path) {
  const std::unique_ptr<daqdataformats::Fragment> frag = m_h5_file->get_frag_ptr(path);
  fddetdataformats::WIBEthFrame* frame_ptr = reinterpret_cast<fddetdataformats::WIBEthFrame*>(frag->get_data());

  detdataformats::DAQEthHeader daq_header = frame_ptr->daq_header;

  return std::make_tuple(int(daq_header.crate_id), int(daq_header.slot_id), int(daq_header.stream_id));
}


std::vector<uint16_t>
WIBEthFrameReader::read_fragment(const std::string& path) {
  std::vector<fddetdataformats::WIBEthFrame> frames = HDF5Reader::read_fragment<fddetdataformats::WIBEthFrame>(path);

  const size_t num_frames = frames.size();
  const size_t num_samples = s_num_samples_per_frame * num_frames;
  std::vector<uint16_t> adcs(num_frames * s_chunk_size);

  for (size_t i = 0; i < num_frames; i++) {
    std::span<uint16_t, s_chunk_size> chunk(adcs.data() + i * s_chunk_size, s_chunk_size);
    fill_adcs(frames[i], chunk);
  }

  // Prefer to use the tranpose of what was read.
  std::vector<uint16_t> adcs_transpose(num_frames * s_chunk_size);
  for (size_t i = 0; i < num_samples; i++) {
    for (size_t j = 0; j < s_num_channels_per_frame; j++) {
      adcs_transpose[j * num_samples + i] = adcs[i * s_num_channels_per_frame + j];
    }
  }

  return adcs_transpose;
}

std::vector<uint16_t>
WIBEthFrameReader::read_all_fragments(const std::vector<std::string>& paths) {
  std::vector<uint16_t> adcs;
  for (auto path : paths) {
    std::vector<uint16_t> new_adcs = read_fragment(path);
    adcs.insert(adcs.end(), std::make_move_iterator(new_adcs.begin()), std::make_move_iterator(new_adcs.end()));
  }
  return adcs;
}

} // namespace daqdatareaders
} // namespace dunedaq
