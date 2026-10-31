/* TC
 * id:    wcs_neg_wcslwr_null_probe
 * area:  wstring
 * funcs:
 * procs: _wcslwr _wcsupr _wcslwr_l _wcsupr_l
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _strlwr, _wcslwr, _strupr, _wcsupr (parameter validation, VS2005+)
 * note:  PROBE: the non-_s case-conversion functions given a NULL string return NULL, set errno=EINVAL and invoke the
 *        invalid parameter handler once (msvcr80+ validation). Not stated explicitly in the docs. If the CRT instead
 *        dereferences NULL (access violation) that is the measured answer; each call runs in its own child process.
 */
#include "tc.h"
#include <errno.h>

TC_DECL_PROC(wchar_t *, _wcslwr, (wchar_t *));
TC_DECL_PROC(wchar_t *, _wcsupr, (wchar_t *));
TC_DECL_PROC(wchar_t *, _wcslwr_l, (wchar_t *, void *));
TC_DECL_PROC(wchar_t *, _wcsupr_l, (wchar_t *, void *));
static wchar_t *volatile pn = NULL;
static void *volatile ploc = NULL;

/* Each call runs in a child process: if the CRT does not validate NULL it will fault, and that must only fail its own case.
 * Child exit code: 0 = NULL return + EINVAL + one handler call; 1..7 = bit mask of wrong results; 70 = not exported. */
static int child_main(const char *n)
{
    wchar_t *r = (wchar_t *)1; int bits;
    TC_LOAD_PROC(_wcslwr); TC_LOAD_PROC(_wcsupr); TC_LOAD_PROC(_wcslwr_l); TC_LOAD_PROC(_wcsupr_l);
    if (!tc_iph_install()) return 70;
    errno = 0; tc_iph_count = 0;
    if (tc_strequ(n, "lwr")) { if (!tc_p__wcslwr) return 70; r = tc_p__wcslwr(pn); }
    else if (tc_strequ(n, "upr")) { if (!tc_p__wcsupr) return 70; r = tc_p__wcsupr(pn); }
    else if (tc_strequ(n, "lwr_l")) { if (!tc_p__wcslwr_l) return 70; r = tc_p__wcslwr_l(pn, ploc); }
    else if (tc_strequ(n, "upr_l")) { if (!tc_p__wcsupr_l) return 70; r = tc_p__wcsupr_l(pn, ploc); }
    bits = (r != NULL) | ((errno != EINVAL) << 1) | ((tc_iph_count != 1) << 2);
    return bits;
}
static void run_child(const char *n)
{
    DWORD code = 0; int ran = tc_spawn_self(n, &code, 20000);
    CHECK(ran);
    if (code == 70) TC_SKIP("not exported");
    CHECK_EQ_U(code, 0);   /* 0xC0000005 here = the CRT dereferenced NULL */
}
TC_CASE(lwr_null) { run_child("lwr"); }
TC_CASE(upr_null) { run_child("upr"); }
TC_CASE(lwr_l_null) { run_child("lwr_l"); }
TC_CASE(upr_l_null) { run_child("upr_l"); }
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) return child_main(c);
    TC_RUN(lwr_null); TC_RUN(upr_null); TC_RUN(lwr_l_null); TC_RUN(upr_l_null);
    return tc_end();
}
