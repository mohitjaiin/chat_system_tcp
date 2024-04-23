Problem Statement: Build a multi-client and single server system and use docker 
technology to build two images.
Pre Requisites : Make sure Docker Desktop is installed on your system
Docker Hub Link: mohitjaiin/chat_system - Docker Image | Docker Hub
1.Create two folders in a directory on your local system with a client and server folder.
2. Create TCP Server Container:(In Server)
Create a C++ program that acts as a TCP server. This program listens for incoming 
connections, receive data from clients, and echo back the received data.
3. Create TCP Client Container(In Client)
Create a C++ program that acts as a TCP client. This program connects to the server container, 
send data, and receive echoed data.
4. Create two separate Dockerfiles: (Each for Client and Server)
Create two Dockerfiles, one for the server container and one for the client container, to build 
the respective images. Each folder has its respective dockerfile.
5. Build Docker Images:
Build Docker images for both the server and client containers in the terminal in the respective 
directory of the target files after docker login and authentication
docker login
>>server>>docker build -t server-image .
>>client>>docker build -t client-image .
6. Run the containers in interactable format(-it):
docker run -it --name server-container server-image
Inspect the server address for server-container using : docker inspect command
docker run -it --name client-container --link server-container client-image
