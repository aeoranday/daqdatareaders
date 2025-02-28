

# This is a skeleton CMakeLists.txt file, auto-generated on
# Fri Oct 11 07:01:13 CEST 2024.  The developer(s) of this package should delete
# this comment as well as adding dependent targets, packages,
# etc. specific to the package. For details on how to write a package,
# please see
# https://dune-daq-sw.readthedocs.io/en/latest/packages/daq-cmake/

cmake_minimum_required(VERSION 3.12)
project(daqdatareaders VERSION 0.0.0)

find_package(daq-cmake REQUIRED)

daq_setup_environment()


find_package(daqdataformats REQUIRED)
find_package(detdataformats REQUIRED)
find_package(fddetdataformats REQUIRED)
find_package(fmt REQUIRED)
find_package(hdf5libs REQUIRED)
find_package(trgdataformats REQUIRED)
#find_package(Boost COMPONENTS unit_test_framework REQUIRED)

##############################################################################

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx2")


# See https://dune-daq-sw.readthedocs.io/en/latest/packages/daq-cmake/#daq_add_library

daq_add_library(*.cpp LINK_LIBRARIES daqdataformats::daqdataformats detdataformats::detdataformats fddetdataformats::fddetdataformats fmt::fmt hdf5libs::hdf5libs trgdataformats::trgdataformats) # We need to link in the opmonlib library for Protobuf-generated code to compile

##############################################################################


# See https://dune-daq-sw.readthedocs.io/en/latest/packages/daq-cmake/#daq_add_python_bindings


daq_add_python_bindings(*.cpp LINK_LIBRARIES ${PROJECT_NAME} daqdataformats::daqdataformats detdataformats::detdataformats fddetdataformats::fddetdataformats hdf5libs::hdf5libs trgdataformats::trgdataformats) # Any additional libraries to link in beyond the main library not yet determined


##############################################################################


# See https://dune-daq-sw.readthedocs.io/en/latest/packages/daq-cmake/#daq_add_unit_test

#daq_add_unit_test(Placeholder_test LINK_LIBRARIES)  # Placeholder_test should be replaced with real unit tests

##############################################################################

daq_install()

