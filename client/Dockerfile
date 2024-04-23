FROM ubuntu:latest

# Install g++ compiler
RUN apt-get update && apt-get install -y g++

# Set the working directory
WORKDIR /main

# Copy the C++ source code into the container
COPY client.cpp .

# Compile the C++ code
RUN g++ -o client client.cpp

# Specify the command to run the compiled program
CMD ["./client"]