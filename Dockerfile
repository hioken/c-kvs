FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

# 必要なパッケージのインストール (C/C++開発ツールとValgrind)
RUN apt-get update && apt-get install -y \
    build-essential \
    valgrind \
    gdb \
    vim \
    git \
    curl \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
CMD ["/bin/bash"]