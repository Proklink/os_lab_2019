#include <netinet/in.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <getopt.h>
 #include <stdbool.h>
 #include <sys/socket.h>
 #include <sys/types.h>
#include <unistd.h>

//#define SERV_PORT 10050
//#define BUFSIZE 100
#define SADDR struct sockaddr

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
  const size_t kSize = sizeof(struct sockaddr_in);

  int lfd, cfd;
  int nread;
  int arg[2];//arg[0] - bufsize, arg[1] - port
  struct sockaddr_in servaddr;
  struct sockaddr_in cliaddr;

   if (argc < 3) {
    printf("Too few arguments \n");
    exit(1);
  }

  if (getArguments(argc, argv, arg))
        return -1;

  printf("\narguments: bufsize = %d, port = %d\n", arg[0],arg[1]);
  
  char buf[arg[0]];

  if ((lfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket");
    exit(1);
  }

  memset(&servaddr, 0, kSize);
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(arg[1]);

  if (bind(lfd, (SADDR *)&servaddr, kSize) < 0) {
    perror("bind");
    exit(1);
  }

  if (listen(lfd, 5) < 0) {
    perror("listen");
    exit(1);
  }
  printf("\nlistening at %d\n", arg[1]);

  while (1) {
    unsigned int clilen = kSize;

    if ((cfd = accept(lfd, (SADDR *)&cliaddr, &clilen)) < 0) {
      perror("accept");
      exit(1);
    }
    printf("connection established\n");

    while ((nread = read(cfd, buf, arg[0])) > 0) {
        int err;
      write(1, &buf, nread);
      err = send(cfd, "Messadge is recieved", 20, 0);
            if (err < 0) {
                fprintf(stderr, "Can't send data to client\n");
                break;
            }
    }

    if (nread == -1) {
      perror("read");
      exit(1);
    }
    close(cfd);
  }
}
