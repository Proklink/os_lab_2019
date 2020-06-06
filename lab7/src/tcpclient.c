#include <arpa/inet.h>
 #include <netinet/in.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <sys/socket.h>
 #include <getopt.h>
 #include <stdbool.h>
 #include <sys/types.h>
#include <unistd.h>

#define BUFSIZE 100
#define SADDR struct sockaddr
#define SIZE sizeof(struct sockaddr_in)

static struct option options[] = {{"bufsize",  required_argument, 0, 0},
                                 {"port",      required_argument, 0, 0},
                                 {"addr",      required_argument, 0, 0},
                                 {0, 0,                          0, 0}};

int getArguments(int argc, char **argv, int *arg, char* addr)
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
                        if (inet_aton(optarg, &inp) == 0) {
                            printf("IP-address is incorrect. Adderess = %s\n", optarg);
                            return -1;
                        }
                        addr = (char *)malloc(strlen(optarg) * sizeof(char));
                        memcpy(addr, optarg, strlen(optarg) * sizeof(char));
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


int main(int argc, char *argv[]) {
  int fd;
  int nread;
  char buf[BUFSIZE];
  char* addr = NULL;
  int arg[2];
  struct sockaddr_in servaddr;
  if (argc < 3) {
    printf("Too few arguments \n");
    exit(1);
  }

  if (getArguments(argc, argv, arg, addr))
        return -1;

    printf("\narguments: bufsize = %d, port = %d, address = %s", arg[0],arg[1],addr);

  if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket creating");
    exit(1);
  }

  memset(&servaddr, 0, SIZE);
  servaddr.sin_family = AF_INET;

  if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) {
    perror("bad address");
    exit(1);
  }

  servaddr.sin_port = htons(atoi(argv[2]));

  if (connect(fd, (SADDR *)&servaddr, SIZE) < 0) {
    perror("connect");
    exit(1);
  }

  write(1, "Input message to send\n", 22);
  while ((nread = read(0, buf, BUFSIZE)) > 0) {
    if (write(fd, buf, nread) < 0) {
      perror("write");
      exit(1);
    }
  }

  close(fd);
  exit(0);
}
