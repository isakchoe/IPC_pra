


#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define KEY_NUM     9527
#define MEM_SIZE    1024

int main(void) {

    int shm_id;
    void *shm_addr;
    int count;

    // 성공 id 값, 실패 -1 :  shared memory get!
    // key, 사이즈, 옵션(공유메모리 생성,권한)
    shm_id = shmget((key_t)KEY_NUM, MEM_SIZE, IPC_CREAT|0666);

    if (shm_id == -1){
        printf("공유메모리 생성 실패\n");
        return -1;
    }

    // 성공 attach 된 프로세스 주소, 실패 -1
    // shared memeory 를 2번째 인자 주소에 붙인다. 0이면 자동 매핑
    shm_addr = shmat(shm_id, 0, 0);
    if ( ( void *)-1 ==  shm_addr ){
        printf( "공유 메모리 첨부 실패\n");
        return -1;
    }

    count = 0;
    while( 1 ){
        sprintf( (char *)shm_addr, "%d", count++);       // 공유 메모리에 카운터 출력
        sleep( 1);
    }

    return 0;
}