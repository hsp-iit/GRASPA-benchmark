FROM ubuntu:bionic
LABEL maintainer="fabrizio.bottarel@iit.it"

# Set up env variables

ENV HOME=/root \
    DEBIAN_FRONTEND=noninteractive \
    LANG=en_US.UTF-8 \
    LANGUAGE=en_US.UTF-8 \
    LC_ALL=C.UTF-8 \
    DISPLAY=:0.0 \
    DISPLAY_WIDTH=1920 \
    DISPLAY_HEIGHT=1080 \
    RUN_XTERM=yes \
    RUN_FLUXBOX=yes \
    BUILD_TYPE=Release \
    SHELL=/bin/bash

# Install git, supervisor, VNC, & X11 packages

RUN set -ex; \
    apt update; \
    apt install -y \
      bash \
      fluxbox \
      git \
      net-tools \
      novnc \
      supervisor \
      x11vnc \
      xterm \
      xvfb

# Install build tools and accessories

RUN apt install -y apt-utils \
                   software-properties-common \
                   tmux \
                   nano \
                   build-essential \
                   cmake \
                   cmake-curses-gui

# Install pyhton and pip3

RUN apt install -y python3 \
                   python3-dev \
                   python3-pip \
                   python3-setuptools &&\
    pip3 install --upgrade pip

# Use the /workspace directory as entry point

RUN mkdir workspace
WORKDIR /workspace

# Install Simox dependencies (Qt4)

RUN apt install -y libboost-all-dev \
                   libeigen3-dev libsoqt4-dev \
                   libcoin80-dev \
                   libqt4-dev \
                   libnlopt-dev \
                   librapidxml-dev

# Get and build Simox v2.3.74

RUN git clone https://gitlab.com/Simox/simox.git
RUN cd simox/build && \
    git checkout v2.3.74 && \
    cmake .. \
    -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
    -DSimox_USE_QT4=ON \
    -DSimox_BUILD_SimDynamics=OFF \
    -DSimDynamics_USE_BULLET=OFF \
    -DSimDynamics_USE_BULLET_USE_GLU=OFF &&\
    make -j4 install
RUN rm -Rf simox

# Create environmental variable for simox install dir

ENV Simox_DIR=/usr/local

# Clone the GRASPA benchmark repository
RUN git clone https://github.com/hsp-iit/GRASPA-benchmark
RUN mkdir GRASPA-benchmark/build && cd GRASPA-benchmark/build && \
    cmake .. && make install

# Install the Python packages
RUN pip3 install -r GRASPA-benchmark/requirements.txt

# Clone the experiment data repository
RUN git clone https://github.com/hsp-iit/GRASPA-test.git

# Set up .bashrc for root user (now in entrypoint)
# RUN echo "alias python='/usr/bin/python3.6'" >> ~/.bashrc
# RUN echo "PS1='\[\e]0;\u \w\a\]\[\033[01;32m\]\u\[\033[00m\] \[\033[01;34m\]\w\[\033[00m\] \$ '" >> ~/.bashrc
# RUN echo "export GRASPA_DIR=/workspace/GRASPA-benchmark" >> ~/.bashrc
# RUN echo "export GRASPA_TEST_DIR=/workspace/GRASPA-test/experiment_data/right_arm" >> ~/.bashrc
# RUN echo "export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib" >> ~/.bashrc

# Set up configs and entrypoint
COPY supervisord.conf entrypoint.sh /app/
COPY conf.d /app/conf.d/

CMD ["/app/entrypoint.sh"]
EXPOSE 8080
