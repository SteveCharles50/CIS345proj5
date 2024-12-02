#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>


#define BUF_SIZE 4096
#define OUTPUT_MODE 0700

int main(int argc, char *argv[]){
    int minimount, miniumount, rd_count, wt_count;

    char buffer[BUF_SIZE];

    if(argc != 3){
        exit(1);
    }

    //checks for argument in command line to open file
    minimount = open(argv[1], O_RDONLY);
    if (minimount < 0) exit(2);

    //overwrites if file already exists
    miniumount = creat(argv[2], OUTPUT_MODE);
    if (miniumount < 0) exit(3);

    printf("Disk image '%s' opened sucessfully. \n", );


    ssize_t bytes_read;
    off_t offset = 0;

    while(){
      
    }

    //reads data and writes to the new file
    while (true) {
        rd_count = read(minimount, buffer, BUF_SIZE);
        if (rd_count <= 0) break;
        wt_count = write(miniumount, buffer, rd_count);
        if (wt_count <= 0) exit(4);
    }

    //file closing mechanism
    while (true) {
        rd_count = read(minimount, buffer, BUF_SIZE);
        if (rd_count <= 0) break;
        wt_count = write(miniumount, buffer, rd_count);
        if (wt_count <= 0) exit(4);
    }

    //closing files
    close(minimount);
    close(miniumount);



    //exit code
    if (rd_count == 0)
        exit(0);
    else
        exit(5);


}


