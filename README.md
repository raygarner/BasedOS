BasedOS is an operating system programmed by Ray Garner.
It runs natively on x86 hardware.
The source can be compiled using the included bash script, however you must have a valid cross-compiler installed for this to work because the executable which is generated will be booted in 32-bit with no libraries from your current OS.
Thoughout development I used a VM client called QEMU. This can be used to boot a .bin file which is created after the compilation script is ran. The 'boot.sh' bash script uses QEMU by default.

Currently, I am trying to create an interpreter similar to Bash, BasedBash. It will not read from files but from the command line. It's features will include string and integer variables, while loops, and arithmetic and logic.

In the future, I may implement a file system and a text editor so that files can be written and then interpreted later.

