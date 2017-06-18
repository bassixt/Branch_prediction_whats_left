# <a name="Phase_4_data_gathering"></a>Phase 4 data gathering 
In order to gather data different Benchmark applications were run over the emulated Busybox system.  
To run programs over the emulated system, a connection between the host OS and the emulated one is needed to exchange the executable files because, obviously, they are not already embedded in Busybox.  

## <a name="How_to_set_the_connection_up"></a>How to set the connection up
To exchange data with the emulated system we used a tftp connection.  
To create a tftp server the following packages are required:

```bash
sudo apt-get install xinetd tftpd tftp
sudo apt-get install isc-dhcp-server
sudo apt-get install bridge-utils
```
Now create this file under `/etc/xinetd.d/` called `tftp`, for instance type:

``` bash
sudo vim /etc/xinetd.d/tftp
```
cut the following lines and past them in the created file:

```
service tftp
{
protocol        = udp
port            = 69
socket_type     = dgram
wait            = yes
user            = nobody
server          = /usr/sbin/in.tftpd
server_args     = /tftpboot
disable         = no
}
```
Create the following folder and execute the following commands:

```bash
sudo mkdir /tftpboot
sudo chmod -R 777 /tftpboot
sudo chown -R nobody /tftpboot
```
Now you can restart the service with the following commands:

```bash
sudo /etc/init.d/xinetd stop
sudo /etc/init.d/xinetd start
```

Now to create a suitable network with the emulated system you can use this script: [other_files/tftp_script].  
N.B.
* this `tftp_script` file must be in the root directory in which there are all the other directories with QEMU, Busybox, Makefile, ...  
* change user value with yours and nic with the name of your Ethernet interface  
Run it every time you reboot your PC using the command:

```bash
sudo ./tftp_script
```

## <a name="About_Dhrystone"></a>About Dhrystone
Dhrystone is a synthetic computing benchmark program developed in 1984 by Reinhold P. Weicker intended to be representative of system (integer) programming. The Dhrystone grew to become representative of general processor (CPU) performance. With Dhrystone, Weicker gathered meta-data from a broad range of software, including programs written in FORTRAN, PL/1, SAL, ALGOL 68, and Pascal. He then characterized these programs in terms of various common constructs: procedure calls, pointer indirections, assignments, etc. From this he wrote the Dhrystone benchmark to correspond to a representative mix. It is written in C language. [Wikipedia](https://en.wikipedia.org/wiki/Dhrystone).

## <a name="How_to_run_Dhrystone"></a>How to run Dhrystone
Dhrystone is a public software which source files can be downloaded from internet and then compiled.  
Its source files are in [Dhrystone](/Dhrystone/) directory but there are also the Makefile to build them and the already built executable file `cc_dry2`.  
To Run Dhrystone these are the steps:
1. Copy the Dhrystone executable in your /tftpboot/ folder.
1. Run the `tftp_script` if not already done.
1. Run QEMU with:
	
	```bash
	$ make run
	or
	$ make exec
	```
1. Once Busybox is running interact with it typing those commands:
	
	```bash
	tftp -gr cc_dry2 192.168.0.1
	chmod a+rwx cc_dry2
	./cc_dry2
	```
1. You will be later asked to type the number of runs: 10000000 was the minimum for us.

At the end you will see the results.
