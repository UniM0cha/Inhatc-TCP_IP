#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
void error_handling(char *message);

int main(int argc, char *argv[])
{
  int sock;
  char message[BUF_SIZE];
  int str_len, recv_len, recv_cnt;
  struct sockaddr_in serv_adr;

  // 명령어가 제대로 입력되지 않았다면
  if (argc != 3)
  {
    printf("Usage : %s <IP> <port>\n", argv[0]);
    exit(1);
  }

  sock = socket(PF_INET, SOCK_STREAM, 0);
  if (sock == -1)
    error_handling("socket() error");

  memset(&serv_adr, 0, sizeof(serv_adr));
  serv_adr.sin_family = AF_INET;
  serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
  serv_adr.sin_port = htons(atoi(argv[2]));

  if (connect(sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1)
    error_handling("connect() error");
  else
    puts("서버와 연결되었습니다.");

  while (1)
  {
    fputs("메시지 입력(Q는 종료): ", stdout);
    fgets(message, BUF_SIZE, stdin);

    if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
    {
      puts("연결을 종료합니다.");
      break;
    }

    // 기존 코드
    // write(sock, message, strlen(message));
    // str_len = read(sock, message, BUF_SIZE-1);
    // message[str_len] = 0;

    //수정
    puts("서버에 메시지 전송!");
    str_len = write(sock, message, strlen(message));
    recv_len = 0;
    while (recv_len < str_len)
    {
      recv_cnt = read(sock, &message[recv_len], BUF_SIZE - 1);
      if (recv_cnt == -1)
        error_handling("read() error");
      recv_len += recv_cnt;
    }
    message[recv_len] = 0;
    printf("\n서버로부터 온 메시지: %s\n", message);
  }

  close(sock);
  return 0;
}

void error_handling(char *message)
{
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}ㅋ