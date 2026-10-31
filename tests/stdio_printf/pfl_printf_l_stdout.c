/* TC
 * id:    pfl_printf_l_stdout
 * area:  stdio_printf
 * funcs: _create_locale _free_locale
 * procs: _printf_l
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs printf, _printf_l (returns number of characters written)
 * note:  Output is a single newline per call so the harness stream stays readable. _printf_l is resolved at run time on
 *        the DLL CRTs and used as the header inline on ucrtbase.
 */
#include "tc.h"
#include <stdio.h>
#include <locale.h>

#if TC_CRT_VER < 140
typedef int (__cdecl *printf_l_t)(const char *, _locale_t, ...);
#endif

static int do_printf_l(_locale_t l, const char *fmt, int v)
{
#if TC_CRT_VER >= 140
    return _printf_l(fmt, l, v);
#else
    printf_l_t p = (printf_l_t)tc_crt_proc("_printf_l");
    if (!p) return -12345;
    return p(fmt, l, v);
#endif
}

TC_CASE(newline_count_one)
{
    _locale_t l = _create_locale(LC_ALL, "C");
    CHECK(l != NULL); if (!l) return;
    CHECK_EQ_I(do_printf_l(l, "\n", 0), 1);
    _free_locale(l);
}
TC_CASE(formatted_count)
{
    _locale_t l = _create_locale(LC_ALL, "C");
    CHECK(l != NULL); if (!l) return;
    CHECK_EQ_I(do_printf_l(l, "%d\n", 12345), 6);
    _free_locale(l);
}

int main(void)
{
    tc_begin();
    TC_RUN(newline_count_one); TC_RUN(formatted_count);
    return tc_end();
}
