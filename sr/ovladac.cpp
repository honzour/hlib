#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char **argv) {
  int soc;
  sockaddr_in serverSock; 
  hostent *host;

  soc = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  host = gethostbyname("localhost");
  serverSock.sin_family = AF_INET;
  serverSock.sin_port = htons(5002);
  memcpy(&(serverSock.sin_addr), host->h_addr, host->h_length);
  if (connect(soc, (sockaddr *)&serverSock, sizeof(serverSock)) == -1) {
    puts("nepodarilo se pripojit");
    return 1;
  }
  char co;
  if (argc > 1) {
    co = 'k';
  } else {
    co = 'v';
  }
  printf("%i\n", send(soc, (void *) &co, 1, 0));
  close(soc);
  return 0;
}

