# client-server-benchwork

Base client

/base:

Base implementation of client-server functionality

Compile:  
on root:
mkdir base_build
cd base_build
cmake ../base
cmake --build .

Run:
./server.exe

./client.exe


Base test

Basic test implementation using client-server
Sends a number a number of times (2000) and reports the time

/base_test:

Compile:  
on root:
mkdir base_test_build
cd base_test_build
cmake ../base_test
cmake --build .

Run:
./server.exe

./client.exe