FROM debian:stable

RUN apt update
RUN apt install -y libsdl2-dev
RUN apt install -y libsdl2-image-dev
RUN apt install -y libsdl2-ttf-dev
RUN apt install -y build-essentials
