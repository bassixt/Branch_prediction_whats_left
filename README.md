# THIS IS THE PLACE WHERE TO SHARE INFOS AND TO TAKE IN MIND PROGRESSES

## SEMESTER PROJECT BRANCH PREDICTION WHAT'S LEFT

### First week:

 * About TAGE and its successors:

    * http://www.irisa.fr/caps/people/seznec/JILP-COTTAGE.pdf
    * https://www.irisa.fr/caps/people/seznec/L-TAGE.pdf

 * A study about why TAGE works so well:
    * https://comparch.net/2013/06/30/why-tage-is-the-best/

 * All André Seznec's publications:
    * https://www.irisa.fr/caps/people/seznec/

 * Additional paper:
    * https://sites.ualberta.ca/~delliott/ece510/seminars/2006f/project/Analysis_Of_Branch_Predictors_Ming&Guang/Report/Analysis%20of%20Branch%20Predictors.pdf

### Second week:
  * Start using Qemu:
    * Download and install Qemu
    * Download, install and configure crosstool-NG for x86_64
    * Build and run minimal Linux / Busybox systems in Qemu
      * https://gitlab.eurecom.fr/snippets/23
  * Set all variables and procedures for future use

### Third week:
  * Start using Qemu for bare metal HW
  * Download latest version of Qemu from:
    * http://git.qemu-project.org/?p=qemu.git;a=summary
  * Download, configure and use Crosstool-NG for aarch64-unknown-linux-gnueabi
    * /packages/LabSoC/toolchains
  *Try to run an Hello world bare metal application:
    * Downloaded from:
      * https://github.com/freedomtan/aarch64-bare-metal-qemu
      * Makefile modification to use our toolchain
  * Try to run SPECint2006
  * Try to run Dhrystone
    * Downloaded from:
     * https://fossies.org/linux/privat/old/dhrystone-2.1.tar.gz/
     * Modification to Makefile
     * Find libraries for aarch64
        * Downloaded from: http://www.musl-libc.org/download.html
     * Ongoing : Modification to Makefile for printf...
      * Trial : simple hello world try to print 

### Fourth week:
  *Useful material
    *https://github.com/cloudius-systems/osv/wiki/AArch64
  *Bare-metal dhrystone
        *Makefile modification
        *Static linking of libc (tried newlib, musl)
        *Linker script 
    *Linaro toolchain, QEMU out-of-tree build, Busy-box, Linux kernel
        *Makefile for the entire "framework"
        *
