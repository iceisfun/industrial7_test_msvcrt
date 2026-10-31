/* TC
 * id:    exp_secure_conv_exported
 * area:  exports
 * funcs:
 * procs: _itoa_s _ltoa_s _ultoa_s _i64toa_s _ui64toa_s _itow_s wcstombs_s mbstowcs_s
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _itoa_s, _itow_s; wcstombs_s; mbstowcs_s
 */
#include "tc.h"
static void need(const char *const *n)
{
    for (; *n; n++) if (!tc_crt_proc(*n)) { tc_failhdr(__FILE__, __LINE__); tc_write("missing export: "); tc_write(*n); tc_write("\n"); }
}
static void absent(const char *const *n)
{
    for (; *n; n++) if (tc_crt_proc(*n)) { tc_failhdr(__FILE__, __LINE__); tc_write("unexpected export: "); tc_write(*n); tc_write("\n"); }
}

TC_CASE(exported_all)
{
    static const char *const n[] = { "_itoa_s", "_ltoa_s", "_ultoa_s", "_i64toa_s", "_ui64toa_s", "_itow_s", "wcstombs_s", "mbstowcs_s", 0 };
    need(n);
}
TC_CASE(itoa_group)
{
    int (__cdecl *i)(int, char *, size_t, int) = (int (__cdecl *)(int, char *, size_t, int))tc_crt_proc("_itoa_s");
    int (__cdecl *l)(long, char *, size_t, int) = (int (__cdecl *)(long, char *, size_t, int))tc_crt_proc("_ltoa_s");
    int (__cdecl *u)(unsigned long, char *, size_t, int) = (int (__cdecl *)(unsigned long, char *, size_t, int))tc_crt_proc("_ultoa_s");
    int (__cdecl *i6)(long long, char *, size_t, int) = (int (__cdecl *)(long long, char *, size_t, int))tc_crt_proc("_i64toa_s");
    int (__cdecl *u6)(unsigned long long, char *, size_t, int) = (int (__cdecl *)(unsigned long long, char *, size_t, int))tc_crt_proc("_ui64toa_s");
    char b[80];
    if (!i || !l || !u || !i6 || !u6) TC_SKIP("not exported");
    CHECK_EQ_I(i(-123, b, sizeof b, 10), 0); CHECK_STR(b, "-123");
    CHECK_EQ_I(l(255, b, sizeof b, 16), 0); CHECK_STR(b, "ff");
    CHECK_EQ_I(u(4294967295UL, b, sizeof b, 10), 0); CHECK_STR(b, "4294967295");
    CHECK_EQ_I(i6(-5000000000LL, b, sizeof b, 10), 0); CHECK_STR(b, "-5000000000");
    CHECK_EQ_I(u6(255ULL, b, sizeof b, 2), 0); CHECK_STR(b, "11111111");
}
TC_CASE(itow)
{
    int (__cdecl *p)(int, wchar_t *, size_t, int) = (int (__cdecl *)(int, wchar_t *, size_t, int))tc_crt_proc("_itow_s");
    wchar_t b[32];
    if (!p) TC_SKIP("not exported");
    CHECK_EQ_I(p(-42, b, 32, 10), 0); CHECK_WSTR(b, L"-42");
}
TC_CASE(mbcs_conv_s)
{
    int (__cdecl *w2m)(size_t *, char *, size_t, const wchar_t *, size_t) = (int (__cdecl *)(size_t *, char *, size_t, const wchar_t *, size_t))tc_crt_proc("wcstombs_s");
    int (__cdecl *m2w)(size_t *, wchar_t *, size_t, const char *, size_t) = (int (__cdecl *)(size_t *, wchar_t *, size_t, const char *, size_t))tc_crt_proc("mbstowcs_s");
    char b[16]; wchar_t w[16]; size_t n = 99;
    if (!w2m || !m2w) TC_SKIP("not exported");
    CHECK_EQ_I(w2m(&n, b, sizeof b, L"abc", _TRUNCATE), 0); CHECK_EQ_U(n, 4); CHECK_STR(b, "abc");
    n = 99;
    CHECK_EQ_I(m2w(&n, w, 16, "xyz", _TRUNCATE), 0); CHECK_EQ_U(n, 4); CHECK_WSTR(w, L"xyz");
}
int main(void) { tc_begin(); TC_RUN(exported_all); TC_RUN(itoa_group); TC_RUN(itow); TC_RUN(mbcs_conv_s); return tc_end(); }
