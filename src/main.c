#include <stdio.h>
#include <programstate.h>

int main(int argc, char *argv[]){
    struct context *ctx = new_context();
    change_state(ctx,stdin,read_menu);
    return 0;
}