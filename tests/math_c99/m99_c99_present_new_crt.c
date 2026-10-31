/* TC
 * id:    m99_c99_present_new_crt
 * area:  math_c99
 * funcs:
 * procs: cbrt trunc round lround fma fmax log2 exp2 expm1 log1p erf erfc tgamma lgamma nexttoward nan remquo scalbn asinh acosh atanh _dclass _dsign
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * conf:  documented
 * ref:   MS docs math functions (C99 functions added in VS2013 / CRT 12.0)
 * note:  Presence check via GetProcAddress; includes names mingw's ucrtbase import lib lacks.
 */
#include "tc.h"
static const char *const names[] = { "cbrt","trunc","round","lround","fma","fmax","log2","exp2","expm1","log1p","erf","erfc","tgamma","lgamma","nexttoward","nan","remquo","scalbn","asinh","acosh","atanh","_dclass","_dsign" };
TC_CASE(present)
{
    unsigned i;
    for (i = 0; i < sizeof names / sizeof names[0]; i++)
        if (!tc_crt_proc(names[i])) { tc_failhdr(__FILE__, __LINE__); tc_write("missing export: "); tc_write(names[i]); tc_write("\n"); }
}
int main(void) { tc_begin(); TC_RUN(present); return tc_end(); }
