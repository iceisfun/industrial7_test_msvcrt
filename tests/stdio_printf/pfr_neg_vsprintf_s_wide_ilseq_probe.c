/* TC
 * id:    pfr_neg_vsprintf_s_wide_ilseq_probe
 * area:  stdio_printf
 * kind:  negative
 * funcs: _snprintf_s sprintf_s
 * crt:   ucrtbase
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _snprintf_s, sprintf_s, printf %ls; wctomb_s EILSEQ
 * note:  PROBE: in the "C" locale %ls of L'\x0100' cannot be narrowed: the _s functions return -1 with errno EILSEQ.
 *        The buffer state after the encoding failure (b[0] is 0 vs. untouched) is only recorded, accepting either.
 *        The handler is installed in case it is invoked; its count is not asserted.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

#ifndef _TRUNCATE
#define _TRUNCATE ((size_t)-1)
#endif
static const wchar_t ws[] = { 0x0100, 0 };
static const wchar_t * volatile p = ws;
static const char * volatile f = "%ls";

TC_CASE(snprintf_s_ilseq)
{
    char b[16]; int r;
    b[0] = 'Q'; b[1] = 0;
    errno = 0; r = _snprintf_s(b, 16, _TRUNCATE, f, p);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EILSEQ);
}
TC_CASE(snprintf_s_state)
{
    char b[16]; int r;
    b[0] = 'Q'; b[1] = 0;
    errno = 0; r = _snprintf_s(b, 16, _TRUNCATE, f, p);
    CHECK_EQ_I(r, -1);
    CHECK(b[0] == 0 || b[0] == 'Q');
}
TC_CASE(sprintf_s_ilseq)
{
    char b[16]; int r;
    b[0] = 'Q'; b[1] = 0;
    errno = 0; r = sprintf_s(b, 16, f, p);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EILSEQ);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(snprintf_s_ilseq); TC_RUN(snprintf_s_state); TC_RUN(sprintf_s_ilseq);
    return tc_end();
}
