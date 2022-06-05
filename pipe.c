
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(){
    int  fd[2];
    char buf[255] = "Hello child process from parent";
    int pid;

    // pipe 생성: 성공=0, 실패 = -1
    if (pipe(fd)  == -1 ){
        perror("pipe error : ");
        exit(0);
    }

    // 자식프로세스 생성 fork: 성공 0 -> 자식 프로세스에게, 부모 프로세스에게는 자식 pid값 리턴, 실패 -1
    pid = fork();

    // 여기서부터 두개의 프로세스가 코드 실행....... 병렬로

    if (pid == - 1){
        perror("fork error : ");
        exit(0);
    }

    // 부모 프로세스 write
    if (pid > 0) {
        close(fd[0]);
        write(fd[1], buf, strlen(buf));
    }
    // 자식 프로세스 read
    else{
        close(fd[1]);
        read(fd[0], buf, 255);
        printf("%s\n", buf);
    }
    exit(0);
}