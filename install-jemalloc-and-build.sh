#!/bin/bash

set -eu

script_dir=$(realpath $(dirname $0))
pushd "$script_dir"
install_dir="install"

install_jemalloc() {
    version=$1
    file="jemalloc-${version}"
    if [ -d "$script_dir"/"${install_dir}" ]; then
        return
    fi

    build="jemalloc-build"

    mkdir -p "${build}"
    pushd "${build}"
    wget https://github.com/jemalloc/jemalloc/releases/download/${version}/jemalloc-${version}.tar.bz2 
    tar -xjvf "${file}".tar.bz2
    "${file}"/configure --enable-stats --enable-prof  \
        --with-malloc-conf --prefix="$script_dir"/"${install_dir}"

    make -j6
    make install
    popd
}

build_experiments() {
    cmake -B build -DCMAKE_BUILD_TYPE=RelWithDebInfo \
        -G Ninja -DJEMALLOC_INSTALL_DIR="${install_dir}"
    cmake --build build
}

install_jemalloc 5.3.0
build_experiments

popd