#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char *message);

int main(int argc, char *argv[])
{
  int sock;
  char message[BUF_SIZE];
  struct sockaddr_in my_adr, your_adr;
  socklen_t adr_sz;
  int str_len;

  if (argc != 2)
  {
    printf("Usage : %s <port>\n", argv[0]);
    exit(1);
  }

  sock = socket(PF_INET, SOCK_DGRAM, 0);
  if (sock == -1)
    error_handling("socket() error");

  memset(&my_adr, 0, sizeof(my_adr));
  my_adr.sin_family = AF_INET;
  my_adr.sin_addr.s_addr = htonl(INADDR_ANY);
  my_adr.sin_port = htons(atoi(argv[1]));

  if (bind(sock, (struct sockaddr *)&my_adr, sizeof(my_adr)) == -1)
    error_handling("bind() error");

  for (int i = 0; i < 3; i++)
  {
    sleep(5); // delay 5 sec.
    adr_sz = sizeof(your_adr);
    str_len = recvfrom(sock, message, BUF_SIZE, 0,
                       (struct sockaddr *)&your_adr, &adr_sz);

    printf("receive message from host2 : %s\n", message);
    printf("resending message to host2...\n");
    sendto(sock, message, sizeof(message), 0,
           (struct sockaddr *)&your_adr, sizeof(your_adr));
  }
  close(sock);
  return 0;
}

void error_handling(char *message)
{
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}