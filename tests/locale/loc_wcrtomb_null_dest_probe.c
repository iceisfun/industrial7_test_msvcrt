/* TC
 * id:    loc_wcrtomb_null_dest_probe
 * area:  locale
 * funcs: 
 * procs: wcrtomb
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   C11 7.29.6.3.3 p2
 * note:  PROBE: wcrtomb(NULL, L'A', &st) in the C locale behaves like wcrtomb(buf, L'\0', &st) and returns 1.
 *        ISO C says so; whether every Microsoft CRT generation honours the NULL destination (instead of
 *        failing the internal wcrtomb_s size validation) is the probed point.
 */
#include "tc.h"
#include <wchar.h>
#include <locale.h>
#include <string.h>

TC_DECL_PROC(size_t, wcrtomb, (char *, wchar_t, void *));
static struct { unsigned long long a, b; } st;
static volatile wchar_t v_wc;

TC_CASE(null_dest_returns_1)
{
    size_t r;
    if (!tc_p_wcrtomb) TC_SKIP("not exported");
    memset(&st, 0, sizeof st);
    v_wc = L'A'; r = tc_p_wcrtomb(NULL, v_wc, &st);
    CHECK_EQ_U(r, 1);
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(wcrtomb);
    TC_RUN(null_dest_returns_1);
    return tc_end();
}
