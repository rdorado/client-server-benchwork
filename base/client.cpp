#include <arpa/inet.h>
#include <sys/socket.h>
#include <iostream>
#include <unistd.h>

#include <string.h>

#define PORT 8888 

int client_connect()
{
  int client_fd;
  struct sockaddr_in serv_addr;
  if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    std::cout << "ERROR: Socket creation error" << std:: endl;
    return -1;
  }
	
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);
  
  if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
  {
    std::cout << "ERROR: Invalid address/ Address not supported" << std:: endl;
    return -1;
  }
  
  int status;
  if ((status = connect(client_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0)
  {
    std::cout << "ERROR: Connection Failed " << std:: endl;
    return -1;
  }
  return client_fd;
}

int main(int argc, char* argv[])
{
  int client_fd = client_connect();
  
  while (true)
  {
    //const char* hello = "Hello from client";
    //send(client_fd, hello, strlen(hello), 0);	  
	  
    int input[1];
	std::cin >> input[0];
	send(client_fd, input, 1, 0);
	
	std::cout << "Input " << input[0] << " sent" << std:: endl;
	
	//char buffer[1024] = { 0 };
    //int valread = read(client_fd, buffer, 1024);
    //std::cout << buffer << std:: endl;
	
	int buffer[1] = { 0 };
    int valread = read(client_fd, buffer, 2);
	
    std::cout << "Received: " << buffer[0] << std:: endl;
  }
  
  // closing the connected socket
  close(client_fd);
  std::cout << "Client disconected" << std:: endl;
  return 0;
}
