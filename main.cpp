#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>

int main() {
  int socketFd;
  const unsigned int BUFFER_SIZE = 2048;
  char buffer[BUFFER_SIZE] {};

  // Create a network TCP socket;
  socketFd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

  if (!socketFd) {
    fprintf(stderr, "Unable to create socket");
    return EXIT_FAILURE;
  }

  //Create the local address object
  sockaddr_in sock_in = {
    .sin_family { AF_INET },
    .sin_port { htons(1337) },
    .sin_addr.s_addr { htonl(INADDR_ANY) }
  };

  // Bind the socket to an address from OS
  if (bind(socketFd, (struct sockaddr*)&sock_in, sizeof(sock_in)) == -1) {
    fprintf(stderr, "Unable to bind socket");
    close(socketFd);
    return EXIT_FAILURE;
  }

  // Listen
  if (listen(socketFd, 1) == -1) {
    fprintf(stderr, "Unable to listen on a socket!");
    close(socketFd);
    return EXIT_FAILURE;
  }

  // 
  while (true) {
    int connfd = accept(socketFd, nullptr, nullptr);

    if (connfd == -1) {
      fprintf(stderr, "Unable to accept connections");
      close(socketFd);
      return EXIT_FAILURE;
    }

    read(connfd, buffer, BUFFER_SIZE);
    fprintf(stdout, "Ready bytes:\n%s", buffer);

    if (shutdown(connfd, SHUT_RDWR) == -1) {
      fprintf(stderr, "Faile dot shutdown connection!\n");
      close(connfd);
      close(socketFd);
      return EXIT_FAILURE;
    }

    close(connfd);
  }

  return EXIT_SUCCESS;
}