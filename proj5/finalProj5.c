#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include<time.h>


#define BLOCK_SIZE 1024
#define OUTPUT_MODE 0700

struct minix_super_block {
    unsigned short s_ninodes;
    unsigned short s_nzones;
    unsigned short s_imap_blocks;
    unsigned short s_zmap_blocks;
    unsigned short s_firstdatazone;
    unsigned short s_log_zone_size;
    unsigned int s_max_size;
    unsigned short s_magic;
    unsigned short s_state;
    unsigned int s_zones;
};

struct minix_inode {
    unsigned short i_mode;
    unsigned short i_uid;
    unsigned int i_size;
    unsigned int i_time;
    unsigned char i_gid;
    unsigned char i_nlinks;
    unsigned short i_zone[9];
};

struct minix_dir_entry {
    unsigned short inode;
    char name[14];
};

int minimount(char *file) {
    int mini = open(file, O_RDONLY);
    if (mini < 0) {
        perror("Error opening file");
        exit(1);
    }
    printf("Disk image '%s' opened successfully. \n", file);
    return mini;
}

void miniumount(int fd) {
    close(fd);
}

void help() {
    printf("    Welcome to Minix Console!\n");
    printf("    minimount: to mount the disk image\n");
    printf("    miniumount: to unmount the disk image\n");
    printf("    traverse: to list files and directories in the root directory\n");
    printf("    traverse -l: to list detailed info regarding files and directories in the root directory\n");
    printf("    exit: to exit the program\n");
}

bool readFile(int fd, int offSetMult, void *ptr, size_t size) {

    if (lseek(fd, offSetMult * BLOCK_SIZE, SEEK_SET) == (off_t) -1) {
        perror("Error seeking file");
        close(fd);
        return 0;
    }

    ssize_t readBytes = read(fd, ptr, size);
    if (readBytes == -1) {
        perror("Error reading file");
        close(fd);
        return 0;
    } else if (readBytes == 0) {
        printf("End of file reached\n");
        return 1;
    } else {
        return 1;
    }
}

void showSuperFunc(struct minix_super_block *msb){
    printf("minix: showsuper\n");
    printf("");
    printf("Total Inodes:        %u\n", msb->s_ninodes);
    printf("Total Zones:         %u\n", msb->s_nzones);
    printf("Inode Bitmap Blocks: %u\n", msb->s_imap_blocks);
    printf("Zone Bitmap Blocks:  %u\n", msb->s_zmap_blocks);
    printf("First Data Zone:     %u\n", msb->s_firstdatazone);
    printf("Log Zone Size:       %u\n", msb->s_log_zone_size);
    printf("Maximum File Size:   %u\n", msb->s_max_size);
    printf("magic:               %u\n", msb->s_magic);
    printf("state:               %u\n", msb->s_state);
    printf("zones:               %u\n", msb->s_zones);
}

void printPerms(int modeInfo){
    char perms[11] = {
            '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '\0'
    };

    if (modeInfo & S_IFDIR) perms[0] = 'd';
    if (modeInfo & S_IRUSR) perms[1] = 'r';
    if (modeInfo & S_IWUSR) perms[2] = 'w';
    if (modeInfo & S_IXUSR) perms[3] = 'x';
    if (modeInfo & S_IRGRP) perms[4] = 'r';
    if (modeInfo & S_IWGRP) perms[5] = 'w';
    if (modeInfo & S_IXGRP) perms[6] = 'x';
    if (modeInfo & S_IROTH) perms[7] = 'r';
    if (modeInfo & S_IWOTH) perms[8] = 'w';
    if (modeInfo & S_IXOTH) perms[9] = 'x';

    printf("   %s", perms);
}

void printTime(time_t time){
    struct tm *pt = localtime(&time);
    char timeArr[255];
    strftime(timeArr, sizeof(timeArr), "%b %d %Y", pt);
    printf("%s ", timeArr);
}
void showMinixDirEntryl(int fd, struct minix_inode *mi){
    for(int i = 0; i < 9; i++) {
        if(mi->i_zone[i] !=0){
            char block[BLOCK_SIZE * 8];
            int byteSize = 1 * BLOCK_SIZE;
            lseek(fd, mi->i_zone[i] * BLOCK_SIZE, SEEK_SET);
            read(fd, block, byteSize);
            int inode_start = 5 * BLOCK_SIZE;
            for (int s = 0; s < (byteSize / 16) - 1; s++) {
                struct minix_dir_entry *mde = (struct minix_dir_entry *) (block + s * 16);
                if (mde->inode != 0) {
                    if(!(strcmp(mde->name, ".")==0 || strcmp(mde->name, "..")==0)) {

                        struct minix_inode *mi1;

                        lseek(fd, inode_start + (mde->inode-1) * 32, SEEK_SET);
                        read(fd, mi1, sizeof(struct minix_inode));
                        printPerms(mi1->i_mode);
                        printf(" %d ", mi1->i_uid);
                        printf(" %d ", mi1->i_size);
                        printTime(mi1->i_time);
                        printf(" ");
                        printf("   %s", mde->name);
                        printf("\n");
                    }
                }
            }
        }
    }
}


void showMinixDirEntry(int fd, struct minix_inode *mi){
    for(int i = 0; i < 9; i++) {
        if(mi->i_zone[i] !=0){
            char block[BLOCK_SIZE * 8];
            int byteSize = 1 * BLOCK_SIZE;
            lseek(fd, mi->i_zone[i] * BLOCK_SIZE, SEEK_SET);
            read(fd, block, byteSize);
            for (int j = 0; j < (byteSize / 16) - 1; j++) {
                struct minix_dir_entry *mde = (struct minix_dir_entry *) (block + j * 16);
                if (mde->inode != 0) {
                    if(!(strcmp(mde->name, ".")==0 || strcmp(mde->name, "..")==0)){
                        printf("   %s\n", mde->name);
                    }
                }
            }
        }
    }
}

void showZone(int offset, int fd) {
    char block[BLOCK_SIZE * 8];
    int byteSize = 1 * BLOCK_SIZE;
    lseek(fd, offset * BLOCK_SIZE, SEEK_SET);
    read(fd, block, byteSize);

    printf("         0    1    2    3    4    5    6    7    8    9    a    b    c    d    e    f\n");

    for (int j = 0; j < (byteSize / 16); j++) {
        printf("%5x              ", j * 16);
        struct minix_dir_entry *mde = (struct minix_dir_entry *)(block + j * 16);
        if (mde->inode != 0) {
            if (!(strcmp(mde->name, ".") == 0 || strcmp(mde->name, "..") == 0)) {

                for (int i = 0; i < sizeof(mde->name); i++) {
                    if (mde->name[i] != '\0') {
                        printf("%c ", mde->name[i]);
                    } else {
                        break;
                    }
                }
            }
        }
        printf("\n");
    }
}




int main(int argc, char *argv[]) {
    int fd = -1;

    while (1) {
        char input[256] = {0};
        char *command, *arg;

        printf("minix: ");

        if (fgets(input, sizeof(input), stdin) == NULL) {
            perror("Error reading input");
            exit(1);
        }

        input[strcspn(input, "\n")] = 0;

        command = strtok(input, " ");
        arg = strtok(NULL, " ");

        if (command != NULL && strcmp(command, "quit") == 0) {
            printf("Exiting program.\n");
            return 0;
        }

        if (command != NULL && strcmp(command, "minimount") == 0) {
            if(arg != NULL){
                fd = minimount(arg);
            }
            else{
                printf("please pass file name\n");
            }
        }


        if(command != NULL && strcmp(command, "showzone") == 0){
            if(fd == -1){
                printf("no disk mounted\n");

            }
            if(arg != NULL){
                int test = atoi(arg);
                if(test != 0 || strcmp(arg, "0") == 0){
                    showZone(test, fd);
                }
            }
            else{
                printf("No Zone number provided \n");
            }

        }

        if (command != NULL && strcmp(command, "miniumount") == 0) {
            if (fd != -1) {
                miniumount(fd);
                printf("Unmounting and exiting minimount program\n");
                return 0;
            } else {
                printf("No disk image mounted.\n");
            }
        }




        if (command != NULL && strcmp(command, "showsuper") == 0) {
            //reading at block 1
            int offset = 1;
            if(fd == -1){
                printf("no disk mounted\n");
            }
            struct minix_super_block msb;
            printf("what\n");
            readFile(fd, offset, &msb, sizeof(struct minix_super_block));
            printf("what\n");
            showSuperFunc(&msb);
        }




        if (command != NULL && strcmp(command, "traverse") == 0 && argv[1] != NULL) {
            printf("test 1");
            int offset = 5;
            if(fd == -1){
                printf("no disk mounted");
            }
            if (argv[1] != NULL && strcmp(argv[1], "-l") == 0) {
                printf("test 3");
                struct minix_inode mi;
                readFile(fd, offset, &mi, sizeof(struct minix_inode));
                printf("We are in traverse argv");
                showMinixDirEntryl(fd, &mi);
            }
        }

        if (command != NULL && strcmp(command, "traverse") == 0) {

            int offset = 5;
            if(fd == -1){
                printf("no disk mounted\n");
            }
            struct minix_inode mi;
            readFile(fd, offset, &mi, sizeof(struct minix_inode));

            if(arg != NULL){
                showMinixDirEntryl(fd, &mi);
            }
            else {
                showMinixDirEntry(fd, &mi);
            }
        }

        if (command != NULL && strcmp(command, "help") == 0) {
            help();
        }

        if (command != NULL && strcmp(command, "minimount") != 0 && strcmp(command, "showzone") != 0 && strcmp(command, "miniumount") != 0 && strcmp(command, "traverse -l") != 0 && strcmp(command, "quit") != 0 && strcmp(command, "traverse") != 0 && strcmp(command, "showsuper") != 0 && strcmp(command, "help") != 0) {
            printf("Invalid command.\n");
        }
    }

}
