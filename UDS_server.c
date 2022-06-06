
#include <sys/select.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/un.h>

#define BUF_SIZE 1024
#define FILE_SERVER "./test_server"//file path

int main(void){

    int server_socket;
    int client_socket;
    int client_addr_size;
    int option;

    int max_size;
    int select_result;
    int read_result;

    // sockaddr_un = unix domain socket
    struct sockaddr_un server_addr;
    struct sockaddr_un client_addr;

    char buff_rcv[BUF_SIZE];
    char buff_snd[BUF_SIZE];

    fd_set read_fds, copy_fds;

    // 기존파일 존재하면, 삭제
    if(!access(FILE_SERVER, F_OK))//if file exist
        unlink(FILE_SERVER);


    server_socket = socket(AF_UNIX, SOCK_STREAM, 0); //create unix socket(TCP)

    // 소켓 생성 실패
    if(server_socket == -1){
        printf("server socket error!\n");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sun_family = AF_UNIX;

    strcpy(server_addr.sun_path, FILE_SERVER);


    if(bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) ==-1){
        printf("bind error!\n");
        exit(1);
    }

    // backlog = queue pending number
    if(listen(server_socket, 128) == -1){
        printf("listen error! \n");
        exit(1);
    }

    int r;
    FD_ZERO(&read_fds);
    FD_SET(server_socket,&read_fds);
    max_size = server_socket;

    while(1){

        copy_fds = read_fds;
        select_result = select(max_size+1, &copy_fds, NULL, NULL, NULL);

        if (select_result == -1){
            printf("select error!\n");
            exit(1);
        }

        int i;
        for(i = 3; i< max_size+1; i++ ){
            if(FD_ISSET(i, &copy_fds)){
                if (i == server_socket){
                    client_addr_size = sizeof(client_addr);
                    client_socket = accept(server_socket, (struct sockaddr*)&client_addr, (socklen_t*) &client_addr_size);

                    if (client_socket == -1){
                        printf("accept failed\n");
                        exit(1);
                    }
                    FD_SET(client_socket, &read_fds);

                    if(max_size < client_socket) max_size = client_socket;
                }
                else{
                    read_result = read(client_socket, buff_rcv, BUF_SIZE);

                    // 통신 종료
                    if (read_result == 0){
                        FD_CLR(i, &read_fds);
                        close(i);
                        printf("socket closed: %%i\n");
                    }
                    else {
                        printf("received msg : %s\n", buff_rcv);
                        strcpy(buff_snd, "ACK");
                        write(client_socket, buff_snd, strlen(buff_snd) + 1);
                    }
                }
            }
        }

//        client_addr_size = sizeof(client_addr);
//        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, (socklen_t*) &client_addr_size);
//
//        if(client_socket == -1){
//            printf("accept failed!\n");
//            exit(1);
//        }
//
//        read(client_socket, buff_rcv, BUF_SIZE);
//        printf("received msg : %s\n",buff_rcv);
//
//        strcpy(buff_snd,"ACK");
//
//        write(client_socket, buff_snd, strlen(buff_snd)+1);
//        close(client_socket);
    }
    return 0;
}
