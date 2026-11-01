/* TC
 * id:    pfq_neg_sprintf_p_small_buffer_probe
 * area:  stdio_printf
 * kind:  negative
 * funcs: _sprintf_p
 * procs: _set_invalid_parameter_handler
 * crt:   >=90
 * tier:  p0
 * conf:  observed
 * ref:   MS docs _sprintf_p
 * note:  OBSERVED on ucrtbase (Win10 22H2 x64 (19045) ucrtbase.dll observed, raw: "too_small r == -1 got=-2"; zzp_probe2 "_sprintf_p inline small: r=-2 errno=0"): _sprintf_p returns -2 (not -1)
 *        when the result does not fit in count (legacy truncation result), and writes only within count (guard bytes beyond count stay 0xAA,
 *        cases exact_no_nul_room/count_zero unchanged: count 0 returns a negative value, -1). -1 kept for pre-UCRT CRTs (unverified). Stable.
 *        NOTE (class F dependence): _sprintf_p here is the mingw-w64 inline wrapper, which passes the raw ucrtbase -2 through; a program built with
 *        MSVC headers would see -1. -2 is the raw __stdio_common_vsprintf_p export result (observed), not a documented value.
 */
#include "tc.h"
#include <stdio.h>

static const char * volatile f = "%1$s%2$s";

TC_CASE(too_small)
{
    char b[16]; int i, r;
    for (i = 0; i < 16; i++) b[i] = (char)0xAA;
    r = _sprintf_p(b, 4, f, "abc", "def");   /* needs 7 + NUL */
#if TC_CRT_VER >= 140
    CHECK_EQ_I(r, -2);   /* ucrtbase observed */
#else
    CHECK_EQ_I(r, -1);
#endif
    for (i = 4; i < 16; i++) CHECK_EQ_U((unsigned char)b[i], 0xAA);
}
TC_CASE(exact_no_nul_room)
{
    char b[16]; int i, r;
    for (i = 0; i < 16; i++) b[i] = (char)0xAA;
    r = _sprintf_p(b, 6, f, "abc", "def");   /* 6 chars, NUL does not fit; return value not asserted */
    (void)r;
    for (i = 6; i < 16; i++) CHECK_EQ_U((unsigned char)b[i], 0xAA);
}
TC_CASE(count_zero)
{
    char b[4]; int r;
    b[0] = 'q';
    r = _sprintf_p(b, 0, f, "a", "b");
    CHECK(r < 0);
    CHECK_EQ_I(b[0], 'q');
}

int main(void)
{
    tc_begin();
    tc_iph_install();
    TC_RUN(too_small); TC_RUN(exact_no_nul_room); TC_RUN(count_zero);
    return tc_end();
}
