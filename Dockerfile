FROM debian:jessie

RUN apt-get update
RUN apt-get install -y git build-essential xdg-utils libtag1-dev cmake libmagic-dev libboost-filesystem-dev libboost-program-options-dev libboost-system-dev libboost-regex-dev file
RUN git clone https://github.com/kaethorn/mp3-report.git
RUN cd mp3-report && cmake . && cmake --build .
RUN cd mp3-report && cmake --build . --target test
ENTRYPOINT exec /root/mp3-report/src/MP3Report
