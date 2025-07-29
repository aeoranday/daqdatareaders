/**
 * TPC Data Frame reader details.
 *
 */

#ifndef DAQDATAREADERS_TPCDATAFRAMEREADER_HXX_
#define DAQDATAREADERS_TPCDATAFRAMEREADER_HXX_

namespace dunedaq {
namespace daqdatareaders {

template <class T>
__m256i
TPCDataFrameReader<T>::expand_frame(const __m256i& regi) {
  __m256i odd = _mm256_permutevar8x32_epi32(regi, _mm256_setr_epi32(1, 0, 1, 2, 3, 4, 5, 6));

  __m256i even = _mm256_sllv_epi64(regi, _mm256_setr_epi64x(6, 14, 22, 30));
  odd = _mm256_srlv_epi64(odd, _mm256_setr_epi64x(30, 22, 14, 6));

  __m256i both  = _mm256_blend_epi32(even, odd, 0b01010101);
  __m256i right = _mm256_and_si256(_mm256_set1_epi32(0xFFFFu), both);
  __m256i left  = _mm256_and_si256(_mm256_set1_epi32(0x3FFF0000u), both);

  right = _mm256_srli_epi32(right, 2);
  return _mm256_or_si256(left, right);
}


template <class T>
void
TPCDataFrameReader<T>::fill_adcs(const T& frame, std::span<uint16_t, s_chunk_size> adc_span) {
  const typename T::word_t (*words_ptr)[T::s_bits_per_adc] = frame.adc_words;

  uint16_t* adc_ptr = adc_span.data();

  const int register_alignment = T::s_bits_per_adc * 16;

  for (int t = 0; t < s_num_samples_per_frame; t++) {
    const typename T::word_t *time_sample = *(words_ptr + t);

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


template <class T>
std::tuple<int, int, int, int>
TPCDataFrameReader<T>::get_fragment_det_crate_slot_stream(const std::string& path) {
  const std::unique_ptr<daqdataformats::Fragment> frag = m_h5_file->get_frag_ptr(path);
  T* frame_ptr = reinterpret_cast<T*>(frag->get_data());

  detdataformats::DAQEthHeader daq_header = frame_ptr->daq_header;

  return std::make_tuple(int(daq_header.det_id), int(daq_header.crate_id), int(daq_header.slot_id), int(daq_header.stream_id));
}


template <class T>
std::vector<uint16_t>
TPCDataFrameReader<T>::read_fragment(const std::string& path) {
  std::vector<T> frames = HDF5Reader::read_fragment<T>(path);

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


template <class T>
std::vector<uint16_t>
TPCDataFrameReader<T>::read_all_fragments(const std::vector<std::string>& paths) {
  std::vector<uint16_t> adcs;
  for (auto path : paths) {
    std::vector<uint16_t> new_adcs = read_fragment(path);
    adcs.insert(adcs.end(), std::make_move_iterator(new_adcs.begin()), std::make_move_iterator(new_adcs.end()));
  }
  return adcs;
}

} // namespace daqdatareaders
} // namespace dunedaq

#endif
