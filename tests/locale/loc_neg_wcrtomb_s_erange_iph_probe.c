/* TC
 * id:    loc_neg_wcrtomb_s_erange_iph_probe
 * area:  locale
 * funcs: setlocale
 * procs: wcrtomb_s
 * crt:   >=90
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs wcrtomb_s
 * note:  PROBE: when the buffer is too small (2-byte CP932 char, size 1) wcrtomb_s calls the invalid-parameter
 *        handler exactly once. The ERANGE return is documented (loc_neg_wcrtomb_s_small_and_unmappable); the handler
 *        invocation on this path is not clearly documented.
 */
#include "tc.h"
#include <wchar.h>
#include <locale.h>
#include <string.h>
#include <errno.h>

TC_DECL_PROC(errno_t, wcrtomb_s, (size_t *, char *, size_t, wchar_t, void *));
static struct { unsigned long long a, b; } st;
static volatile wchar_t v_wc;

TC_CASE(iph_count)
{
    char b[8]; size_t n = 99; int c0;
    if (!tc_p_wcrtomb_s || !tc_iph_install()) TC_SKIP("n/a");
    CHECK(setlocale(LC_ALL, ".932") != NULL); memset(&st, 0, sizeof st); c0 = tc_iph_count;
    v_wc = 0x3042; tc_p_wcrtomb_s(&n, b, 1, v_wc, &st);
    CHECK_EQ_I(tc_iph_count - c0, 1);
    setlocale(LC_ALL, "C");
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(wcrtomb_s);
    TC_RUN(iph_count);
    return tc_end();
}
