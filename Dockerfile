FROM ubuntu:latest

ENV TZ=Europe/London

RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

RUN apt update

RUN mkdir -p /cjson

RUN apt install -y valgrind \
                   build-essential \
                   cmake \
                   autoconf \
                   libtool \
                   libtool-bin \
                   pkg-config \
                   git \
                   wget

RUN cd /tmp && \
    wget https://github.com/libcheck/check/releases/download/0.15.2/check-0.15.2.tar.gz && \
    tar xf check*.tar.gz && \
    rm -f check*.tar.gz && \
    cd check* && \
    autoreconf --install && \
    ./configure && \
    make install && \
    ldconfig

WORKDIR /cjson
