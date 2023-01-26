#!/bin/bash

usage() { echo "usage: $0 [-a|c {a} (all|clean)] [-r|d {d} (release|debug)] [-g <cmake-generator> {Ninja}] [-p <cmake-prefix-path>]" 2>&1; exit 0; }

BUILD_TYPE="Debug"
GENERATOR="Ninja"
TARGET="all"
ACTION="Build"

while getopts ":acrdg:p:h" arg; do
    case "${arg}" in
        a)
            TARGET="all"
            ACTION="Build"
            ;;
        c)
            TARGET="clean"
            ACTION="Clean"
            ;;
        r)
            BUILD_TYPE="Release"
            ;;
        d)
            BUILD_TYPE="Debug"
            ;;
        g)
            GENERATOR=${OPTARG}
            ;;
        p)
            PREFIX_PATH=${OPTARG}
            ;;
        h|*)
            usage
            ;;
    esac
done
shift $((OPTIND-1))

if [ ${TARGET} = "all" -a -z "${PREFIX_PATH}" ]; then
    echo "Please provide a valid prefix path to the event loop headers and library"
    usage
fi

if [ -z "${GENERATOR}" ]; then
    usage
fi

if [ ${TARGET} = "all" ]; then
    if [ -d ${PWD}/build ]; then
        rm -rf build/{.[!.]*,*}
    fi
    echo "Configuring Shopping Cart in ${BUILD_TYPE} mode..."
    cmake -DCMAKE_PREFIX_PATH:STRING=${PREFIX_PATH} -DCMAKE_BUILD_TYPE:STRING=${BUILD_TYPE} -S . -B ${PWD}/build -G "${GENERATOR}"
fi

echo "${ACTION}ing Shopping Cart..."
if [ ! -z "$(ls -A ${PWD}/build)" ]; then
    cmake --build ${PWD}/build --config ${BUILD_TYPE} --target ${TARGET}
fi

if [ ${TARGET} = "clean" ]; then
    if [ -d ${PWD}/build ]; then
        rm -rf build/{.[!.]*,*}
    fi
    if [ -d ${PWD}/bin ]; then
        rm -rf bin/*
    fi
fi