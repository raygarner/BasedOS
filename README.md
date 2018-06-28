BasedOS is an operating system programmed by Ray Garner.
It runs natively on x86 hardware.
The source can be compiled using the included bash script, however you must have a valid cross-compiler installed for this to work.
Thoughout development I used a VM client called QEMU. This can be used to boot a .bin file which is created after the compilation script is ran. The 'boot.sh' bash script uses QEMU by default.

Current plans for the project include:

-A bash like interpreter which can be used for script writing from the command line and for utility commands. This will include:
  -variables
  -while loops
  -logic and arithmetic
  
-It will convert BasedBash syntax to C
Future plans
-implementation of a text editor.
-implementation of a file system.
-the addition of the above two would mean that within BasedOS, you could write BasedBash files, and interpret them aswell as just doing so from a command line.



