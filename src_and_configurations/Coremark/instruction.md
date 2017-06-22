# COREMARK AND INSTRUCTIONS TO RUN THIS BENCHMARCK IN A LINUX KERNEL RUNNING ON QEMU
CoreMark is a synthetic benchmark that measures the performance of central processing units (CPU) used in embedded systems.
It is intended to become an industry standard, replacing the older Dhrystone benchmark. The code is written in C and contains implementations of the following algorithms: list processing
(find and sort), matrix manipulation (common matrix operations), state machine (determine if an input stream contains valid numbers), and CRC (cyclic redundancy check).
IT was designed to avoid the issues that have been spotted with Dhrystone such as:

- Compiler optimizations

   Dhrystone is susceptible to compilers being able to optimize work away. When this
happens, Dhrystone becomes an unreliable processor benchmark.
With CoreMark, every operation in the benchmark derives values that are not available at
compile time. This ensures that while compilers can still make optimizations, they cannot
pre-compute results to optimize the work away completely.  


- Library calls

  Dhrystone contains library calls within the timed portion of the benchmark, which can
account for a significant portion of the benchmark time. This makes it difficult to compare
results where different libraries have been used.
CoreMark is designed so that it does not make any library calls during the timed portion
of the benchmark.  

- Version control

 Dhrystone has no official source, so several different versions are in use. If the Dhrystone
version is undisclosed, it is difficult to compare benchmark results.
CoreMark is available from the CoreMark web site http://www.coremark.org/
## DOWNLOAD COREMARK
   Coremark can be downloaded from the official site http://www.eembc.org/coremark/download.php filling the registration form or from other git repository (e.g. git clone https://github.com/tonyho/CoreMark.git)

## MODIFY THE FILE core_portme.h AND core_portme.mak IN THE RIGHT SUBDIRECTORY
   Because we have Linux in qemu, we have to modify the Linux subdirectory.

  - In particular in the core_portme.h we have to the name of the compiler we want use:
```
    #define COMPILER_VERSION "Please put compiler version here (e.g. gcc 4.1)"
```
   In our case Linaro toolchain:
```
    #define COMPILER_VERSION "/home/francesco/Scrivania/aarch64-unknown-linux-gnueabi/aarch64-unknown-linux-gnueabi/bin/aarch64-linux-gnu-gcc"
```
   Another important thing is that  ee_ptr_int needs to be the data type used to hold pointers, otherwise coremark may fail.
   In our case, Linaro support uint64_t as data type to hold pointers:
```
   typedef uint64_t ee_ptr_int;
```
  - In core_portme.mak we have first at all specify the name of the compiler as done for the core_portme.h
    Use this flag to define compiler to use
```
    CC = gcc
```  
    In our case:
```
    CC=/home/francesco/Scrivania/gcc-linaro-6.3.1-2017.02-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-gcc
```
    The second thing to is to statically link the library belong to libc
```
    LFLAGS_END += -static -L/home/francesco/Scrivania/gcc-linaro-6.3.1-2017.02-x86_64_aarch64-linux-gnu/aarch64-linux-gnu/libc/lib
```
## COMPILE COREMARK

    In your pc in a terminal go to the directory containing Coremark type
    ```
    make  compile PORT_DIR=linux
    make link PORT_DIR=linux
    ```
   the file coremark.exe will be created

 ## RUN COREMARK

   To run coremark you can simply type in the terminal
  ```
    make PORT_DIR=linux
  ```
    or type separately
  ```
    ./coremark.exe  0x0 0x0 0x66 0 7 1 2000 > ./run1.log

    ./coremark.exe  0x3415 0x3415 0x66 0 7 1 2000  > ./run2.log

    ./coremark.exe  8 8 8 0 7 1 1200 > ./run3.log
  ```
    For more info about results and iteration please read the readme.txt

  In our case to import the coremark in the qemu enviroment we have to follow these steps

   - start qemu

   - copy the coremark.exe in the tftpboot

   - send the coremark.exe to qemu : in the qemu console type
    ```
     tftp -gr coremark.exe 192.168.0.1
    ```
   - make it executable : chmod a+rwx coremark.exe

   - run the benchmark:
    ```
     ./coremark.exe  0x0 0x0 0x66 0 7 1 2000 > ./run1.log

     ./coremark.exe  0x3415 0x3415 0x66 0 7 1 2000  > ./run2.log

     ./coremark.exe  8 8 8 0 7 1 1200 > ./run3.log
    ```
