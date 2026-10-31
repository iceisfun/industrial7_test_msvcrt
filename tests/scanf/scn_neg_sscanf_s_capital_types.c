/* TC
 * id:    scn_neg_sscanf_s_capital_types
 * area:  scanf
 * kind:  negative
 * funcs:
 * procs: sscanf_s swscanf_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs scanf_s: %S/%C read the opposite character width and need a size argument counted in destination characters; too small -> invalid parameter handler
 * note:  Return value and ERANGE are NOT asserted (see scn_neg_secure_too_small_ret_errno_probe).
 *        The NULL-destination call runs in a child process. On ucrtbase sscanf_s/swscanf_s are header inlines; elsewhere resolved at run time.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

typedef int (__cdecl *ss_t)(const char *, const char *, ...);
typedef int (__cdecl *sw_t)(const wchar_t *, const wchar_t *, ...);
#if TC_CRT_VER >= 140
#define SS sscanf_s
#define SW swscanf_s
#define HAVE_P(x) 1
#else
static ss_t p_ss; static sw_t p_sw;
#define SS p_ss
#define SW p_sw
#define HAVE_P(x) ((x) != NULL)
#endif
static const char *volatile vs; static const wchar_t *volatile vw; static wchar_t *volatile nw; static char *volatile nc;

static int child_main(const char *c)
{
    int n;
    if (!HAVE_P(SS) || !HAVE_P(SW) || !tc_iph_install()) return 90;
    if (tc_strequ(c, "S_null")) { vs = "abc"; n = SS(vs, "%S", nw, (unsigned)4); return (n != 1 && tc_iph_count >= 1) ? 0 : 10; }
    return 99;
}
TC_CASE(narrow_S_too_small)
{
    wchar_t a[4]; int n, i; for (i = 0; i < 4; i++) a[i] = L'Z';
    if (!HAVE_P(SS)) TC_SKIP("not exported");
    CHECK(tc_iph_install()); tc_iph_count = 0; errno = 0; vs = "abc";
    n = SS(vs, "%S", a, (unsigned)2);
    CHECK(tc_iph_count >= 1); (void)n;
    CHECK_EQ_U(a[2], L'Z'); CHECK_EQ_U(a[3], L'Z');
}
TC_CASE(narrow_C_size0)
{
    wchar_t a[2]; int n; a[0] = L'Z'; a[1] = L'Z';
    if (!HAVE_P(SS)) TC_SKIP("not exported");
    CHECK(tc_iph_install()); tc_iph_count = 0; vs = "a";
    n = SS(vs, "%C", a, (unsigned)0);
    CHECK(n != 1); CHECK(tc_iph_count >= 1);
}
TC_CASE(wide_S_too_small)
{
    char a[4]; int n, i; for (i = 0; i < 4; i++) a[i] = 'Z';
    if (!HAVE_P(SW)) TC_SKIP("not exported");
    CHECK(tc_iph_install()); tc_iph_count = 0; errno = 0; vw = L"abc";
    n = SW(vw, L"%S", a, (unsigned)2);
    CHECK(tc_iph_count >= 1); (void)n;
    CHECK_EQ_I(a[2], 'Z'); CHECK_EQ_I(a[3], 'Z');
}
TC_CASE(wide_C_size0)
{
    char a[2]; int n; a[0] = 'Z'; a[1] = 'Z';
    if (!HAVE_P(SW)) TC_SKIP("not exported");
    CHECK(tc_iph_install()); tc_iph_count = 0; vw = L"a";
    n = SW(vw, L"%C", a, (unsigned)0);
    CHECK(n != 1); CHECK(tc_iph_count >= 1);
}
TC_CASE(S_null_dest)
{
    DWORD code = 1234; int ok = tc_spawn_self("S_null", &code, 20000);
    CHECK(ok); CHECK_EQ_U(code, 0);
}
TC_CASE(S_exact_fit_control)
{
    wchar_t a[4]; int n, i; for (i = 0; i < 4; i++) a[i] = L'Z';
    if (!HAVE_P(SS)) TC_SKIP("not exported");
    CHECK(tc_iph_install()); tc_iph_count = 0; vs = "abc";
    n = SS(vs, "%S", a, (unsigned)4);
    CHECK_EQ_I(n, 1); CHECK_WSTR(a, L"abc"); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    const char *c; tc_begin();
#if TC_CRT_VER < 140
    p_ss = (ss_t)tc_crt_proc("sscanf_s"); p_sw = (sw_t)tc_crt_proc("swscanf_s");
#endif
    if ((c = tc_child_name())) return child_main(c);
    TC_RUN(narrow_S_too_small); TC_RUN(narrow_C_size0); TC_RUN(wide_S_too_small); TC_RUN(wide_C_size0);
    TC_RUN(S_null_dest); TC_RUN(S_exact_fit_control);
    return tc_end();
}
