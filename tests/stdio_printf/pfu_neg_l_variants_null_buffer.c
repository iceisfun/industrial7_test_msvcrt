/* TC
 * id:    pfu_neg_l_variants_null_buffer
 * area:  stdio_printf
 * kind:  negative
 * funcs: _sprintf_s_l _snprintf_s_l _swprintf_s_l _snwprintf_s_l _vsprintf_s_l _vswprintf_s_l _vsnprintf_s_l _vsnwprintf_s_l
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * conf:  documented
 * ref:   MS docs sprintf_s/_sprintf_s_l (NULL buffer or too-small buffer -> invalid parameter handler, EINVAL / ERANGE, return -1); _snprintf_s/_snprintf_s_l (count < size or _TRUNCATE truncates, returns -1, no handler)
 * note:  Only the return value, handler count and (for NULL) EINVAL are asserted. errno of the too-small path is not asserted.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <locale.h>
#include <errno.h>

static _locale_t loc;
#define TRUNC ((size_t)-1)

static int a_vsprintf(char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = _vsprintf_s_l(b, n, f, loc, ap); va_end(ap); return r; }
static int a_vsnprintf(char *b, size_t n, size_t c, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = _vsnprintf_s_l(b, n, c, f, loc, ap); va_end(ap); return r; }
static int a_vswprintf(wchar_t *b, size_t n, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = _vswprintf_s_l(b, n, f, loc, ap); va_end(ap); return r; }
static int a_vsnwprintf(wchar_t *b, size_t n, size_t c, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = _vsnwprintf_s_l(b, n, c, f, loc, ap); va_end(ap); return r; }

#define NEG(call) do { int r_; tc_iph_count = 0; errno = 0; r_ = (call); \
    CHECK_EQ_I(r_, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL); } while (0)

TC_CASE(null_narrow)
{
    char *volatile p = NULL; volatile size_t n = 16;
    NEG(_sprintf_s_l(p, n, "abc", loc));
    NEG(_snprintf_s_l(p, n, 4, "abc", loc));
    NEG(a_vsprintf(p, n, "abc"));
    NEG(a_vsnprintf(p, n, 4, "abc"));
}
TC_CASE(null_wide)
{
    wchar_t *volatile p = NULL; volatile size_t n = 16;
    NEG(_swprintf_s_l(p, n, L"abc", loc));
    NEG(_snwprintf_s_l(p, n, 4, L"abc", loc));
    NEG(a_vswprintf(p, n, L"abc"));
    NEG(a_vsnwprintf(p, n, 4, L"abc"));
}
#define SMALL(call, b, z) do { int r_; tc_iph_count = 0; r_ = (call); \
    CHECK_EQ_I(r_, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I((b)[0], 0); (void)(z); } while (0)
TC_CASE(too_small_handler)
{
    char b[16]; wchar_t w[16]; volatile size_t n = 4;
    SMALL(_sprintf_s_l(b, n, "%s", loc, "abcdefgh"), b, 0);
    SMALL(a_vsprintf(b, n, "%s", "abcdefgh"), b, 0);
    SMALL(_swprintf_s_l(w, n, L"%ls", loc, L"abcdefgh"), w, 0);
    SMALL(a_vswprintf(w, n, L"%ls", L"abcdefgh"), w, 0);
}
TC_CASE(truncate_no_handler)
{
    char b[16]; wchar_t w[16]; volatile size_t n = 4; int r;
    tc_iph_count = 0;
    r = _snprintf_s_l(b, n, TRUNC, "%s", loc, "abcdefgh"); CHECK_EQ_I(r, -1); CHECK_STR(b, "abc");
    r = a_vsnprintf(b, n, TRUNC, "%s", "abcdefgh");        CHECK_EQ_I(r, -1); CHECK_STR(b, "abc");
    r = _snwprintf_s_l(w, n, TRUNC, L"%ls", loc, L"abcdefgh"); CHECK_EQ_I(r, -1); CHECK_WSTR(w, L"abc");
    r = a_vsnwprintf(w, n, TRUNC, L"%ls", L"abcdefgh");    CHECK_EQ_I(r, -1); CHECK_WSTR(w, L"abc");
    r = _snprintf_s_l(b, 16, 2, "%s", loc, "abcdefgh");    CHECK_EQ_I(r, -1); CHECK_STR(b, "ab");
    r = _snwprintf_s_l(w, 16, 2, L"%ls", loc, L"abcdefgh"); CHECK_EQ_I(r, -1); CHECK_WSTR(w, L"ab");
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    loc = _create_locale(LC_ALL, "C"); CHECK(loc != NULL);
    TC_RUN(null_narrow); TC_RUN(null_wide); TC_RUN(too_small_handler); TC_RUN(truncate_no_handler);
    _free_locale(loc);
    return tc_end();
}
