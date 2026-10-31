/* TC
 * id:    m99_c99_neg_not_in_msvcrt_probe
 * area:  math_c99
 * funcs:
 * procs: cbrt trunc round fma log2 expm1 tgamma asinh remainder nexttoward lgamma erf nan _nextafter _copysign
 * crt:   msvcrt
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs "Universal C runtime routines by category" / math functions: C99 math added in VS2013 (CRT 12.0); Requirements tables
 * note:  PROBE: the system msvcrt.dll (Win7+; C99 math absent in the VS6-era CRT) does not export C99 math names. Split from the msvcr80-110 test because later OS builds of msvcrt.dll are not verified. Controls _nextafter/_copysign must resolve.
 */
#include "tc.h"
static const char *const names[] = { "cbrt","trunc","round","fma","log2","expm1","tgamma","asinh","remainder","nexttoward","lgamma","erf","nan" };
TC_CASE(absent)
{
    unsigned i;
    for (i = 0; i < sizeof names / sizeof names[0]; i++)
        if (tc_crt_proc(names[i])) { tc_failhdr(__FILE__, __LINE__); tc_write("unexpected export: "); tc_write(names[i]); tc_write("\n"); }
}
TC_CASE(controls) { CHECK(tc_crt_proc("_nextafter") != NULL); CHECK(tc_crt_proc("_copysign") != NULL); }
int main(void) { tc_begin(); TC_RUN(absent); TC_RUN(controls); return tc_end(); }
