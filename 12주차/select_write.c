#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE 30

int main(int argc, char *argv[])
{
	fd_set writes, temps;
	int result, str_len;
	char buf[BUF_SIZE];
	struct timeval timeout;

	FD_ZERO(&writes);
	FD_SET(1, &writes); // 1 is standard output(console)

	/*
	timeout.tv_sec=5;
	timeout.tv_usec=5000;
	*/

	while (1)
	{
		temps = writes;
		timeout.tv_sec = 5;
		timeout.tv_usec = 0;
		result = select(2, 0, &temps, 0, &timeout);
		if (result == -1)
		{
			puts("select() error!");
			break;
		}
		else if (result == 0)
		{
			puts("Time-out!");
		}
		else
		{
			if (FD_ISSET(1, &temps))
			{
				printf("Standard output\n");
				break;
			}
		}
	}
	return 0;
}