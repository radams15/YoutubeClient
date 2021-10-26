FROM fedora:34

RUN dnf -y in gtk4-devel xauth cmake make gcc gcc-c++ libxml++-devel
RUN dnf -y in libcurl-devel libadwaita-devel python3-pip
RUN pip3 install yt-dlp

RUN curl -L https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage --output /bin/linuxdeploy && chmod +x /bin/linuxdeploy
RUN dnf in -y fuse fuse-libs
