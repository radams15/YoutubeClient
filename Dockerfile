FROM fedora:34

RUN dnf -y in gtk4-devel xauth cmake make gcc gcc-c++ libxml++-devel
RUN dnf -y in libcurl-devel libadwaita-devel python3-pip
RUN pip3 install yt-dlp
