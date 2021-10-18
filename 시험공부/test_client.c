#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE 1024

// socket > 초기화 > connect > 처리 > close

int main(int argc, char const *argv[])
{
  //socket
  int client_socket = socket(PF_INET, SOCK_STREAM, 0);

  //초기화
  struct sockaddr_in server_addr;
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr(argv[1]);
  server_addr.sin_port = htons(atoi(argv[2]));

  //connect
  if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1){
    puts("connect() 에러");
    exit(1);
  } else {
    puts("서버와 연결되었습니다.");
  }

  //처리
  while (1)
  {
    char message[BUF_SIZE];
    fputs("전송할 메시지 입력(q는 종료): ", stdout);
    fgets(message, BUF_SIZE, stdin);

    if(!strcmp(message, "q\n") || !strcmp(message, "Q\n")){
      puts("서버와 연결을 종료합니다.");
      break;
    } 
    
    write(client_socket, message, strlen(message));

    char recv_message[BUF_SIZE];
    read(client_socket, recv_message, BUF_SIZE);
    printf("서버로부터 돌아온 메시지: %s", recv_message);
  }

  //close
  close(client_socket);

  return 0;
}
