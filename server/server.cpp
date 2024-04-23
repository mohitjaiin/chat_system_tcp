#include <algorithm>
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>
#include <vector>

using namespace std;

// Global variables
int listenSocket;
vector<int> clients;

void InteractWithClient(int clientSocket) {
  // send/recv client
  cout << "Client connected " << endl;
  char buffer[1024];
  while (1) {
    int bytesrecvd = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesrecvd <= 0) {
      cout << "Client disconnected" << endl;
      break;
    }
    string message(buffer, bytesrecvd);
    cout << message << endl;

    // Broadcast message to all clients except the sender
    for (auto &client : clients) {
      if (client != clientSocket) {
        send(client, message.c_str(), message.length(), 0);
      }
    }
  }

  // Remove client from the list
  auto it = find(clients.begin(), clients.end(), clientSocket);
  if (it != clients.end()) {
    clients.erase(it);
  }
  close(clientSocket);
}

void ServerMessageSender() {
  while (1) {
    string serverMessage;
    getline(cin, serverMessage); // Read message from server terminal
    serverMessage = "Server: " + serverMessage;

    // Broadcast server message to all clients
    for (auto &client : clients) {
      send(client, serverMessage.c_str(), serverMessage.length(), 0);
    }
  }
}

int main() {
  cout << "Server Side" << endl;

  listenSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (listenSocket < 0) {
    cout << "Failed to create socket" << endl;
    return 1;
  }

  // address structure
  sockaddr_in servaddr;
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(9090);
  servaddr.sin_addr.s_addr = INADDR_ANY;

  if (bind(listenSocket, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
    cout << "Failed to bind socket" << endl;
    close(listenSocket);
    return 1;
  }

  if (listen(listenSocket, SOMAXCONN) < 0) {
    cout << "Failed to listen to client" << endl;
    close(listenSocket);
    return 1;
  }

  cout << "Server is listening..." << endl;

  // Start a separate thread for sending server messages
  thread messageSender(ServerMessageSender);
  messageSender.detach();

  while (1) {
    int clientSocket = accept(listenSocket, nullptr, nullptr);
    if (clientSocket < 0) {
      cout << "Invalid client socket " << endl;
    }
    clients.push_back(clientSocket);

    // Start a new thread to interact with the connected client
    thread clientThread(InteractWithClient, clientSocket);
    clientThread.detach();
  }

  close(listenSocket);

  return 0;
}