/* TC
 * id:    pfm_2_vsnprintf_c99_conv_matrix
 * area:  stdio_printf
 * funcs:
 * procs: vsnprintf
 * crt:   >=120
 * tier:  p0
 * conf:  documented
 * note:  ucrtbase: vsnprintf is an inline over __stdio_common_vsprintf (header). msvcr120: called through the export; the C99 length/termination
 *        semantics (truncation, count 0) are asserted on ucrtbase only.
 * ref:   MS docs vsnprintf (C99 behaviour: always null-terminates, returns the would-be length); C11 7.21.6.12
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>
#include <string.h>

static volatile int v7 = 7, v8 = 8, v255 = 255;
static volatile wchar_t vwc = L'q';
static const wchar_t *volatile ws = L"wide";

#if TC_CRT_VER >= 140
#define CALL_VSN vsnprintf
#define HAVE_VSN 1
#else
typedef int (__cdecl *vsn_t)(char *, size_t, const char *, va_list);
static vsn_t p_vsn;
#define CALL_VSN p_vsn
#define HAVE_VSN (p_vsn != NULL)
#endif
static int vsn(char *b, size_t n, const char *f, ...) { va_list ap; int r; va_start(ap, f); r = CALL_VSN(b, n, f, ap); va_end(ap); return r; }

TC_CASE(ints_chars)
{
    if (!HAVE_VSN) TC_SKIP("vsnprintf not exported");
    char b[64]; int r = vsn(b, 64, "%i|%o|%X|%C|%S", v7, v8, v255, (int)vwc, ws);
    CHECK_STR(b, "7|10|FF|q|wide"); CHECK_EQ_I(r, 14);
}
TC_CASE(truncation_returns_full_length)
{
    char b[8]; int r;
    if (TC_CRT_VER < 140) TC_SKIP("ucrt semantics");
    memset(b, 'Z', sizeof b);
    r = vsn(b, 4, "%s", "abcdefg");
    CHECK_EQ_I(r, 7); CHECK_STR(b, "abc");
}
TC_CASE(zero_count_measures)
{
    char b[4] = "zzz"; int r;
    if (TC_CRT_VER < 140) TC_SKIP("ucrt semantics");
    r = vsn(b, 0, "%d-%X", 123, v255);
    CHECK_EQ_I(r, 6); CHECK_STR(b, "zzz");
}
TC_CASE(null_buffer_zero_count)
{
    int r;
    if (TC_CRT_VER < 140) TC_SKIP("ucrt semantics");
    r = vsn(NULL, 0, "%i|%o", v7, v8);
    CHECK_EQ_I(r, 4);
}
int main(void)
{
    tc_begin();
#if TC_CRT_VER < 140
    p_vsn = (vsn_t)tc_crt_proc("vsnprintf");
#endif
    TC_RUN(ints_chars); TC_RUN(truncation_returns_full_length); TC_RUN(zero_count_measures); TC_RUN(null_buffer_zero_count);
    return tc_end();
}
