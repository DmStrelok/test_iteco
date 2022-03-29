#!/bin/bash

build-party()
{
    cd 3party
    rm -rf mbedtls
    mkdir -p mbedtls/libs mbedtls/include/mbedtls mbedtls/include/psa
    tar -xsf mbedtls-2.24.0.tar.gz
    cd mbedtls-2.24.0
    mkdir -p build
    cd build
    cmake ../.
    make
    mv library/*.a ../../mbedtls/libs
    mv include/mbedtls/* ../../mbedtls/include/mbedtls
    mv include/psa/* ../../mbedtls/include/psa
    cd ../../
    rm -rf mbedtls-2.24.0
    cd ..
}

build-project()
{
    rm -rf build
    mkdir -p build/
    cd build/
    cmake ../.
    make
    cd ..
}

case $1 in
    app) build-project ;;
    full) build-party
          build-project
          build/src/testapp/testapp ;;
    *)  build-party
        build-project ;;
esac
