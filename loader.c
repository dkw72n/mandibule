// ======================================================================== //
// author:  ixty                                                       2018 //
// project: mandibule                                                       //
// licence: beerware                                                        //
// ======================================================================== //

// only c file of our code injector
// it includes directly all other code to be able to wrap all generated code
// between start/end boundaries

#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
#include <linux/unistd.h>

// #include "echo.h"
// #include "top.h"
#include "loader.h"

// include minimal inline c runtime + support code
#include "icrt.h"

#undef printf
#define printf(...)
#include "elfload.h"
#include "fakestack.h"
#include "ptinject.h"
//#include "shargs.h"

// forward declarations
unsigned long mandibule_end(void);
void _main(unsigned long * sp);
void payload_loadelf(void* sp);

// small macro for print + exit
#define error(...) do { printf(__VA_ARGS__); _exit(1); } while(0)
#define debug(fmt, ...) do { printf("[-] " fmt "\n", ##__VA_ARGS__);} while(0)

// define injector entry point - which calls main()
void _start(void)
{
    CALL_SP(_main);
}

// injector main code
void _main(unsigned long * sp)
{
    // argument parsing stuff
    int             ac          = *sp;
    char **         av          = (char **)(sp + 1);
    debug("ac = %d av = %lx", ac, av);
    payload_loadelf(sp);
    _exit(0);
}

static inline void* get_auxv_ptr(void* sp)
{
/*
--- argc
0x00007fffffffdc50│+0x0000: 0x0000000000000001	 ← $rsp
--- argv
0x00007fffffffdc58│+0x0008: 0x00007fffffffe020  →  "/bin/echo"
0x00007fffffffdc60│+0x0010: 0x0000000000000000
--- env
0x00007fffffffdc68│+0x0018: 0x00007fffffffe02a  →  "CLUTTER_IM_MODULE=xim"
0x00007fffffffdc70│+0x0020: 0x00007fffffffe040  →  "LC_ALL=en_US.UTF-8"
0x00007fffffffdc78│+0x0028: 0x00007fffffffe053  →  "LS_COLORS=rs=0:di=01;34:ln=01;36:mh=00:pi=40;33:so[...]"
0x00007fffffffdc80│+0x0030: 0x00007fffffffe63f  →  "LD_LIBRARY_PATH=/usr/local/lib:/home/ljanjun/.loca[...]"
0x00007fffffffdc88│+0x0038: 0x00007fffffffe678  →  "LC_MEASUREMENT=zh_CN.UTF-8"
0x00007fffffffdc90│+0x0040: 0x0000000000000000  
--- auxv
...
*/
    long * auxv_ptr = sp;
    long c;
    c = *(auxv_ptr++); // argc
    while(c-- > 0) auxv_ptr++; // argv
    auxv_ptr++; // 0
    while(*(auxv_ptr++)); // env
    return auxv_ptr;
}

void payload_loadelf(void* sp)
{
    unsigned long * auxv_ptr;
    unsigned long   eop;
    unsigned long   base_addr;

    auxv_ptr = get_auxv_ptr(sp); 
    // load the elf into memory!
    if(map_elf_from_buf(_payload, sizeof(_payload), 0, auxv_ptr, &eop) < 0)
         error("> failed to load elf\n");
    
    // all done
    debug("starting (sp: %lx, eop: %lx) ...", sp, eop);
    FIX_SP_JMP(sp, eop);

    // never reached if everything goes well
    debug("returned from loader");
    _exit(1);
}
