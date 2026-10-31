/* TC
 * id:    exp_secure_stdio_exported
 * area:  exports
 * funcs:
 * procs: sprintf_s fprintf_s printf_s sscanf_s scanf_s fscanf_s vsprintf_s vfprintf_s vprintf_s _snprintf_s _vsnprintf_s swprintf_s fwprintf_s swscanf_s _snwprintf_s _vsnwprintf_s vswprintf_s
 * crt:   msvcr80 msvcr90 msvcr100 msvcr110 msvcr120
 * tier:  p1
 * conf:  documented
 * ref:   MS docs sprintf_s, sscanf_s, _snprintf_s, swprintf_s
 * note:  ucrtbase intentionally excluded: these are inline there (see exp_neg_probe_ucrt_stdio_inline).
 *        swprintf_s is the standard (non-_l) wide form; only valid arguments are used.
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
    static const char *const n[] = { "sprintf_s", "fprintf_s", "printf_s", "sscanf_s", "scanf_s", "fscanf_s", "vsprintf_s", "vfprintf_s",
        "vprintf_s", "_snprintf_s", "_vsnprintf_s", "swprintf_s", "fwprintf_s", "swscanf_s", "_snwprintf_s", "_vsnwprintf_s", "vswprintf_s", 0 };
    need(n);
}
TC_CASE(printf_s_family)
{
    int (__cdecl *p)(char *, size_t, const char *, ...) = (int (__cdecl *)(char *, size_t, const char *, ...))tc_crt_proc("sprintf_s");
    char b[32];
    if (!p) TC_SKIP("not exported");
    CHECK_EQ_I(p(b, sizeof b, "%d-%s", 42, "x"), 4); CHECK_STR(b, "42-x");
}
TC_CASE(scanf_s_family)
{
    int (__cdecl *p)(const char *, const char *, ...) = (int (__cdecl *)(const char *, const char *, ...))tc_crt_proc("sscanf_s");
    int a = 0; char s[8] = "";
    if (!p) TC_SKIP("not exported");
    CHECK_EQ_I(p("17 word", "%d %s", &a, s, (unsigned)sizeof s), 2); CHECK_EQ_I(a, 17); CHECK_STR(s, "word");
}
TC_CASE(snprintf_s_family)
{
    int (__cdecl *p)(char *, size_t, size_t, const char *, ...) = (int (__cdecl *)(char *, size_t, size_t, const char *, ...))tc_crt_proc("_snprintf_s");
    char b[8];
    if (!p) TC_SKIP("not exported");
    CHECK_EQ_I(p(b, sizeof b, 3, "%s", "abcdef"), -1); CHECK_STR(b, "abc");   /* truncated to count: documented -1 */
    CHECK_EQ_I(p(b, sizeof b, _TRUNCATE, "%d", 12), 2); CHECK_STR(b, "12");
}
TC_CASE(wide_s_family)
{
    int (__cdecl *p)(wchar_t *, size_t, const wchar_t *, ...) = (int (__cdecl *)(wchar_t *, size_t, const wchar_t *, ...))tc_crt_proc("swprintf_s");
    wchar_t b[16];
    if (!p) TC_SKIP("not exported");
    CHECK_EQ_I(p(b, 16, L"%d%s", 7, L"w"), 2); CHECK_WSTR(b, L"7w");
}
int main(void) { tc_begin(); TC_RUN(exported_all); TC_RUN(printf_s_family); TC_RUN(scanf_s_family); TC_RUN(snprintf_s_family); TC_RUN(wide_s_family); return tc_end(); }
