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
  int serv_sock, clnt_sock;
  char message[BUF_SIZE];
  int str_len;

  struct sockaddr_in serv_adr;
  struct sockaddr_in clnt_adr;
  socklen_t clnt_adr_sz;

  // 명령어가 제대로 입력되지 않았다면
  if (argc != 2)
  {
    printf("Usage : %s <port>\n", argv[0]);
    exit(1);
  }

  serv_sock = socket(PF_INET, SOCK_STREAM, 0);
  if (serv_sock == -1)
    error_handling("socket() error");

  memset(&serv_adr, 0, sizeof(serv_adr));
  serv_adr.sin_family = AF_INET;
  serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_adr.sin_port = htons(atoi(argv[1]));

  if (bind(serv_sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1)
    error_handling("bind() error");

  if (listen(serv_sock, 5) == -1)
    error_handling("listen() error");

  clnt_adr_sz = sizeof(clnt_adr);

  // 여기서부터 클라이언트로부터 데이터 받아옴
  clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_adr, &clnt_adr_sz);
  if (clnt_sock == -1)
    error_handling("accept() error");
  else
    printf("클라이언트와 연결되었습니다.\n");

  // read 함수는 연결이 끊기면 0을 반환함
  while ((str_len = read(clnt_sock, message, BUF_SIZE)) != 0)
  {
    // message 안에 전에 보냈던 내용들이 아직 들어있기 때문에,
    // read로 읽은 문자 길이 만큼만 받아들이고, 그 뒤는 모두 무시
    message[str_len] = 0;

    // message 안에 \n이 포함되어 있으므로 message를 찍을때는 개행하지 않음
    printf("클라이언트로부터 온 메시지 : %s", message);
    printf("메시지의 크기 : %d\n", str_len);

    puts("\n클라이언트로 다시 보내는 중...\n");
    write(clnt_sock, message, str_len);
  }

  printf("클라이언트와 연결이 끊어졌습니다. 서버를 종료합니다.\n");

  close(clnt_sock);
  close(serv_sock);

  return 0;
}

void error_handling(char *message)
{
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}