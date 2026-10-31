/* TC
 * id:    pfu_neg_stdio_common_vsprintf_s_direct
 * area:  stdio_printf
 * kind:  negative
 * funcs: __stdio_common_vsprintf_s __stdio_common_vsnprintf_s __stdio_common_vswprintf_s __stdio_common_vsnwprintf_s
 * procs: _set_invalid_parameter_handler
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs sprintf_s, _snprintf_s, swprintf_s, _snwprintf_s (via corecrt_stdio_config/stdio.h inline wrappers: options=0, locale NULL)
 * note:  worker signatures: vsprintf_s(opts, buf, bufcount, fmt, locale, ap); vsnprintf_s(opts, buf, bufcount, maxcount, fmt, locale, ap).
 *        Buffer-too-small asserts a non-zero errno only (ERANGE per docs of sprintf_s family; value not relied upon).
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

static int c_s(char *b, size_t n, const char *f, ...) { va_list ap; int r; va_start(ap, f); r = __stdio_common_vsprintf_s(0, b, n, f, NULL, ap); va_end(ap); return r; }
static int c_sn(char *b, size_t n, size_t m, const char *f, ...) { va_list ap; int r; va_start(ap, f); r = __stdio_common_vsnprintf_s(0, b, n, m, f, NULL, ap); va_end(ap); return r; }
static int c_ws(wchar_t *b, size_t n, const wchar_t *f, ...) { va_list ap; int r; va_start(ap, f); r = __stdio_common_vswprintf_s(0, b, n, f, NULL, ap); va_end(ap); return r; }
static int c_wsn(wchar_t *b, size_t n, size_t m, const wchar_t *f, ...) { va_list ap; int r; va_start(ap, f); r = __stdio_common_vsnwprintf_s(0, b, n, m, f, NULL, ap); va_end(ap); return r; }

TC_CASE(vsprintf_s_too_small)
{
    char b[16]; volatile size_t n = 4; int r; int i;
    for (i = 0; i < 16; i++) b[i] = 'Z';
    tc_iph_count = 0; errno = 0;
    r = c_s(b, n, "%s", "abcdefgh");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK(errno != 0); CHECK_EQ_I(b[0], 0);
    for (i = 4; i < 16; i++) CHECK_EQ_I(b[i], 'Z');
}
TC_CASE(vsprintf_s_null_buffer) { volatile char *b = NULL; int r; tc_iph_count = 0; errno = 0; r = c_s((char *)b, 10, "x"); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(vsprintf_s_null_format) { char b[8]; volatile const char *f = NULL; int r; tc_iph_count = 0; errno = 0; r = c_s(b, sizeof b, (const char *)f); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(vsnprintf_s_truncate)
{
    char b[8]; volatile size_t m = (size_t)-1; int r, i;
    for (i = 0; i < 8; i++) b[i] = 'Z';
    tc_iph_count = 0;
    r = c_sn(b, 4, m, "%s", "abcdefgh");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 0); CHECK_STR(b, "abc"); CHECK_EQ_I(b[4], 'Z');
}
TC_CASE(vswprintf_s_zero_size) { wchar_t b[4] = {L'Z', 0, 0, 0}; volatile size_t n = 0; int r; tc_iph_count = 0; errno = 0; r = c_ws(b, n, L"%d", 1); CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK(errno != 0); CHECK_EQ_I(b[0], L'Z'); }
TC_CASE(vsnwprintf_s_truncate)
{
    wchar_t b[8]; volatile size_t m = (size_t)-1; int r, i;
    for (i = 0; i < 8; i++) b[i] = L'Z';
    tc_iph_count = 0;
    r = c_wsn(b, 4, m, L"%ls", L"abcdefgh");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 0);
    CHECK(b[0] == L'a' && b[1] == L'b' && b[2] == L'c' && b[3] == 0); CHECK(b[4] == L'Z');
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(vsprintf_s_too_small); TC_RUN(vsprintf_s_null_buffer); TC_RUN(vsprintf_s_null_format);
    TC_RUN(vsnprintf_s_truncate); TC_RUN(vswprintf_s_zero_size); TC_RUN(vsnwprintf_s_truncate);
    return tc_end();
}
