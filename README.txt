
Name - Raunak
I'd -  1217240245


################################################################################
############################### Procedure ###################################
##############################################################################

1. This zip file contains,
	(i)   Makefile
	(ii)  sys_test.c
	(iii) assign4.patch
	(iv)  README.txt

2. Extract the linux3.19-r0.tar.gz, linux3.19-r0 folder will be created.

3. Rename the kernel folder inside linux3.19-ro folder to kernel.old.

4. Copy the assign4.patch to the kernel folder inside linux3.19-r0 folder.

5. Apply the patch inside the kernel folder through the following command,
	 patch -p1 < assign4.patch

6. Include the cross-compilation tools in your PATH through the following command while staying in the kernel.old directory:
	export PATH=path_to_sdk/sysroots/x86_64-pokysdk-linux/usr/bin/i586-poky-linux:$PATH

7. Cross-compile the kernel through the following command, if you have more than 4 cores, you could choose the appropriate number instead of j4,
	ARCH=x86LOCALVERSION= CROSS_COMPILE=i586-poky-linux-make -j4  

8. While compiling, it will ask if you want to enable CONFIG_DYNAMIC_DUMP_STACK, enter y to enable the CONFIG_DYNAMIC_DUMP_STACK.

9. copy the bzImage from arch/x86/boot directory inside the kernel.old folder to the sd card of the galileo board, while removing the old bzImage.

10. Reboot the galileo board into your new kernel.

11. Now do, sudo chmod 777 /dev/ttyUSB0 to login to putty.

12. Now make the ip static by,
	ifconfig enp0s20f6 192.168.1.5 netmask 255.255.0.0 up

13. connect the lan cable.

14. Now, in this assignment folder, run make to compile the c file, stackdump_tester will be created.

15. Now, transfer the stackdump_tester to the galileo using scp command given below,
	sudo scp -r raunak@192.168.1.233:/home/raunak/eosi/assignment_4/stackdump_tester root@192.168.1.5:/home/test1

16. Run the test program, stackdump_tester to test the system calls.

##############################################################################
################################ OUTPUT #####################################
##############################################################################

Welcome to Dump Stack Testing
Enter the dumpstack mode
2
Select the symbol name where you want to insdump 1.sys_open 2.sys_close 3.sys_time 4.invalid_symbol
Enter 1,2,3,4 to select the symbol
1
Inserting Insdump
Dumpstack I'd: 0
Executing Rmdump
Continue?
Enter 1 to continue dumpstack testing else enter any other key to abort
3
Checking for rmdump, insertion of wrong dumpstack I'D
Inserting Insdump for Symbol3
Dumpstack I'd: 1
Executing Rmdump
Enter the dumpstack I'd for rmdump
1
Dumpstack I'd 1 is removed
Continue?
Enter 1 to continue entering another dumpstack I'd else enter any other key to abort
1
Enter the dumpstack I'd for rmdump
5
Failed, incorrect dumpstack I'd
Dumpstack I'd 5 is removed
Continue?
Enter 1 to continue entering another dumpstack I'd else enter any other key to abort
3
Checking for other mode
Enter the dumpstack mode
1
Dumpstack I'd: 2
Inside child
Failed rmdump
Testing for removal of insdump when program is Killed after insdump and insertion of multiple insdump
Enter the dumpstack mode
Testing for removal of insdump when program is Killed after insdump and insertion of multiple insdump
Enter the dumpstack mode
2
Dumpstack I'd: 3
Dumpstack I'd: 4


