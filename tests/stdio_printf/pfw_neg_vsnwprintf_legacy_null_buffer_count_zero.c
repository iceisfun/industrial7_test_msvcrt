/* TC
 * id:    pfw_neg_vsnwprintf_legacy_null_buffer_count_zero
 * area:  stdio_printf
 * funcs: _vsnwprintf
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs vsnprintf, _vsnprintf, _vsnwprintf (NULL buffer with count 0 returns required length in UCRT; invalid parameter for NULL buffer with nonzero count)
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>
#include <errno.h>

static int vs(wchar_t *b, size_t n, const wchar_t *fmt, ...)
{ va_list ap; int r; va_start(ap, fmt); r = _vsnwprintf(b, n, fmt, ap); va_end(ap); return r; }
static const wchar_t *volatile fabc = L"abc";
static wchar_t *volatile nullp = NULL;
static volatile size_t zero_n = 0, eight = 8;

TC_CASE(null_count0_len)
{
    int n;
    tc_iph_count = 0;
    n = vs(nullp, zero_n, fabc);
#if TC_CRT_VER >= 140
    CHECK_EQ_I(n, 3); CHECK_EQ_I(tc_iph_count, 0);
#else
    CHECK(n == -1 || n == 3);
#endif
}
TC_CASE(null_count8)
{
    int n; errno = 0; tc_iph_count = 0;
    n = vs(nullp, eight, fabc);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(buf_count0_untouched)
{
    wchar_t b[4] = { L'S', L'S', L'S', L'S' };
    vs(b, zero_n, fabc);
    CHECK_EQ_I(b[0], L'S'); CHECK_EQ_I(b[1], L'S'); CHECK_EQ_I(b[2], L'S'); CHECK_EQ_I(b[3], L'S');
}
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(null_count0_len); TC_RUN(null_count8); TC_RUN(buf_count0_untouched);
    return tc_end();
}
