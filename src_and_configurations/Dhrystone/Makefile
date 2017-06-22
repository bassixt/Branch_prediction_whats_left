# To use this makefile, you need to modify CC in order to use your cross-compiler
# The following one is intended only as example so, will not work without modifications.
CC=		/home/mc/project/gcc-linaro-4.9.4-2017.01-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-gcc		

GCC=		/home/mc/project/gcc-linaro-4.9.4-2017.01-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-gcc

PROGS=		unix			# Programs to build (UNIX)

TIME_FUNC=	-DTIME			# Use time(2) for measurement

HZ=		60			# Frequency of times(2) clock ticks


STRUCTASSIGN=	-DNOSTRUCTASSIGN	# Compiler cannot assign structs
STRUCTASSIGN=				# Compiler can assign structs

ENUMS=		-DNOENUMS		# Compiler doesn't have enum type
ENUMS=					# Compiler does have enum type

OPTIMIZE=	-Ox -G2			# Optimization Level (MSC, 80286)
OPTIMIZE=	-O4			# Optimization Level (generic UNIX)
GCCOPTIM=       -O

LFLAGS=		-static	#Loader Flags

CFLAGS=	$(OPTIMIZE) $(TIME_FUNC) -DHZ=$(HZ) $(ENUMS) $(STRUCTASSIGN) $(CFL)
GCCFLAGS= $(GCCOPTIM) $(TIME_FUNC) -DHZ=$(HZ) $(ENUMS) $(STRUCTASSIGN) $(CFL)

SRC=		dhry_1.c dhry_2.c
HDR=		dhry.h



all:	cc_dry2

cc_dry2:		$(SRC) $(HDR)
		$(CC) $(CFLAGS) $(SRC) $(LFLAGS)  -o $@


clean:
	-rm -f *.o *.obj


