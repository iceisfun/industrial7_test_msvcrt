/* TC
 * id:    nfx_exit_onexit_32_handlers
 * area:  negative_fatal
 * kind:  positive
 * funcs:  exit _exit
 * procs: _crt_atexit _onexit
 * crt:   all
 * tier:  p0
 * conf:  observed
 * ref:   MS docs _onexit; cf. C11 7.22.4.2 (atexit: at least 32 functions)
 * note:  D: ucrtbase.dll does not export _onexit (GetProcAddress == NULL on Win10 22H2 19045; the old call through a NULL
 *        pointer crashed with 0xc0000005). registration goes through tc_reg_atexit() (tc.h): the real export _crt_atexit where present, else _onexit (legacy CRTs). Observed on ucrtbase 19045: 32 handlers register and all run in
 *        reverse order at exit(). Stable. Legacy CRTs keep the original _onexit coverage [unverified on the reference].
 */
#include "tc.h"
#include <stdlib.h>
#include <limits.h>



static volatile unsigned mask;
static volatile int count;
static void __cdecl hh0(void) { mask |= 1u << 0; count++; _exit((count == 32 && mask == 0xFFFFFFFFu) ? 32 : 100 + count); }
static void __cdecl hh1(void) { mask |= 1u << 1; count++; }
static void __cdecl hh2(void) { mask |= 1u << 2; count++; }
static void __cdecl hh3(void) { mask |= 1u << 3; count++; }
static void __cdecl hh4(void) { mask |= 1u << 4; count++; }
static void __cdecl hh5(void) { mask |= 1u << 5; count++; }
static void __cdecl hh6(void) { mask |= 1u << 6; count++; }
static void __cdecl hh7(void) { mask |= 1u << 7; count++; }
static void __cdecl hh8(void) { mask |= 1u << 8; count++; }
static void __cdecl hh9(void) { mask |= 1u << 9; count++; }
static void __cdecl hh10(void) { mask |= 1u << 10; count++; }
static void __cdecl hh11(void) { mask |= 1u << 11; count++; }
static void __cdecl hh12(void) { mask |= 1u << 12; count++; }
static void __cdecl hh13(void) { mask |= 1u << 13; count++; }
static void __cdecl hh14(void) { mask |= 1u << 14; count++; }
static void __cdecl hh15(void) { mask |= 1u << 15; count++; }
static void __cdecl hh16(void) { mask |= 1u << 16; count++; }
static void __cdecl hh17(void) { mask |= 1u << 17; count++; }
static void __cdecl hh18(void) { mask |= 1u << 18; count++; }
static void __cdecl hh19(void) { mask |= 1u << 19; count++; }
static void __cdecl hh20(void) { mask |= 1u << 20; count++; }
static void __cdecl hh21(void) { mask |= 1u << 21; count++; }
static void __cdecl hh22(void) { mask |= 1u << 22; count++; }
static void __cdecl hh23(void) { mask |= 1u << 23; count++; }
static void __cdecl hh24(void) { mask |= 1u << 24; count++; }
static void __cdecl hh25(void) { mask |= 1u << 25; count++; }
static void __cdecl hh26(void) { mask |= 1u << 26; count++; }
static void __cdecl hh27(void) { mask |= 1u << 27; count++; }
static void __cdecl hh28(void) { mask |= 1u << 28; count++; }
static void __cdecl hh29(void) { mask |= 1u << 29; count++; }
static void __cdecl hh30(void) { mask |= 1u << 30; count++; }
static void __cdecl hh31(void) { mask |= 1u << 31; count++; }
static void (__cdecl *tbl[32])(void) = { hh0,hh1,hh2,hh3,hh4,hh5,hh6,hh7,hh8,hh9,hh10,hh11,hh12,hh13,hh14,hh15,hh16,hh17,hh18,hh19,hh20,hh21,hh22,hh23,hh24,hh25,hh26,hh27,hh28,hh29,hh30,hh31 };

TC_CASE(all_registered_and_run)
{
    DWORD code = 0; int ran = tc_spawn_self("go", &code, 20000);
    CHECK(ran); CHECK_EQ_U(code, 32);
}
int main(void)
{
    int i;
    tc_begin();
    if (tc_child_name() != NULL) {
        for (i = 0; i < 32; i++) if (tc_reg_atexit(tbl[i])) _exit(61);   /* 0 = registered */
        exit(0);
    }
    TC_RUN(all_registered_and_run);
    return tc_end();
}
