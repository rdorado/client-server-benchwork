#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <unistd.h>
#include <string.h>
#include <iostream>

#define TRUE   1 
#define PORT 8888 
#define MAX_CLIENTS 30 


int main(int argc, char* argv[])
{
  int opt = TRUE;
  int client_socket[MAX_CLIENTS], master_socket;
  struct sockaddr_in address;
  
  for (int i = 0; i < MAX_CLIENTS; i++)  
  {  
        client_socket[i] = 0;  
  }
  
  //create a master socket 
  if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0)  
  {
	std::cout << "Error: socket failed" << std::endl;
    return -1; 
  }
  
  //set master socket to allow multiple connections , 
  //this is just a good habit, it will work without this 
  if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 )  
  {   
	std::cout << "Error: setsockopt failed" << std::endl;
    return -1;  
  }
  
  //type of socket created 
  address.sin_family = AF_INET;  
  address.sin_addr.s_addr = INADDR_ANY;  
  address.sin_port = htons( PORT );  
  
  //bind the socket to localhost port 8888 
  if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0)  
  {
	std::cout << "Error: bind failed" << std::endl;
    return -1;   
  }  
  std::cout << "Listener on port " << PORT << std::endl;    
  
  //try to specify maximum of 3 pending connections for the master socket 
  if (listen(master_socket, 3) < 0)  
  {  
	std::cout << "Error: listen" << std::endl;
    return -1;   
  }
  
  int addrlen = sizeof(address); 
  
  while(TRUE)  
  {
    fd_set readfds;
    
	//clear the socket set 
    FD_ZERO(&readfds);
	
	//add master socket to set 
    FD_SET(master_socket, &readfds);  
    int max_sd = master_socket;
	
    //add child sockets to set 
    for (int i = 0 ; i < MAX_CLIENTS ; i++)  
    {  
      //socket descriptor 
      int sd = client_socket[i];  
                 
      //if valid socket descriptor then add to read list 
      if(sd > 0) FD_SET( sd , &readfds);  
                 
      //highest file descriptor number, need it for the select function 
      if(sd > max_sd) max_sd = sd;  
    } 
	
	std::cout << "Info: Witing for connection" << std::endl;
	
	//wait for an activity on one of the sockets , timeout is NULL , 
    //so wait indefinitely 
    int activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);  
       
    if ((activity < 0) && (errno!=EINTR))  
    {  
      std::cout << "Error: select error" << std::endl;  
    }
	
	//If something happened on the master socket , 
    //then its an incoming connection 
    if (FD_ISSET(master_socket, &readfds))  
    {  
	
	  int new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen);
	  if (new_socket < 0)  
      {  
	    std::cout << "Error: listen" << std::endl;
        return -1;    
      }

      //inform user of socket number - used in send and receive commands 
      std::cout << "INFO: New connection , socket fd is " << new_socket << ", ip: " << inet_ntoa(address.sin_addr) << ", port: " << ntohs(address.sin_port) << std::endl;  	  
                 
      //add new socket to array of sockets 
      for (int i = 0; i < MAX_CLIENTS; i++)  
      {  
        //if position is empty 
        if( client_socket[i] == 0 )  
        {  
          client_socket[i] = new_socket;  
          std::cout << "INFO: Adding to list of sockets as " << i << std::endl;  
          break;  
        }  
      } 
	}
	
    //else its some IO operation on some other socket
    for (int i = 0; i < MAX_CLIENTS; i++)  
    {  
      int sd = client_socket[i];  
                 
      if (FD_ISSET( sd , &readfds))  
      {  
        //Check if it was for closing , and also read the 
        //incoming message or Echo back the message that came in
		int buffer[1] = { 0 };
		int valread = read(sd , buffer, 2);
		std::cout << "Received " << buffer[0] << " from client " << sd << std::endl;
        if (valread == 0)  
        {  
          //Somebody disconnected , get his details and print 
          getpeername(sd , (struct sockaddr*)&address , (socklen_t*)&addrlen);  
          printf("Host disconnected , ip %s , port %d \n" , 
          inet_ntoa(address.sin_addr) , ntohs(address.sin_port));  
                         
          //Close the socket and mark as 0 in list for reuse 
          close( sd );  
          client_socket[i] = 0;  
        }		
        else
        {  
          //set the string terminating NULL byte on the end 
          //of the data read 
          int response[1] = { 0 };
		  response[0] = 3;
          send(sd, response, 2, 0 );  
        }  
      }  
    }  
	
	std::cout << "Info: Connection created" << std::endl;  
  }
  return 0;
}

