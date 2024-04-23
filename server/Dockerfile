FROM ubuntu:latest
WORKDIR /main

# Update the packages in Ubuntu
RUN apt-get update && apt-get install -y --fix-missing g++

# Copy and compile the C++ file
COPY server.cpp .
RUN g++ -o server server.cpp -lpthread

# Set the command to run on container start
CMD ["./server"]