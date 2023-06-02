#include <utilities.h>

int min_int(int a, int b){
    if(a<b) return a;
    return b;
}
int max_int(int a, int b){
    if(a>b) return a;
    return b;
}
double dist(int x1, int y1, int x2, int y2){
    return sqrt(pow(x1-x2,2)+pow(y1-y2,2));
}
double absl(double x){
    if(x<0){
        x *= -1;
    }
    return x;
}