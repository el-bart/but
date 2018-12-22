FROM debian:testing

RUN apt-get update
RUN apt-get dist-upgrade -y

RUN apt-get install -y cmake
RUN apt-get install -y make ninja-build

RUN apt-get install -y g++-6
RUN apt-get install -y g++-7
RUN apt-get install -y g++-8

RUN apt-get install -y clang-6.0 llvm-6.0
RUN apt-get install -y clang-7 llvm-7

RUN apt-get install -y g++
RUN apt-get install -y clang llvm

RUN apt-get install -y googletest
RUN apt-get install -y libboost-all-dev
RUN apt-get install -y nlohmann-json-dev
