#include <netinet/in.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include <stdbool.h>
 #include <getopt.h>
 #include <arpa/inet.h>
 #include <string.h>
 #include <sys/socket.h>
 #include <sys/stat.h>
 #include <sys/types.h>
#include <unistd.h>

//#define SERV_PORT 20001
//#define BUFSIZE 1024
#define SADDR struct sockaddr
#define SLEN sizeof(struct sockaddr_in)

static struct option options[] = {{"bufsize",  required_argument, 0, 0},
                                 {"port",      required_argument, 0, 0},
                                 {"addr",      required_argument, 0, 0},
                                 {0, 0,                          0, 0}};

int getArguments(int argc, char **argv, int *arg, char** addr)
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

                    case 2: {
                        //arg[2] = atoi(optarg);
                        *addr = (char *)malloc(strlen(optarg) * sizeof(char));
                        memcpy(*addr, optarg, strlen(optarg) * sizeof(char));
                        break;
                    }
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

int main(int argc, char **argv) {
  int sockfd, n;
  int arg[2];//arg[0] - bufsize, arg[1] - port
  char *addr;//IP-address

    if (getArguments(argc, argv, arg, &addr))
        return -1;

  char sendline[arg[0]], recvline[arg[0] + 1];
  struct sockaddr_in servaddr;
  struct sockaddr_in cliaddr;

  if (argc < 4) {//изменить
    printf("usage: client <IPaddress of server>\n");
    exit(1);
  }

  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(arg[1]);

  if (inet_pton(AF_INET, addr, &servaddr.sin_addr) < 0) {
    perror("inet_pton problem");
    exit(1);
  }
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket problem");
    exit(1);
  }

  write(1, "Enter string\n", 13);

  while ((n = read(0, sendline, arg[0])) > 0) {
    if (sendto(sockfd, sendline, n, 0, (SADDR *)&servaddr, SLEN) == -1) {
      perror("sendto problem");
      exit(1);
    }

    if (recvfrom(sockfd, recvline, arg[0], 0, NULL, NULL) == -1) {
      perror("recvfrom problem");
      exit(1);
    }

    printf("REPLY FROM SERVER= %s\n", recvline);
  }
  close(sockfd);
}
