#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080
#define SERVER_BACKLOG 10
#define REQUEST_BUFFER_SIZE 100

int server_fd;

void *handle_client(void *);
char *build_response(void);

int main() {
  if ((server_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
    perror("Socket Failed");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(PORT);

  if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) <
      0) {
    perror("Socket binding Failed");
    exit(EXIT_FAILURE);
  }

  if (listen(server_fd, SERVER_BACKLOG) < 0) {
    perror("Listening to Request Failed");
    exit(EXIT_FAILURE);
  }

  while (1) {
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    int *client_fd = malloc(sizeof(int));

    if ((*client_fd = accept(server_fd, (struct sockaddr *)&client_addr,
                             &client_addr_len)) < 0) {
      perror("Client Connection Failed");
      continue;
    }

    pthread_t thread_id;
    pthread_create(&thread_id, NULL, handle_client, client_fd);
    pthread_detach(thread_id);
  }

  return 0;
}

void *handle_client(void *arg) {
  int client_fd = *(int *)arg;

  char *buffer = malloc(REQUEST_BUFFER_SIZE * sizeof(buffer));

  recv(client_fd, buffer, REQUEST_BUFFER_SIZE, 0);

  printf("Recieved from Client: %s", buffer);

  send(client_fd, buffer, REQUEST_BUFFER_SIZE, 0);

  close(client_fd);
  free(buffer);
  free(arg);

  return NULL;
}

char* build_response() {
  return NULL;
}
