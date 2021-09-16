// 참고 문헌 : https://novice-programmer-story.tistory.com/18

#include <netdb.h>
#include <stdio.h>
#include <ctype.h>
#include <arpa/inet.h>

int main(int argc, char **argv)
{
  struct hostent *he;
  struct in_addr addr;
  
  char *host = argv[1];

  // 주소가 도메인이면
  if (isalpha(host[0])){
    // 도메인 주소로 ip 받아옴
    he = gethostbyname(host);

    /*
    struct hostent {
      char*   h_name;         // 호스트 이름
      char**  h_aliases;      // 호스트 별명들
      int	    h_addrtype;     // 주소종류(AF_INET=2)
      int	    h_length;       // 주소크기(바이트)
      char**  h_addr_list;    // IP주소 리스트(포인터배열)
    }

    char * inet_ntoa(struct in_addr adr);

    h_addr (=h_addr_list[0]) 자체는 char* 타입으로 in_addr 구조체 변수의 주소를 저장하고 있음.
    따라서 char* 타입을 in_addr* 로 자료형을 변환하고
    inet_ntoa() 함수에게 넘겨서 IP주소를 문자열로 받아옴.
    */
    printf("Domain To IP = %s\n", inet_ntoa(*(struct in_addr*)he->h_addr));
  }

  // 주소가 IP 주소이면
  else {
    /*
    inet_addr() : 함수로 10진수로 표현된 문자열을 32비트 정수형으로 변환
    
    struct in_addr{
      in_addr_t s_addr;
    }
    in_addr 구조체의 s_addr 에 저장
    s_addr 의 자료형은 in_addr_t
    in_addr_t 는 32비트 IPv4 인터넷 주소
    */

    addr.s_addr = inet_addr(host);

    // *gethostbyaddr(const char *in_addr, int len, int family);
    // char* 형식의 in_addr 의 주소값을 주어야 하기 때문에 (char *) &addr 을 적어준다.
    he = gethostbyaddr((char*)&addr, 4, AF_INET);   // ip 주소로 도메인 이름을 받아옴
    printf("IP To Domain = %s\n", he->h_name);
  }
  return 0;
}
