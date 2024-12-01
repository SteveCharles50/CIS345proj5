#includ <sys/types.h>
#inlcude <fcntl.h>
#include <stdlib.h>
#inlcude <unistd.h>

int main(argc, char *argv[]);

#define BUF_SIZE 4096;
#define OUTPUT_MODE 0700;

int main(){
    int in_fd, out_fd, rd_count, wt_count;

    char buffer[BUF_SIZE];

    if(argc != 3){
        exit(1);
    }

    


}