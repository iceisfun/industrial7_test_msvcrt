/* TC
 * id:    scn_neg_snscanf_s_null_dest
 * area:  scanf
 * kind:  negative
 * funcs:
 * procs: _snscanf_s _snwscanf_s _snscanf_s_l
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _snscanf_s, _snwscanf_s: NULL pointers invoke the invalid parameter handler; return EOF with errno EINVAL
 * note:  Each NULL call runs in a child process (with a returning invalid-parameter handler installed) so a crash is a clean per-case failure.
 *        Exact return for a NULL destination is not asserted (only != 1); NULL input/format return EOF with EINVAL.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

typedef int (__cdecl *sn_t)(const char *, size_t, const char *, ...);
typedef int (__cdecl *snw_t)(const wchar_t *, size_t, const wchar_t *, ...);
typedef int (__cdecl *snl_t)(const char *, size_t, const char *, void *, ...);
#if TC_CRT_VER >= 140
#define SN _snscanf_s
#define SNW _snwscanf_s
#define SNL _snscanf_s_l
#define HAVE_P(x) 1
#else
static sn_t p_sn; static snw_t p_snw; static snl_t p_snl;
#define SN p_sn
#define SNW p_snw
#define SNL p_snl
#define HAVE_P(x) ((x) != NULL)
#endif
static const char *volatile vn; static const wchar_t *volatile vw; static char *volatile nd; static wchar_t *volatile ndw;
static const char *volatile nullstr; static const wchar_t *volatile nullwstr;

static int child_main(const char *c)
{
    int n, x = 5;
    if (!HAVE_P(SN) || !HAVE_P(SNW)) return 90;
    if (!tc_iph_install()) return 91;
    errno = 0;
    if (tc_strequ(c, "s_null")) { vn = "abc"; n = SN(vn, 3, "%s", nd, (unsigned)4); return (n != 1 && tc_iph_count >= 1) ? 0 : 10; }
    if (tc_strequ(c, "w_null")) { vw = L"abc"; n = SNW(vw, 3, L"%s", ndw, (unsigned)4); return (n != 1 && tc_iph_count >= 1) ? 0 : 11; }
    if (tc_strequ(c, "in_null")) {
        n = SN(nullstr, 3, "%d", &x);
        if (n != EOF || errno != EINVAL || tc_iph_count < 1 || x != 5) return 12;
        tc_iph_count = 0; errno = 0;
        n = SNW(nullwstr, 3, L"%d", &x);
        return (n == EOF && errno == EINVAL && tc_iph_count >= 1 && x == 5) ? 0 : 13;
    }
    if (tc_strequ(c, "fmt_null")) {
        vn = "12"; n = SN(vn, 2, nullstr, &x);
        if (n != EOF || errno != EINVAL || tc_iph_count < 1 || x != 5) return 14;
        tc_iph_count = 0; errno = 0; vw = L"12";
        n = SNW(vw, 2, nullwstr, &x);
        return (n == EOF && errno == EINVAL && tc_iph_count >= 1 && x == 5) ? 0 : 15;
    }
    if (tc_strequ(c, "l_null")) {
        if (!HAVE_P(SNL)) return 92;
        vn = "abc"; n = SNL(vn, 3, "%s", NULL, nd, (unsigned)4);
        return (n != 1 && tc_iph_count >= 1) ? 0 : 16;
    }
    return 99;
}
static void run_child(const char *name)
{
    DWORD code = 1234; int ok = tc_spawn_self(name, &code, 20000);
    CHECK(ok); CHECK_EQ_U(code, 0);
}
TC_CASE(s_null_dest_iph) { run_child("s_null"); }
TC_CASE(wide_s_null_dest_iph) { run_child("w_null"); }
TC_CASE(null_input_eof_einval) { run_child("in_null"); }
TC_CASE(null_format_eof_einval) { run_child("fmt_null"); }
TC_CASE(l_variant_null_dest) { run_child("l_null"); }
TC_CASE(valid_control)
{
    char a[8]; wchar_t w[8]; int n;
    CHECK(HAVE_P(SN) && HAVE_P(SNW)); if (!HAVE_P(SN) || !HAVE_P(SNW)) return;
    CHECK(tc_iph_install()); tc_iph_count = 0; vn = "abc"; vw = L"abc";
    n = SN(vn, 3, "%s", a, (unsigned)8); CHECK_EQ_I(n, 1); CHECK_STR(a, "abc");
    n = SNW(vw, 3, L"%s", w, (unsigned)8); CHECK_EQ_I(n, 1); CHECK_WSTR(w, L"abc");
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    const char *c; tc_begin();
#if TC_CRT_VER < 140
    p_sn = (sn_t)tc_crt_proc("_snscanf_s"); p_snw = (snw_t)tc_crt_proc("_snwscanf_s"); p_snl = (snl_t)tc_crt_proc("_snscanf_s_l");
#endif
    if ((c = tc_child_name())) return child_main(c);
    TC_RUN(s_null_dest_iph); TC_RUN(wide_s_null_dest_iph); TC_RUN(null_input_eof_einval);
    TC_RUN(null_format_eof_einval); TC_RUN(valid_control); TC_RUN(l_variant_null_dest);
    return tc_end();
}
