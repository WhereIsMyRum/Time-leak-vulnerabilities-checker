FROM gcc:latest
COPY ./time-leak-detector /usr/src/app
WORKDIR /usr/src/app

RUN g++ -g main.cpp src/time_leak/**.cpp -o main