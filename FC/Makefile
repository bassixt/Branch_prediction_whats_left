###############################################################################
# Basically, try go execute commands top-down, I have put comments to look on
# marked by XXX, in vim its highligted. Execute the commands from project root
# directory where you also put this script, all components (kernel, initramfs,
# qemu etc.) will be child directories
#
#XXX:
# xcc - this is patch to your cross-tollchain binaries. The best is to download
# pre-build toolchain from linaro (for ubuntu/debian it works out-of-the box)
# and unpack it to the path of your choice, eg:
# /opt/gcc-linaro-4.9-2015.05-x86_64_aarch64-linux-gnu/
#
# Link to newest:
# https://releases.linaro.org/components/toolchain/binaries/4.9-2017.01/aarch64-linux-gnu/gcc-linaro-4.9.4-2017.01-x86_64_aarch64-linux-gnu.tar.xz
#
# To unpack (example location):
# $ cd /opt
# $ tar -xJf gcc-linaro-4.9.4-2017.01-x86_64_aarch64-linux-gnu.tar.xz
#
# Now find the bin/ directory inside and setup the xcc path accordingly,
# together with the toolchain prefix, eg:
# xcc	= /opt/gcc-linaro-4.9-2015.05-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-
#
#
# Link to olders (sometimes there are weird version-specific issues, depending
# what you are working on):
# https://releases.linaro.org/archive/15.05/components/toolchain/gcc-linaro/4.9/
# 
# You can find many different versions in their archive tree.
#
#
# If you need to build from sources, they also provide them, I think here:
# https://releases.linaro.org/components/toolchain/binaries/4.9-2017.01/aarch64-linux-gnu/gcc-linaro-4.9-2017.01.tar.xz
#
# But I have not done this recently and most likely you don't need that
###############################################################################


###############################################################################
# QEMU out-of-tree build
###############################################################################
qemu_root = $(PWD)/qemu
qemu_src = $(qemu_root)/src
qemu_build = $(qemu_root)/build
qemu_dtc =  $(qemu_src)/dtc

# XXX: latest is stable-2.7 AFAIK, you can try
qemu_ver = stable-2.7

qclone:
	mkdir -p $(qemu_build)
	test -d $(qemu_src) || git clone git://git.qemu-project.org/qemu.git $(qemu_src)
	cd $(qemu_src) && git checkout -b $(qemu_ver) origin/$(qemu_ver)
	cd $(qemu_src) && ( test -d $(qemu_dtc) || git submodule update --init dtc)


# XXX: Several things disabled, probably you don't need them if the names tells
# nothing to you. Enabling may require another libraries installed. Even this
# configuration may require some packages I don't remember now but nothing
# exotic perhaps. Maybe you need to enable some if you'd need to have
# graphic console??

qconfigure:
	test -d $(qemu_build) || mkdir -p $(qemu_build)
	cd $(qemu_build) && $(qemu_src)/configure  \
    --target-list="aarch64-softmmu"  \
    --enable-debug  \
    --enable-kvm \
	--disable-libusb --disable-sdl \
	--disable-seccomp --disable-numa \
	--disable-spice --disable-archipelago --disable-usb-redir \
	--disable-brlapi --disable-gtk --disable-bluez --disable-rbd \
	--disable-opengl --disable-libiscsi --disable-vde --disable-vnc-png \
	--disable-vnc-sasl --disable-werror
	cd $(qemu_root) && gtags .

qbuild:
	make -C $(qemu_build) -j3


###############################################################################
# Initramfs - I am using small busybox, if you need more rich userspace maybe
# try with buildroot but I haven't spent my time on it yet
###############################################################################
busybox_dir		=	$(PWD)/busybox
busybox_install	=	$(busybox_dir)/_install
rcs_path		=	$(busybox_install)/etc/init.d/rcS
ramfs_img		=	~/Desktop/linaro_busybox_aarch64/busybox.cpio.gz

define INIT_RCS
cat <<@ >$(rcs_path)
#! /bin/sh
mount -t proc proc /proc
mount -t sysfs sysfs /sys
mount -t devtmpfs devtmpfs /dev
@
chmod +x $(rcs_path)
endef
export INIT_RCS

static_flag = CONFIG_STATIC
xcc_flag 	= CONFIG_CROSS_COMPILER_PREFIX

# XXX: This is a crucial step described at the top. The setup below means that
# the toolset bin/ directory is in the PATH, otherwise (perhaps better to avoid
# mess of installed packages) setup manually, eg:
xcc			= /opt/gcc-linaro-6.3.1-2017.02-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-
#xcc		 	= aarch64-linux-gnu-
bbx_config  = .config
dbg_flag	= CONFIG_DEBUG

initrd:
	@rm -f $(ramfs_img)
	@test -d $(busybox_dir) || git clone git://git.busybox.net/busybox
	@cd $(busybox_dir) && make distclean && make defconfig
	@cd $(busybox_dir) && \
		awk '{ \
			gsub(/^.*$(static_flag).*/, "$(static_flag)=y") \
		} {print $0}' \
		$(bbx_config) > config.tmp && mv config.tmp $(bbx_config) 
	@cd $(busybox_dir) && sed -i -e 's@$(xcc_flag)=""@$(xcc_flag)="$(xcc)"@' $(bbx_config)
	@cd $(busybox_dir) && make -j3 install
	@cd $(busybox_install) && \
		mkdir dev etc etc/init.d proc sys && \
		ln -s /sbin/init init && \
		sh -c "$$INIT_RCS"
	@cd $(busybox_install) && find . | cpio -o --format=newc | gzip -c > $(ramfs_img)

###############################################################################
# Kernel
#
###############################################################################
kernel_dir		=	$(PWD)/linux
kernel_img		=	~/Desktop/linaro_busybox_aarch64/linux/arch/arm64/boot/Image

kconf:
	@cd $(kernel_dir) && make distclean
	@cd $(kernel_dir) && ARCH=arm64 CROSS_COMPILE=$(xcc) make defconfig

# CONFIG_DEFAULT_HUNG_TASK_TIMEOUT=
kmenuconf:
	@cd $(kernel_dir) && ARCH=arm64 CROSS_COMPILE=$(xcc) make menuconfig
	

kbuild:
	cd $(kernel_dir) && ARCH=arm64 CROSS_COMPILE=$(xcc) make -j2 Image

###############################################################################
# Run
#
# XXX: two tricks you can remove:
# - script /tmp/run saves the stdout output into /tmp/run - may be useful for
#   simple logs
# - stty intr... - remaps the host SIGINT trigger so that Ctrl-C inside QEMU
#   will kill your application but not QEMU itelf - this avoid annoying
#   restarts
#
# XXX:
# For NIC to work you will probably need dhcpd installed and configured. Try to
# google for it, I cannot find the configuration now. Otherwise you can simply
# remove this options.
###############################################################################

# mon_ip		= 172.17.0.2

#mon_ip		= 172.17.0.1
#mon_port	= 2222
#init_mem	= 1024M

run:
	script /tmp/run -c '\
		stty intr ^] && \
		~/Desktop/linaro_busybox_aarch64/qemu/build/aarch64-softmmu/qemu-system-aarch64 \
		-M virt \
		-m $(init_mem) \
		-nographic \
		-cpu cortex-a57 \
		-kernel $(kernel_img) \
		-initrd $(ramfs_img) \
		-netdev tap,id=net0,ifname=tap0,script=no,downscript=no \
		-device virtio-net-device,netdev=net0,mac="52:54:00:12:34:50" \
		-append "earlyprintk console=ttyAMA0 loglevel=8 ip=dhcp" \
		-serial stdio \
		-monitor telnet:$(mon_ip):$(mon_port),server,nowait,nodelay \
		'

# XXX: QEMU monitor - open from a separate terminal
monitor:
	telnet $(tel_ip) $(mon_port)

# XXX: QEMU help
help:
	$(qemu_build)/aarch64-softmmu/qemu-system-aarch64 \
		-M virt \
		-help

###############################################################################
# download host files to QEMU's busybox fia tftp (exposed by dnsmasq on my host)
#
# 	tftp -r hotplug3G.qemu -g 172.17.0.1 && chmod 755 hotplug3G.qemu
# 	tftp -r hptest3G -g 172.17.0.1 && chmod 755 hptest3G
###############################################################################

#ramfs_img       =   $(PWD)/busybox.cpio.gz
#kernel_img      =   $(PWD)/linux/arch/arm64/boot/Image
#qemu_root = $(PWD)/qemu
#qemu_src = $(qemu_root)/src
#qemu_build = $(qemu_root)/build
#qemu_dtc =  $(qemu_src)/dtc
mon_ip      = 192.168.0.25
tel_ip      = 192.168.0.1
mon_port    = 2222
init_mem    = 1024M

mcMake:
	script /tmp/run -c '\
		stty intr ^] && \
		$(qemu_build)/aarch64-softmmu/qemu-system-aarch64 \
		-M virt \
		-m $(init_mem) \
		-nographic \
		-cpu cortex-a57 \
		-kernel $(kernel_img) \
		-initrd $(ramfs_img) \
		-netdev tap,id=net0,ifname=tap0,script=no,downscript=no \
		-device virtio-net-device,netdev=net0,mac="52:54:00:12:34:50" \
		-append "earlyprintk console=ttyAMA0 loglevel=8 ip=$(mon_ip)" \
		-serial stdio \
		-monitor telnet:$(tel_ip):$(mon_port),server,nowait,nodelay \
		'
