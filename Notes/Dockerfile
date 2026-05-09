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
    libblkid-dev \
    lcov \
    gcovr \
    curl \
    libcurl4-openssl-dev \
    libmount-dev

RUN git clone https://github.com/pistacheio/pistache.git && \
    cd pistache && \
    meson setup build && \
    ninja -C build && \
    ninja -C build install

# =========================
# Project
# =========================
WORKDIR /app

COPY CMakeLists.txt ./
COPY src ./src
COPY tests ./tests
COPY sql ./sql
COPY api-stubs ./api-stubs
COPY external ./external
COPY config.txt ./

# =========================
# Configure with coverage
# =========================
RUN cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Coverage

# =========================
# Build
# =========================
RUN cmake --build build

# =========================
# Run tests + generate coverage
# =========================
COPY run-tests.sh /app/run-tests.sh

CMD ["/app/run-tests.sh"]