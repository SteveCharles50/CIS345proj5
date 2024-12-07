Steve Charles: 100% CSUID: 2850400

Project mounts disc image and successfully returns values for these 6 commands(will show output):
    -help: lists all of the commands you can enter
        -OUTPUT:
            minix: help
                Welcome to Minix Console!
                minimount: to mount the disk image
                miniumount: to unmount the disk image
                traverse: to list files and directories in the root directory
                traverse -l: to list detailed info regarding files and directories in the root directory
                exit: to exit the program

    -minimount: mounts hardcoded image provided(imagefile.img)
        -OUTPUT:
            -minix: minimount "imagefile.img" <- pass in the minix disk image you want to read
             Disk image 'imagefile.img' opened successfully.

    -miniumount: unmounts the mounted minix disk image
        -OUTPUT:
            -minix: miniumount
             Unmounting and exiting minimount program

    -showsuper: list information of the superblock
        -OUTPUT:
            -minix: showsuper
             what <- code breaks when I remove these print statments more in the P.S section
             what
             minix: showsuper
             Total Inodes:        3424
             Total Zones:         10240
             Inode Bitmap Blocks: 1
             Zone Bitmap Blocks:  2
             First Data Zone:     112
             Log Zone Size:       0
             Maximum File Size:   268966912
             magic:               5007
             state:               0
             zones:               0


    -traverse[-l]: lists the root directory. if switch -l is turned on will add details as well
        -OUTPUT:
            -minix: traverse -l
                -rw-rw-r-- 1000  670 Sep 20 2012     A.cla
                -rw-r--r-- 1000  763 Dec 14 2010     Aloop1.cla
                -rw-r--r-- 1000  333 Dec 14 2010     Aloop1.jav
                -rw-r--r-- 1000  786 Dec 14 2010     Aloop2.cla
                -rw-r--r-- 1000  404 Dec 14 2010     Aloop2.jav
                -rw-r--r-- 1000  786 Dec 14 2010     Aloop3.cla
                -rw-r--r-- 1000  408 Dec 14 2010     Aloop3.jav
                -rw-rw-r-- 1000  358 Sep 20 2012     B.cla
                -rw-r--r-- 1000  1067 Dec 14 2010     Condit.cla
                -rw-r--r-- 1000  444 Dec 14 2010     Condit.jav
                -rw-rw-r-- 1000  508 Sep 20 2012     D.cla
                -rw-r--r-- 1000  967 Dec 14 2010     Demo.cla
                -rw-r--r-- 1000  648 Dec 14 2010     Demo.jav
                drwx------ 1000  128 Oct 27 2014     dir1
                -rw-rw-r-- 1000  630 Sep 20 2012     PolyTest.cla
                -rw-rw-r-- 1000  1063 Sep 20 2012     PolyTest.jav
                -rw-r--r-- 1000  226 Dec 14 2010     some.cla
                -rw-r--r-- 1000  278 Dec 14 2010     some.java

    -showzone[zone number]: shows the ASCII content of the entered zone number
        -OUTPUT:
            -minix: showzone 112
                      0    1    2    3    4    5    6    7    8    9    a    b    c    d    e    f
                 0
                10
                20
                30
                40              A . c l a
                50
                60              A l o o p 1 . c l a
                70
                80              A l o o p 1 . j a v
                90
                a0              A l o o p 2 . c l a
                b0
                c0              A l o o p 2 . j a v
                d0
                e0              A l o o p 3 . c l a
                f0
               100              A l o o p 3 . j a v
               110
               120              B . c l a
               130
               140              C o n d i t . c l a
               150
               160              C o n d i t . j a v
               170
               180              D . c l a
               190
               1a0              D e m o . c l a
               1b0
               1c0              D e m o . j a v
               1d0
               1e0              d i r 1
               1f0
               200              P o l y T e s t . c l a
               210
               220              P o l y T e s t . j a v
               230
               240              s o m e . c l a
               250
               260              s o m e . j a v a
               270
               280
               290
               2a0
               2b0
               2c0
               2d0
               2e0
               2f0
               300
               310
               320
               330
               340
               350
               360
               370
               380
               390
               3a0
               3b0
               3c0
               3d0
               3e0
               3f0

    -quit: quits the program
        -OUTPUT:
            -minix: quit
             Exiting program.

    P.S: ISSUES:
       I have "what" being printed twice in this if statement on line 280 and 282
       struct minix_super_block msb;
                   printf("what\n");
                   readFile(fd, offset, &msb, sizeof(struct minix_super_block));
                   printf("what\n");
                   showSuperFunc(&msb);
       I had originally done this for debugging purposes and left it in there, now when I remove either whats I get a segmentation error in traverse -l which is called by completely different if statement.
       These print statements are essential if they aren't there traverse -l just doesn't work.


       -This exact same error also occurs when I remove the return 0; in miniumount on line 264, because of this miniumout unmounts and closes the program


