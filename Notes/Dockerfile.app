FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

# =========================
# Dependencies
# =========================

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
    curl \
    libcurl4-openssl-dev \
    libmount-dev

# =========================
# Pistache
# =========================

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
# Configure
# =========================

RUN cmake -S . -B build -G Ninja \
    -DCMAKE_BUILD_TYPE=Debug \
    -DBUILD_HTTP_SERVER=ON

# =========================
# Build
# =========================

RUN cmake --build build

# =========================
# Healthcheck
# =========================

HEALTHCHECK --interval=2s --timeout=2s --retries=30 \
    CMD curl -f http://localhost:8080/api/v1/whitelist/ || exit 1

# =========================
# Run app
# =========================

CMD ["./build/app"]