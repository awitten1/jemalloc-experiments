#!/bin/bash

set -eu

script_dir=$(realpath $(dirname $0))
cd "$script_dir"

install_jemalloc() {
    version=$1
    if [ -d install ]; then
        return
    fi

    mkdir -p build
    pushd build
    file="jemalloc-${version}"
    wget https://github.com/jemalloc/jemalloc/releases/download/${version}/jemalloc-${version}.tar.bz2 
    tar -xjvf "${file}".tar.bz2
    "${file}"/configure --enable-stats --enable-prof  \
        --with-malloc-conf --prefix=$script_dir/install

    make -j6
    make install
    popd
}

install_jemalloc 5.3.0