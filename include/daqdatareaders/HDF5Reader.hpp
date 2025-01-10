/**
 * HDF5 file entry point reader. Only gets general information of the HDF5 file.
 *
 */

#ifndef DAQDATAREADERS_HDFREADER_HPP_
#define DAQDATAREADERS_HDFREADER_HPP_

#include "hdf5libs/HDF5RawDataFile.hpp"
#include "daqdataformats/Fragment.hpp"

#include <algorithm>
#include <memory>
#include <vector>

namespace dunedaq {
namespace daqdatareaders {

class HDF5Reader {
  protected:
    std::string m_file_name;
    std::unique_ptr<hdf5libs::HDF5RawDataFile> m_h5_file;
    std::vector<std::string> m_fragment_paths;

  public:
    HDF5Reader(const std::string& file_name) {
      m_file_name = file_name;
      m_h5_file = std::make_unique<hdf5libs::HDF5RawDataFile>(m_file_name);
    }

    std::vector<std::string> get_attribute_names();

    template <typename T>
    T get_attribute(const std::string& attr_name);


    virtual ~HDF5Reader() = default;

    virtual void filter_fragment_paths(const std::vector<std::string>& fragment_paths) = 0;

    std::vector<std::string> get_fragment_paths();

    template <typename RDT>
    std::vector<RDT> read_fragment(const std::string& path);

    template <typename RDT>
    std::vector<RDT> read_all_fragments(const std::vector<std::string>& paths);
};

} // namespace daqdatareaders
} // namespace dunedaq

#include "daqdatareaders/HDF5Reader.hxx"

#endif // DAQDATAREADERS_HDF5READER_HPP_
