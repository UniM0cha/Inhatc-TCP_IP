#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define BUF_SIZE 1024

// socket > 초기화 > bind > listen > accept > 처리 > close

int main(int argc, char const *argv[])
{
  // socket
  int server_sock = socket(PF_INET, SOCK_STREAM, 0);

  // 초기화
  struct sockaddr_in server_addr;
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htons(INADDR_ANY);
  server_addr.sin_port = htons(atoi(argv[1]));

  // bind
  bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr));

  // listen
  listen(server_sock, 5);

  // accept
  struct sockaddr_in client_addr;
  socklen_t client_addr_size = sizeof(client_addr);

  int client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_addr_size);
  if (client_sock == -1){
    puts("accept() 에러");
    exit(1);
  } else {
    puts("클라이언트와 연결되었습니다.");
  }

  // 처리
  while (1)
  {
    char message[BUF_SIZE];
    int msg_len = read(client_sock, message, BUF_SIZE);
    if (msg_len == 0)
    {
      puts("클라이언트와 연결을 종료합니다.");
      break;
    }
    printf("클라이언트로부터 도착한 메시지 : %s", message);

    puts("클라이언트에게 다시 보내는 중");
    write(client_sock, message, msg_len);
  }

  // close
  close(server_sock);
  close(client_sock);

  return 0;
}
