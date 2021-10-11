#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>

int main(void)
{
  char buf[] = "end\n";
  int sd1, sd2, fd1, fd2;
  sd1 = socket(PF_INET, SOCK_STREAM, 0);
  fd1 = open("test1.dat", O_CREAT | O_RDONLY);
  fd2 = open("test2.dat", O_CREAT | O_WRONLY);
  sd2 = socket(PF_INET, SOCK_DGRAM, 0);
  write(fd2, buf, 4);

  printf("file descripter of sd1 : %d\n", sd1);
  printf("file descripter of fd1 : %d\n", fd1);
  printf("file descripter of fd2 : %d\n", fd2);
  printf("file descripter of sd2 : %d\n", sd2);

  close(sd1);
  close(fd1);
  close(fd2);
  close(sd2);
}