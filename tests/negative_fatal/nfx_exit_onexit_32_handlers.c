/* TC
 * id:    nfx_exit_onexit_32_handlers
 * area:  negative_fatal
 * kind:  positive
 * funcs:  exit _exit
 * procs: _onexit
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _onexit; cf. C11 7.22.4.2 (atexit: at least 32 functions)
 * note:  32 distinct handlers; hh0 is registered first, runs last and reports.
 */
#include "tc.h"
#include <stdlib.h>
#include <limits.h>

TC_DECL_PROC(_onexit_t, _onexit, (_onexit_t));


static volatile unsigned mask;
static volatile int count;
static int __cdecl hh0(void) { mask |= 1u << 0; count++; _exit((count == 32 && mask == 0xFFFFFFFFu) ? 32 : 100 + count); return 0; }
static int __cdecl hh1(void) { mask |= 1u << 1; count++; return 0; }
static int __cdecl hh2(void) { mask |= 1u << 2; count++; return 0; }
static int __cdecl hh3(void) { mask |= 1u << 3; count++; return 0; }
static int __cdecl hh4(void) { mask |= 1u << 4; count++; return 0; }
static int __cdecl hh5(void) { mask |= 1u << 5; count++; return 0; }
static int __cdecl hh6(void) { mask |= 1u << 6; count++; return 0; }
static int __cdecl hh7(void) { mask |= 1u << 7; count++; return 0; }
static int __cdecl hh8(void) { mask |= 1u << 8; count++; return 0; }
static int __cdecl hh9(void) { mask |= 1u << 9; count++; return 0; }
static int __cdecl hh10(void) { mask |= 1u << 10; count++; return 0; }
static int __cdecl hh11(void) { mask |= 1u << 11; count++; return 0; }
static int __cdecl hh12(void) { mask |= 1u << 12; count++; return 0; }
static int __cdecl hh13(void) { mask |= 1u << 13; count++; return 0; }
static int __cdecl hh14(void) { mask |= 1u << 14; count++; return 0; }
static int __cdecl hh15(void) { mask |= 1u << 15; count++; return 0; }
static int __cdecl hh16(void) { mask |= 1u << 16; count++; return 0; }
static int __cdecl hh17(void) { mask |= 1u << 17; count++; return 0; }
static int __cdecl hh18(void) { mask |= 1u << 18; count++; return 0; }
static int __cdecl hh19(void) { mask |= 1u << 19; count++; return 0; }
static int __cdecl hh20(void) { mask |= 1u << 20; count++; return 0; }
static int __cdecl hh21(void) { mask |= 1u << 21; count++; return 0; }
static int __cdecl hh22(void) { mask |= 1u << 22; count++; return 0; }
static int __cdecl hh23(void) { mask |= 1u << 23; count++; return 0; }
static int __cdecl hh24(void) { mask |= 1u << 24; count++; return 0; }
static int __cdecl hh25(void) { mask |= 1u << 25; count++; return 0; }
static int __cdecl hh26(void) { mask |= 1u << 26; count++; return 0; }
static int __cdecl hh27(void) { mask |= 1u << 27; count++; return 0; }
static int __cdecl hh28(void) { mask |= 1u << 28; count++; return 0; }
static int __cdecl hh29(void) { mask |= 1u << 29; count++; return 0; }
static int __cdecl hh30(void) { mask |= 1u << 30; count++; return 0; }
static int __cdecl hh31(void) { mask |= 1u << 31; count++; return 0; }
static _onexit_t tbl[32] = { hh0,hh1,hh2,hh3,hh4,hh5,hh6,hh7,hh8,hh9,hh10,hh11,hh12,hh13,hh14,hh15,hh16,hh17,hh18,hh19,hh20,hh21,hh22,hh23,hh24,hh25,hh26,hh27,hh28,hh29,hh30,hh31 };

TC_CASE(all_registered_and_run)
{
    DWORD code = 0; int ran = tc_spawn_self("go", &code, 20000);
    CHECK(ran); CHECK_EQ_U(code, 32);
}
int main(void)
{
    int i;
    tc_begin();
    TC_LOAD_PROC(_onexit);
    if (tc_child_name() != NULL) {
        for (i = 0; i < 32; i++) if (!tc_p__onexit(tbl[i])) _exit(61);
        exit(0);
    }
    TC_RUN(all_registered_and_run);
    return tc_end();
}
