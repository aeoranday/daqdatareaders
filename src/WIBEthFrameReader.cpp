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

std::vector<std::vector<uint16_t>>
WIBEthFrameReader::get_adcs(const fddetdataformats::WIBEthFrame& frame) {
  std::vector<std::vector<uint16_t>> adcs;
  adcs.reserve(64);

  const fddetdataformats::WIBEthFrame::word_t (*words_ptr)[fddetdataformats::WIBEthFrame::s_bits_per_adc] = frame.adc_words;

  const int register_alignment = fddetdataformats::WIBEthFrame::s_bits_per_adc * 16;

  for (int t = 0; t < 64; t++) {
    const fddetdataformats::WIBEthFrame::word_t *time_sample = *(words_ptr + t);

    char* cursor = (char*) time_sample;

    std::vector<uint16_t> sample(64, 0);
    for (int p = 0; p < 4; p++) {
      if (p == 3)
        cursor -= 4;

      __m256i regi = _mm256_lddqu_si256((__m256i*)cursor);

      if (p == 3)
        regi = _mm256_permutevar8x32_epi32(regi, _mm256_setr_epi32(1, 2, 3, 4, 5, 6, 7, 0));

      __m256i expanded_frame = expand_frame(regi);
      _mm256_storeu_si256(reinterpret_cast<__m256i*>(sample.data() + p*16), expanded_frame);

      cursor += register_alignment / 8;
    }
    adcs.push_back(std::move(sample));
  }

  return adcs;
}

std::vector<std::vector<uint16_t>>
WIBEthFrameReader::read_fragment(const std::string& path) {
  std::vector<fddetdataformats::WIBEthFrame> frames = HDF5Reader::read_fragment<fddetdataformats::WIBEthFrame>(path);

  std::vector<std::vector<uint16_t>> adcs;
  adcs.reserve(frames.size() * 64);
  for (auto frame : frames) {
    std::vector<std::vector<uint16_t>> new_adcs = get_adcs(frame);
    adcs.insert(adcs.end(), std::make_move_iterator(new_adcs.begin()), std::make_move_iterator(new_adcs.end()));
  }

  return adcs;
}

std::vector<std::vector<uint16_t>>
WIBEthFrameReader::read_all_fragments(const std::vector<std::string>& paths) {
  std::vector<std::vector<uint16_t>> adcs;
  for (auto path : paths) {
    std::vector<std::vector<uint16_t>> new_adcs = read_fragment(path);
    adcs.insert(adcs.end(), std::make_move_iterator(new_adcs.begin()), std::make_move_iterator(new_adcs.end()));
  }
  return adcs;
}

} // namespace daqdatareaders
} // namespace dunedaq
