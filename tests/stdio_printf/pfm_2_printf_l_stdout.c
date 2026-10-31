/* TC
 * id:    pfm_2_printf_l_stdout
 * area:  stdio_printf
 * funcs:
 * procs: _printf_l
 * crt:   >=80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs printf, _printf_l (returns the number of characters printed)
 * note:  writes one diagnostic line to stdout starting with '#'. Isolated from other tests because _printf_l is an import-library gap on msvcr80.
 */
#include "tc.h"
#include <stdio.h>

#if TC_CRT_VER < 140
TC_DECL_PROC(int, _printf_l, (const char *, void *, ...));
#endif

TC_CASE(returns_count)
{
    int n;
#if TC_CRT_VER >= 140
    n = _printf_l("# pfm_2 printf_l %x %s\n", NULL, 255, "ok");
#else
    if (!tc_p__printf_l) TC_SKIP("_printf_l absent");
    n = tc_p__printf_l("# pfm_2 printf_l %x %s\n", NULL, 255, "ok");
#endif
    fflush(stdout);
    CHECK_EQ_I(n, 23);
}
int main(void) { tc_begin(); 
#if TC_CRT_VER < 140
    TC_LOAD_PROC(_printf_l); CHECK(tc_p__printf_l != NULL);
#endif
    TC_RUN(returns_count); return tc_end(); }
