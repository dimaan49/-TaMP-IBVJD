FROM debian

RUN apt update
RUN apt install qt6-base-dev -y
RUN apt install g++ -y
RUN apt install sqlite3 -y
RUN apt install -y make qmake6 -y

EXPOSE 33333

WORKDIR /app

COPY . .

RUN mkdir build && \
    cd build && \
    qmake6 .. && \
    make -j$(nproc) && \
    ls -la

CMD ["./build/tcpserver"]