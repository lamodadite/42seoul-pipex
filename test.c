#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    pid_t pid[3];
    int status;

    for (int i = 0; i < 3; ++i) {
        pid[i] = fork();

        if (pid[i] == 0) {
            // 자식 프로세스의 동작
            printf("자식 프로세스 %d\n", i);
            exit(i + 1);
        }
    }

    // 부모 프로세스가 모든 자식 프로세스의 종료를 기다림
    for (int i = 0; i < 3; ++i) {
        waitpid(pid[i], &status, 0);
        printf("부모 프로세스: 자식 프로세스 %d 종료, 종료 코드: %d\n", pid[i], WEXITSTATUS(status));
    }

    return 0;
}