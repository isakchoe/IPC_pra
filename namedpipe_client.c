
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#define PIPENAME "./named_pipe_file2"

int main(void) {
    char msg[255];
    char recv_msg[255] ="se";
    int fd;
    int write_result, read_result, i;

    /* named pipe 열기, Write 전용으로 열기 */
    if ((fd = open(PIPENAME, O_WRONLY)) < 0) {
        printf("fail to open named pipe\n");
        return 0;
    }

    /* Data를 보낸다. */
    for (i = 0; i < 3; i++) {
        snprintf(msg, sizeof(msg), "Send Message[%i]", i);
        if ((write_result = write(fd, msg, sizeof(msg))) < 0 ) {
            printf("fail to call write()\n");
            return 0;
        }
    }

    printf("----------------------------------\n");
//    close(fd);
//
//    fd = open(PIPENAME, O_RDWR);
//
//    if(fd < 0){
//        printf("error\n");
//        return 0;
//    }
//
//    read_result = read(fd, recv_msg, sizeof (recv_msg));
//    printf("dsds %s\n", recv_msg);


    return 0;
}