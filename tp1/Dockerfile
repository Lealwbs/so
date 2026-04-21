FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    git \
    build-essential \
    gdb-multiarch \
    qemu-system-misc \
    qemu-utils \
    gcc-riscv64-linux-gnu \
    binutils-riscv64-linux-gnu \
    gdb \
    bc \
    make \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /tp1-sistemas-operacionais

COPY . .

CMD ["/bin/bash"]