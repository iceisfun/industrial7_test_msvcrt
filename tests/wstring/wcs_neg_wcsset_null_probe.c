/* TC
 * id:    wcs_neg_wcsset_null_probe
 * area:  wstring
 * funcs:
 * procs: _wcsset _wcsnset
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _strset, _wcsset, _strnset, _wcsnset (parameter validation, VS2005+)
 * note:  PROBE: _wcsset(NULL,c) and _wcsnset(NULL,c,n>0) return NULL, errno=EINVAL, handler invoked once
 *        (each call in a child process in case the CRT faults instead). The n==0 case is in wcs_neg_wcsnset_null_n0_probe.
 */
#include "tc.h"
#include <errno.h>

TC_DECL_PROC(wchar_t *, _wcsset, (wchar_t *, wchar_t));
TC_DECL_PROC(wchar_t *, _wcsnset, (wchar_t *, wchar_t, size_t));
static wchar_t *volatile pn = NULL;

/* Each call runs in a child process (a CRT without NULL validation would fault). Exit code: 0 = NULL + EINVAL + 1 handler call;
 * 1..7 bit mask of wrong results; 70 = not exported. */
static int child_main(const char *n)
{
    wchar_t *r = (wchar_t *)1; int bits;
    TC_LOAD_PROC(_wcsset); TC_LOAD_PROC(_wcsnset);
    if (!tc_iph_install()) return 70;
    errno = 0; tc_iph_count = 0;
    if (tc_strequ(n, "set")) { if (!tc_p__wcsset) return 70; r = tc_p__wcsset(pn, L'x'); }
    else if (tc_strequ(n, "nset3")) { if (!tc_p__wcsnset) return 70; r = tc_p__wcsnset(pn, L'x', 3); }
    bits = (r != NULL) | ((errno != EINVAL) << 1) | ((tc_iph_count != 1) << 2);
    return bits;
}
static void run_child(const char *n)
{
    DWORD code = 0; int ran = tc_spawn_self(n, &code, 20000);
    CHECK(ran);
    if (code == 70) TC_SKIP("not exported");
    CHECK_EQ_U(code, 0);
}
TC_CASE(wcsset_null) { run_child("set"); }
TC_CASE(wcsnset_null_n3) { run_child("nset3"); }
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) return child_main(c);
    TC_RUN(wcsset_null); TC_RUN(wcsnset_null_n3);
    return tc_end();
}
