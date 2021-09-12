#include <netdb.h>
#include <stdio.h>
#include <ctype.h>
#include <arpa/inet.h>

int main(int argc, char **argv)
{
  struct hostent *he;
  struct in_addr addr;
  
  char *host = argv[1];

  if (isalpha(host[0])){  // 주소가 도메인이면
    he = gethostbyname(host);
    printf("%s\n", he->h_addr_list[0]);
  }
  else {                  // 주소가 아이피이면
    addr.s_addr = inet_addr(host);
    he = gethostbyaddr((char *) &addr, 4, AF_INET);
    printf("%s\n", he->h_name);
  }
  return 0;
}
