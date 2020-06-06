#include <arpa/inet.h>
 #include <netinet/in.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <stdbool.h>
 #include <getopt.h>
 #include <sys/socket.h>
 #include <sys/stat.h>
 #include <sys/types.h>
#include <unistd.h>

#define SERV_PORT 20001
#define BUFSIZE 1024
#define SADDR struct sockaddr
#define SLEN sizeof(struct sockaddr_in)

static struct option options[] = {{"bufsize",  required_argument, 0, 0},
                                 {"port",      required_argument, 0, 0},
                                 {0, 0,                          0, 0}};

int getArguments(int argc, char **argv, int *arg)
{

     while (true) {
        int option_index = 0;
        struct in_addr inp;
        int c = getopt_long(argc, argv, "", options, &option_index);

        if (c == -1)
            break;

        switch (c) {
            case 0: {
                switch (option_index) {
                    case 0: {
                        arg[0] = atoi(optarg);
                        if (arg[0] < 0) {
                            printf("The bufsize must be a positive number or 0. Now bufsize argument = %d\n", arg[0]);
                            return -1;
                        }
                        break;
                    }

                    case 1:
                        arg[1] = atoi(optarg);
                        if (arg[1] < 0) {
                            printf("The port must be a positive number. Now port = %d\n", arg[1]);
                            return -1;
                        }
                        break;
                }
                break;
            }

            case '?':
                break;

            default:
                printf("getopt returned character code 0%o?\n", c);
        }
    }
    return 0;
}

int main(int argc, char *argv[]) {
  int sockfd, n;
  int arg[2];//arg[0] - bufsize, arg[1] - port

    if (getArguments(argc, argv, arg))
        return -1;

  char mesg[arg[0]], ipadr[16];
  struct sockaddr_in servaddr;
  struct sockaddr_in cliaddr;

  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket problem");
    exit(1);
  }

  memset(&servaddr, 0, SLEN);
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(arg[1]);

  if (bind(sockfd, (SADDR *)&servaddr, SLEN) < 0) {
    perror("bind problem");
    exit(1);
  }
  printf("SERVER starts... at %d\n", arg[1]);

  while (1) {
    unsigned int len = SLEN;

    if ((n = recvfrom(sockfd, mesg, arg[0], 0, (SADDR *)&cliaddr, &len)) < 0) {
      perror("recvfrom");
      exit(1);
    }
    mesg[n] = 0;

    printf("REQUEST %s      FROM %s : %d\n", mesg,
           inet_ntop(AF_INET, (void *)&cliaddr.sin_addr.s_addr, ipadr, 16),
           ntohs(cliaddr.sin_port));

    if (sendto(sockfd, mesg, n, 0, (SADDR *)&cliaddr, len) < 0) {
      perror("sendto");
      exit(1);
    }
  }
}
