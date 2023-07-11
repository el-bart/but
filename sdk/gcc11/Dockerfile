FROM debian:12
RUN apt-get update && apt-get dist-upgrade -y

RUN apt-get update && apt-get install -y \
    cmake \
    make \
    ninja-build

RUN apt-get update && apt-get install -y \
    googletest \
    libboost-all-dev \
    nlohmann-json3-dev

RUN apt-get update && apt-get install -y g++-11
