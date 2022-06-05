

#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>

#define PIPENAME "./named_pipe_file2"

int main(void) {
    char msg[255];
    char send_msg[255] = "send from server";
    int fd, fd2;
    int result_fifo, read_result;
    int w_result;

    /* 기존에 named pipe가 있으면 삭제 */
    // access 해당 파일이 특정 모드(R_OK, W_OK, E_OK, F_OK)로 가능한지 확인 -> 성공 0, 실패 -1
    if (access(PIPENAME,F_OK) == 0){
        unlink(PIPENAME);
    }

    /* named pipe 생성하기 */
    // mkfifo: fifo named pipe 생성, 성공 0 실패 -1
    // 모드 인자 -> 소유자, 그룹, 이외 사용자 권한  666 != 0666?
    result_fifo = mkfifo(PIPENAME, 0666);
    if (result_fifo < 0) {
        printf("fail to make named pipe\n");
        return 0;
    }

    /* named pipe 열기, Read Write가능 해야 한다  최초 개방에는 write를 열어야 read 가 가능하다. */
    // 성공 음이 아닌 정수= fd, 실패 -1
    if ((fd = open(PIPENAME, O_RDWR)) < 0) {
        printf("fail to open named pipe\n");
        return 0;
    }

    // 대기 상태,, like select
    int k = 0;
    while (k<3) {
        if ((read_result = read(fd, msg, sizeof(msg))) < 0) {
            printf("fail to call read()\n");
            return 0;
        }
        printf("recv: %s\n", msg);
        k++;
    }

    close(fd);

    fd2 = open(PIPENAME, O_WRONLY);
    if(fd2 < 0) return 0;

    // 반이중 통신,,, 무전기
    // A  --> B
    // A  < -- B
    // 동일한 네임드 파이프로 불가능한가??
    w_result = write(fd2, send_msg, sizeof (send_msg));

    return 0;
}