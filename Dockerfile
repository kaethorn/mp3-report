FROM debian:stretch

RUN apt-get update
RUN apt-get install -y git build-essential xdg-utils libtag1-dev cmake libmagic-dev libboost-filesystem-dev libboost-program-options-dev libboost-system-dev libboost-regex-dev file
ADD https://api.github.com/repos/kaethorn/mp3-report/git/refs/heads/master version.json
RUN git clone --depth=1 https://github.com/kaethorn/mp3-report.git
RUN cd mp3-report && cmake . && cmake --build .
RUN cd mp3-report && cmake --build . --target test
ENTRYPOINT [ "/mp3-report/src/MP3Report" ]
