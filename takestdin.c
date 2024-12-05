#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(){
    char userinput[256];
    fgets(userinput, 10, stdin);
    printf("my stdin: %s", userinput);
}