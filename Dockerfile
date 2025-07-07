FROM ubuntu:22.04

RUN apt update && apt install -y \
    build-essential \
    cmake \
    liballegro5-dev \
    libcurl4-openssl-dev \
    git \
    x11-apps \
    libx11-dev \
    libglu1-mesa-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY . .

RUN make

CMD ["./bin/Debug/sudoku"]

