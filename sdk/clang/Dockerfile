FROM debian:13
RUN apt-get update && apt-get dist-upgrade -y

RUN apt-get update && apt-get install -y \
    cmake \
    make \
    ninja-build

RUN apt-get update && apt-get install -y \
    libcatch2-dev \
    googletest \
    libboost-all-dev \
    nlohmann-json3-dev

RUN apt-get update && apt-get install -y \
    gdb \
    gdbserver \
    vim

RUN apt-get update && apt-get install -y \
    clang \
    llvm
