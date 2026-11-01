/* TC
 * id:    loc_neg_wcrtomb_s_erange_iph_probe
 * area:  locale
 * funcs: setlocale
 * procs: wcrtomb_s
 * crt:   >=90
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   MS docs wcrtomb_s
 * note:  Win10 triage (class A, probe -> observed): the invalid-parameter handler is NOT called on ucrtbase 19045 for this path (which returns EILSEQ, see
 *        loc_neg_wcrtomb_s_small_and_unmappable). Observed on Windows 10 22H2 build 19045.
 *        Original PROBE: when the buffer is too small (2-byte CP932 char, size 1) wcrtomb_s calls the invalid-parameter
 *        handler exactly once. The ERANGE return is documented (loc_neg_wcrtomb_s_small_and_unmappable); the handler
 *        invocation on this path is not clearly documented.
 *        [scope] observed on ucrtbase 19045 only; the legacy CRTs listed in crt: are [unverified] until the P3 runs.
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
    CHECK_EQ_I(tc_iph_count - c0, 0);   /* observed on Win10 22H2: no handler call on this path */
    setlocale(LC_ALL, "C");
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(wcrtomb_s);
    TC_RUN(iph_count);
    return tc_end();
}
