

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/un.h>

#define BUFF_SIZE 1024
#define FILE_SERVER "./test_server"//file path


int main(void){
    int client_socket;
    char * msg = "Hello World!";
    struct sockaddr_un server_addr;
    char buff[BUFF_SIZE];


    client_socket = socket(AF_UNIX, SOCK_STREAM, 0);

    if(client_socket == -1){
        printf("socket create fail!\n");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, FILE_SERVER);

    if(connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1){
        printf("connect fail!\n");
        exit(1);
    }

    write(client_socket, msg, strlen(msg)+1);
    read(client_socket, buff, BUFF_SIZE);
    printf("recv msg from server : %s\n",buff);

    close(client_socket);

    return 0;
}
