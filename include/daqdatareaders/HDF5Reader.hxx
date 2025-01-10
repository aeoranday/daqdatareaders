#ifndef DAQDATAREADER_HDF5READER_HXX_
#define DAQDATAREADER_HDF5READER_HXX_

namespace dunedaq {
namespace daqdatareaders {

template <typename T>
T HDF5Reader::get_attribute(const std::string& attr_name) {
  return m_h5_file->get_attribute<T>(attr_name);
}

template <typename RDT>
std::vector<RDT>
HDF5Reader::read_fragment(const std::string& path) {
  const std::unique_ptr<daqdataformats::Fragment> frag = m_h5_file->get_frag_ptr(path);

  // Pre-allocation.
  const int num_rdts = frag->get_data_size() / sizeof(RDT);
  std::vector<RDT> data(num_rdts);

  // Reading and storing.
  RDT* data_ptr = reinterpret_cast<RDT*>(frag->get_data());
  for (int i = 0; i < num_rdts; i++) {
    data[i] = *data_ptr;
    data_ptr++;
  }

  return data;
}

template <typename RDT>
std::vector<RDT> HDF5Reader::read_all_fragments(const std::vector<std::string>& paths) {
  // Pre-allocation.
  int num_rdts = 0;
  for (auto path : paths) {
    const std::unique_ptr<daqdataformats::Fragment> frag = m_h5_file->get_frag_ptr(path);
    num_rdts += frag->get_data_size();
  }
  std::vector<RDT> data(num_rdts);

  // Reading and storing.
  int idx = 0;
  for (auto path : paths) {
    std::vector<RDT> datum = read_fragment<RDT>(path);
    int step = datum.size();
    std::move(datum.begin(), datum.end(), data.begin() + idx);
    idx += step;
  }
  return data;
}

} // namespace daqdatareaders
} // namespace dunedaq

#endif // DAQDATAREADER_HDF5READER_HXX_
