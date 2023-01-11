#!/bin/bash

#if any statement returns a non-true return value
set -e

if [ $# -ne 1 ]; then
  echo "Error: Script requires exactly one argument (plain, openmp, opencl)"
  exit 1
fi

if [ $1 = "plain" ]; then
  cd plain_c
  make mac-omp-opt
  ./tsp-ant.out
elif [ $1 = "openmp" ]; then
  cd openmp
  make mac-omp-opt
  ./tsp-ant.out
elif [ $1 = "opencl" ]; then
  cd opencl
  make
  ./tsp-ant.out
elif [ $1 = "all" ]; then
  ./run.sh plain
  echo "################################################"
  sleep 1
  ./run.sh openmp
  echo "################################################"
  sleep 1
  ./run.sh opencl
  echo "################################################"
fi