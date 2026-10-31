/* TC
 * id:    pfw_stdio_common_snprintf_standard
 * area:  stdio_printf
 * funcs:
 * procs: __stdio_common_vswprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  observed
 * ref:   UCRT corecrt_stdio_config.h _CRT_INTERNAL_PRINTF_STANDARD_SNPRINTF_BEHAVIOR (1<<1); C11 7.29.2.3 swprintf / 7.21.6.5 snprintf semantics
 * note:  With option bit 2 set, truncation returns the would-be length and the buffer is NUL-terminated (C99 semantics, not -1).
 */
#include "tc.h"
#include <stdarg.h>
#include <stddef.h>

typedef int (__cdecl *vsw_t)(unsigned long long, wchar_t *, size_t, const wchar_t *, void *, va_list);
static vsw_t p_vsw;
#define OPT_STD_SNPRINTF 2ULL

static int wf(wchar_t *b, size_t n, const wchar_t *fmt, ...)
{
    va_list ap; int r;
    va_start(ap, fmt); r = p_vsw(OPT_STD_SNPRINTF, b, n, fmt, NULL, ap); va_end(ap);
    return r;
}
static void fill(wchar_t *b) { int i; for (i = 0; i < 16; i++) b[i] = L'Z'; }
TC_CASE(trunc_ret_5_buf_hel0)
{
    wchar_t b[16]; int n; if (!p_vsw) TC_SKIP("no export"); fill(b);
    n = wf(b, 4, L"%ls", L"hello");
    CHECK_EQ_I(n, 5); CHECK(b[0] == L'h' && b[1] == L'e' && b[2] == L'l' && b[3] == 0); CHECK(b[4] == L'Z');
}
TC_CASE(count0_null_buffer_returns_len)
{
    int n; if (!p_vsw) TC_SKIP("no export");
    n = wf(NULL, 0, L"%ls-%d", L"hello", 42); CHECK_EQ_I(n, 8);
}
TC_CASE(exact_fit)
{
    wchar_t b[16]; int n; if (!p_vsw) TC_SKIP("no export"); fill(b);
    n = wf(b, 6, L"%ls", L"hello"); CHECK_EQ_I(n, 5); CHECK_WSTR(b, L"hello"); CHECK(b[6] == L'Z');
}
TC_CASE(fits)
{
    wchar_t b[16]; int n; if (!p_vsw) TC_SKIP("no export"); fill(b);
    n = wf(b, 16, L"%d", 123); CHECK_EQ_I(n, 3); CHECK_WSTR(b, L"123");
}
int main(void)
{
    tc_begin(); p_vsw = (vsw_t)tc_crt_proc("__stdio_common_vswprintf");
    TC_RUN(trunc_ret_5_buf_hel0); TC_RUN(count0_null_buffer_returns_len); TC_RUN(exact_fit); TC_RUN(fits);
    return tc_end();
}
