#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

using namespace std;

void SendMessages(int s) {
  cout << "Enter Client Name: ";
  string name;
  getline(cin, name);

  string message;
  while (1) {
    getline(cin, message);
    string msg = "(" + name + "): " + message;
    int bytessent = send(s, msg.c_str(), msg.length(), 0);
    if (bytessent < 0) {
      cout << "Error sending message " << endl;
      break;
    }
    if (message == "Q" || message == "q" || message == "quit" ||
        message == "exit") {
      cout << name << " Stopped" << endl;
      break;
    }
  }
  close(s);
}

void ReceiveMessage(int s) {
  char buffer[1024];
  int recvlength;
  string msg = "";
  while (1) {
    recvlength = recv(s, buffer, sizeof(buffer), 0);
    if (recvlength <= 0) {
      cout << "Disconnected from the server" << endl;
      break;
    } else {
      msg = string(buffer, recvlength);
      cout << msg << endl;
    }
  }
}

int main() {
  int s;
  s = socket(AF_INET, SOCK_STREAM, 0);
  if (s < 0) {
    cout << "Invalid socket created " << endl;
    return 1;
  }

  int port = 9090;
  string serveraddress;

  cout << "Enter server IP address: ";
  getline(cin, serveraddress);

  sockaddr_in servaddr;
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(port);

  // Convert server address string to binary form and store in servaddr.sin_addr
  if (inet_pton(AF_INET, serveraddress.c_str(), &servaddr.sin_addr) <= 0) {
    cout << "Invalid address entered" << endl;
    close(s);
    return 1;
  }

  if (connect(s, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
    cout << "Unable to connect to the server" << endl;
    close(s);
    return 1;
  }

  cout << "Connected to the server successfully!" << endl;

  cout << "Type your messages below after entering your name..." << endl;

  thread senderthread(SendMessages, s);
  thread receiver(ReceiveMessage, s);

  senderthread.join();
  receiver.join();

  return 0;
}
