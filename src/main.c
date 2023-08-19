#include <stdio.h>
#include <programstate.h>
#include <signal.h>

struct context *ctx;

void sigint_handler(int signo);

int main(int argc, char *argv[]){
    signal(SIGINT,sigint_handler);
    ctx = new_context();
    if(ctx==NULL){
        printf("Could not create context\n");
        return -1;
    }
    change_state(ctx,stdin,read_menu);
    return 0;
}

void sigint_handler(int signo){
    destroy_context(ctx);
    exit(-1);
}