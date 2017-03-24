ENTRY(_Reset)
SECTIONS {
        . = 0x40000000;
	.startup . : { startup64.o(.text) }
        .text : {
              * (vectors);
              * (.text);
        }
        .rodata : {
              * (.rodata);
        }
        flash_sdata = .;

        . = 0xA0000000;
        ram_sdata = .;
        .data : AT (flash_sdata) {
              * (.data);
        }
        ram_edata = .;
        data_size = ram_edata - ram_sdata;

        sbss = .;
        .bss : {
             * (.bss COMMON);
        }
	. = ALIGN(8);
	. = . + 0x1000; /* 4kB of stack memory */
        ebss = .;
        bss_size = ebss - sbss;
}

/*ENTRY(_Reset)
SECTIONS
{
	. = 0x40000000;
	.startup . : { startup64.o(.text) }
	.text : { *(.text) }
	.data : { *(.data) }
	. = 5368709120;
	.bss : { *(.bss) *(COMMON) }
	. = ALIGN(8);
*/	. = . + 0x10000; /* 4kB of stack memory */
/*	stack_top = .;
}*/
