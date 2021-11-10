//echo_mpserv.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char *message);
void read_childproc(int sig);

int main(int argc, char *argv[])
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;

	pid_t pid;
	struct sigaction act;
	socklen_t adr_sz;
	int str_len, state;
	char buf[BUF_SIZE];
	if (argc != 2)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	act.sa_handler = read_childproc;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	state = sigaction(SIGCHLD, &act, 0);
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	if (bind(serv_sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error");
	if (listen(serv_sock, 5) == -1)
		error_handling("listen() error");

	while (1)
	{
		adr_sz = sizeof(clnt_adr); // 클라이언트와 연결되면
		clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_adr, &adr_sz);
		if (clnt_sock == -1)
			continue; // while에 맨끝으로 보냄
		else				//0이 아니면
			puts("new client connected...");
		pid = fork();	 //클라이언트가 들어와서 승인하면 자식 프로세스를 생성
		if (pid == -1) //함수 실패시 - 거의 실행 안됨
		{
			close(clnt_sock);
			continue;
		}
		if (pid == 0) //보통 여기로 옴 - 클라이언트가 real 통신
		{
			close(serv_sock);																				//자식은 서버소캣 필요없음 그래서 닫음
			while ((str_len = read(clnt_sock, buf, BUF_SIZE)) != 0) //에코라 읽고
				write(clnt_sock, buf, str_len);												//쏴줌 - 클라이언트가 q와 enter를 치면 0이 날아감

			close(clnt_sock);								// 할일이 끝나서 콜라이언트 소캣을 닫아줌
			puts("client disconnected..."); // 클라이언트 끊었다 출력
			return 0;												// 시그 차일드가 발생 -> read- childproc 핸들러 실행
		}
		else
			close(clnt_sock); //클라이언트 소캣 닫아주고
	}
	close(serv_sock); //모르겠습니다 머라구요?
	return 0;
}

//read_childproc 핸들러
void read_childproc(int sig)
{
	pid_t pid;
	int status;
	pid = waitpid(-1, &status, WNOHANG);
	if (WIFEXITED(status))
	{
		printf("removed proc id: %d \n", pid);
		printf("return value : %d\n", WEXITSTATUS(status));
	}
}
void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}