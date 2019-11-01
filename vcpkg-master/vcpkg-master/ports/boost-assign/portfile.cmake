# Automatically generated by boost-vcpkg-helpers/generate-ports.ps1

include(vcpkg_common_functions)

vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO boostorg/assign
    REF boost-1.71.0
    SHA512 5875a5244a6f96239a37a196c06a9046c8af57399f37fb4259f268faee35dded2c4185ce2d44d18ccaa0ebfa1b8f6e12190d1873df326cf7068c3e31df28725b
    HEAD_REF master
)

include(${CURRENT_INSTALLED_DIR}/share/boost-vcpkg-helpers/boost-modular-headers.cmake)
boost_modular_headers(SOURCE_PATH ${SOURCE_PATH})