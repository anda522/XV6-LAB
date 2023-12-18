#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void dfs(int pl[2]) {
	int p;
    read(pl[0], &p, sizeof p);
    if (p == -1) exit(0);
    printf("prime %d\n", p);

    int pr[2];
    pipe(pr);

    if (fork() == 0) {
        close(pr[1]);
        close(pl[0]);
        dfs(pr);
    } else {
        close(pr[0]);
        int receive;
        while (read(pl[0], &receive, sizeof receive) && receive != -1) {
            if (receive % p != 0) {
                write(pr[1], &receive, sizeof receive);
            }
        }
        receive = -1;
        write(pr[1], &receive, sizeof receive);
        wait(0);
        exit(0);
    }
    

}
int main(int argc, char **argv) {
    int in_pipe[2];
    pipe(in_pipe);
    
    if (fork() == 0) { // children
        close(in_pipe[1]);
        dfs(in_pipe);
        exit(0);
    } else {
        close(in_pipe[0]);
        int i;
        for (i = 2; i <= 35; ++i) {
            write(in_pipe[1], &i, sizeof(i));
        }
        i = -1;
        write(in_pipe[1], &i, sizeof(i));
    }
    wait(0);
    exit(0);
}
