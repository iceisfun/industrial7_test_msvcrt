/* TC
 * id:    exp_secure_string_exported
 * area:  exports
 * funcs:
 * procs: strcpy_s strcat_s strncpy_s strncat_s memcpy_s memmove_s wcscpy_s wcscat_s wcsncpy_s wcsncat_s _strlwr_s _strupr_s _wcslwr_s _wcsupr_s strnlen wcsnlen _strset_s _strnset_s strtok_s wcstok_s
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs "Security-enhanced versions of CRT functions" (VS2005); strcpy_s, strnlen, strtok_s
 * note:  msvcr80's import library lacks these, hence procs. Behaviour cases only use valid arguments.
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

typedef int (__cdecl *f_s3)(char *, size_t, const char *);
typedef int (__cdecl *f_s4)(char *, size_t, const char *, size_t);
typedef int (__cdecl *f_w3)(wchar_t *, size_t, const wchar_t *);
typedef int (__cdecl *f_m)(void *, size_t, const void *, size_t);
typedef int (__cdecl *f_c)(char *, size_t);
typedef size_t (__cdecl *f_nlen)(const char *, size_t);
typedef char *(__cdecl *f_tok)(char *, const char *, char **);

TC_CASE(exported_all)
{
    static const char *const n[] = { "strcpy_s", "strcat_s", "strncpy_s", "strncat_s", "memcpy_s", "memmove_s", "wcscpy_s", "wcscat_s",
        "wcsncpy_s", "wcsncat_s", "_strlwr_s", "_strupr_s", "_wcslwr_s", "_wcsupr_s", "strnlen", "wcsnlen", "_strset_s", "_strnset_s",
        "strtok_s", "wcstok_s", 0 };
    need(n);
}
TC_CASE(narrow_copy_cat)
{
    f_s3 cpy = (f_s3)tc_crt_proc("strcpy_s"), cat = (f_s3)tc_crt_proc("strcat_s");
    f_s4 ncpy = (f_s4)tc_crt_proc("strncpy_s"), ncat = (f_s4)tc_crt_proc("strncat_s");
    char b[16];
    if (!cpy || !cat || !ncpy || !ncat) TC_SKIP("not exported");
    CHECK_EQ_I(cpy(b, sizeof b, "abc"), 0); CHECK_STR(b, "abc");
    CHECK_EQ_I(cat(b, sizeof b, "def"), 0); CHECK_STR(b, "abcdef");
    CHECK_EQ_I(ncpy(b, sizeof b, "xyzzy", 3), 0); CHECK_STR(b, "xyz");
    CHECK_EQ_I(ncat(b, sizeof b, "12345", 2), 0); CHECK_STR(b, "xyz12");
}
TC_CASE(wide_copy_cat)
{
    f_w3 cpy = (f_w3)tc_crt_proc("wcscpy_s"), cat = (f_w3)tc_crt_proc("wcscat_s");
    wchar_t b[16];
    if (!cpy || !cat) TC_SKIP("not exported");
    CHECK_EQ_I(cpy(b, 16, L"ab"), 0); CHECK_WSTR(b, L"ab");
    CHECK_EQ_I(cat(b, 16, L"cd"), 0); CHECK_WSTR(b, L"abcd");
}
TC_CASE(case_s)
{
    f_c lw = (f_c)tc_crt_proc("_strlwr_s"), up = (f_c)tc_crt_proc("_strupr_s");
    char b[16] = "MiXed1";
    if (!lw || !up) TC_SKIP("not exported");
    CHECK_EQ_I(lw(b, sizeof b), 0); CHECK_STR(b, "mixed1");
    CHECK_EQ_I(up(b, sizeof b), 0); CHECK_STR(b, "MIXED1");
}
TC_CASE(strnlen_call)
{
    f_nlen p = (f_nlen)tc_crt_proc("strnlen");
    if (!p) TC_SKIP("not exported");
    CHECK_EQ_U(p("abcdef", 3), 3); CHECK_EQ_U(p("ab", 10), 2);
}
TC_CASE(tok_s)
{
    f_tok p = (f_tok)tc_crt_proc("strtok_s");
    char s[] = "a,b;c", *ctx = NULL, *t;
    if (!p) TC_SKIP("not exported");
    t = p(s, ",;", &ctx); CHECK_STR(t, "a");
    t = p(NULL, ",;", &ctx); CHECK_STR(t, "b");
    t = p(NULL, ",;", &ctx); CHECK_STR(t, "c");
    t = p(NULL, ",;", &ctx); CHECK(t == NULL);
}
TC_CASE(memory_s)
{
    f_m cp = (f_m)tc_crt_proc("memcpy_s"), mv = (f_m)tc_crt_proc("memmove_s");
    char b[8] = "abcdefg";
    if (!cp || !mv) TC_SKIP("not exported");
    CHECK_EQ_I(cp(b, 8, "xyz", 3), 0); CHECK_STR(b, "xyzdefg");
    CHECK_EQ_I(mv(b + 1, 7, b, 3), 0); CHECK_STR(b, "xxyzefg");
}
int main(void)
{
    tc_begin();
    TC_RUN(exported_all); TC_RUN(narrow_copy_cat); TC_RUN(wide_copy_cat); TC_RUN(case_s); TC_RUN(strnlen_call); TC_RUN(tok_s); TC_RUN(memory_s);
    return tc_end();
}
