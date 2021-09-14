#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <signal.h>

/*
 * To build and run:
 *   gcc main.c -o main && ./main
 *
 * To connect while it is running:
 *   minicom -D unix\#./cli.sock
 */

#define SOCKET_NAME "cli.sock"
#define BUFFER_SIZE 128


bool server_run(void)
{
  int connection_socket = socket(AF_UNIX, SOCK_STREAM, 0);
  if(connection_socket == -1){
    return false;
  }

  struct sockaddr_un name = {
    .sun_family = AF_UNIX,
    .sun_path = SOCKET_NAME,
  };
  int ret = bind(connection_socket, (const struct sockaddr *)&name, sizeof(name));
  if(ret == -1){
    return false;
  }

  const int maximum_clients = 2;
  ret = listen(connection_socket, maximum_clients);
  if(ret == -1){
    return false;
  }

  // Server loop
  int data_socket;
  signal(SIGPIPE, SIG_IGN); // Ignore SIGPIPE, received when client disconnects
  for(;;){

    printf("Waiting for a client...\n");
    data_socket = accept(connection_socket, NULL, NULL);
    if(data_socket == -1){
      printf("Error trying to accept the client!\n");
      return false;
    }
    printf("Client accepted!\n");


    const char welcome_message[] = "Connection Sucessefull!\n";
    ret = write(data_socket, welcome_message, sizeof(welcome_message));
    if(ret != -1){

      // Communication loop: application code!
      static char buffer[BUFFER_SIZE];
      for(;;){

        buffer[0] = '\0'; // Clear the buffer

        ret = read(data_socket, buffer, sizeof(buffer));
        if(ret == -1){
          break;
        }
        buffer[sizeof(buffer) - 1] = '\0'; // Enforce null-terminating buffer

        printf("Received buffer message: \"%s\"\n", buffer);
        ret = write(data_socket, buffer, strlen(buffer));
        if(ret == -1){
          break;
        }

      }
      printf("Client disconnected!\n");

    }

    close(data_socket);
  }

  close(connection_socket);
  unlink(SOCKET_NAME);

  return 0;
}


int main(void)
{
  server_run();

  return 0;
}
