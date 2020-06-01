#include <stdbool.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <unistd.h>

 #include <errno.h>
 #include <getopt.h>
 #include <netdb.h>
 #include <netinet/in.h>
 #include <netinet/ip.h>
 #include <sys/socket.h>
#include <sys/types.h>

struct Server {
  char ip[255];
  int port;
};

uint64_t MultModulo(uint64_t a, uint64_t b, uint64_t mod) {
  uint64_t result = 0;
  a = a % mod;
  while (b > 0) {
    if (b % 2 == 1)
      result = (result + a) % mod;
    a = (a * 2) % mod;
    b /= 2;
  }

  return result % mod;
}

bool ConvertStringToUI64(const char *str, uint64_t *val) {
  char *end = NULL;
  unsigned long long i = strtoull(str, &end, 10);
  if (errno == ERANGE) {
    fprintf(stderr, "Out of uint64_t range: %s\n", str);
    return false;
  }

  if (errno != 0)
    return false;

  *val = i;
  return true;
}

int main(int argc, char **argv) {
  uint64_t k = -1;
  uint64_t mod = -1;
  FILE* pf;
  char servers[255] = {'\0'}; // TODO: explain why 255

  while (true) {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"k", required_argument, 0, 0},
                                      {"mod", required_argument, 0, 0},
                                      {"servers", required_argument, 0, 0},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "", options, &option_index);

    if (c == -1)
      break;

    switch (c) {
    case 0: {
      switch (option_index) {
      case 0:
        ConvertStringToUI64(optarg, &k);
        if (k <= 0)
        {
            printf("\nthe argument of the factorial should be positive");
            return -1;
        }
        break;
      case 1:
        ConvertStringToUI64(optarg, &mod);
         if (mod <= 0)
        {
            printf("\nthe module should be positive");
            return -1;
        }
        break;
      case 2:
        memcpy(servers, optarg, strlen(optarg));
        if((pf=fopen(servers,"r"))==NULL)
        {
            printf("\nopenning file failed");
            return -1;
        }
        else
            fclose(pf);
        break;
      default:
        printf("Index %d is out of options\n", option_index);
      }
    } break;

    case '?':
      printf("Arguments error\n");
      break;
    default:
      fprintf(stderr, "getopt returned character code 0%o?\n", c);
    }
  }

  if (k == -1 || mod == -1 || !strlen(servers)) {
    fprintf(stderr, "Using: %s --k 1000 --mod 5 --servers /path/to/file\n",
            argv[0]);
    return 1;
  }

  if((pf=fopen(servers,"r"))==NULL)
    {
        printf("\nopenning file failed");
        fclose(pf);
        return -1;
    }
  struct Server *to; 
  unsigned int servers_num = 1;
  while ( !feof(pf) )  
  {
    to = realloc(to, sizeof(struct Server) * servers_num);
    if ( fscanf(pf, "%s:%d\n",to[0].ip, &(to[0].port)) < 2 )
    {
        printf("\nreading from file error");
        fclose(pf);
        break;
    }

    //to[0].port = 20001;
    //memcpy(to[0].ip, "127.0.0.1", sizeof("127.0.0.1"));

    servers_num++;
  }
    fclose(pf);

  // TODO: work continuously, rewrite to make parallel
  for (int i = 0; i < servers_num; i++) {
    struct hostent *hostname = gethostbyname(to[i].ip);
    if (hostname == NULL) {
      fprintf(stderr, "gethostbyname failed with %s\n", to[i].ip);
      exit(1);
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(to[i].port);
    server.sin_addr.s_addr = *((unsigned long *)hostname->h_addr);

    int sck = socket(AF_INET, SOCK_STREAM, 0);
    if (sck < 0) {
      fprintf(stderr, "Socket creation failed!\n");
      exit(1);
    }

    if (connect(sck, (struct sockaddr *)&server, sizeof(server)) < 0) {
      fprintf(stderr, "Connection failed\n");
      exit(1);
    }

    // TODO: for one server
    // parallel between servers
    uint64_t begin = 1;
    uint64_t end = k;

    char task[sizeof(uint64_t) * 3];
    memcpy(task, &begin, sizeof(uint64_t));
    memcpy(task + sizeof(uint64_t), &end, sizeof(uint64_t));
    memcpy(task + 2 * sizeof(uint64_t), &mod, sizeof(uint64_t));

    if (send(sck, task, sizeof(task), 0) < 0) {
      fprintf(stderr, "Send failed\n");
      exit(1);
    }

    char response[sizeof(uint64_t)];
    if (recv(sck, response, sizeof(response), 0) < 0) {
      fprintf(stderr, "Recieve failed\n");
      exit(1);
    }

    // TODO: from one server
    // unite results
    uint64_t answer = 0;
    memcpy(&answer, response, sizeof(uint64_t));
    printf("answer: %llu\n", (unsigned long long)answer);

    close(sck);
  }
  free(to);

  return 0;
}
