FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    meson \
    ninja-build \
    pkg-config \
    git \
    g++ \
    build-essential \
    cmake \
    libudev-dev \
    dosfstools \
    util-linux \
    sudo \
    sqlite3 \
    libsqlite3-dev \
    nlohmann-json3-dev \
    libblkid-dev

RUN git clone https://github.com/pistacheio/pistache.git && \
    cd pistache && \
    meson setup build && \
    ninja -C build && \
    ninja -C build install

WORKDIR /app

COPY . .

RUN cmake -B build -S . && cmake --build build

CMD ["ctest", "--test-dir", "build", "--output-on-failure"]