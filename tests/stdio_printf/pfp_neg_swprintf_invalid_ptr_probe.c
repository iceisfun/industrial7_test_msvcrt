/* TC
 * id:    pfp_neg_swprintf_invalid_ptr_probe
 * area:  stdio_printf
 * kind:  negative
 * funcs: swprintf
 * crt:   >=140
 * tier:  p1
 * conf:  probe
 * ref:   MS docs swprintf, _swprintf (C99-conformant swprintf returns negative when output does not fit)
 * note:  PROBE: swprintf(buf, 8, L"%p") cannot fit 16 characters on x64. Return -1 is expected (documented
 *        negative); "invalid-parameter handler not called" and the truncated buffer state are best guesses and live in
 *        separate cases. Buffer state probed: the non-legacy-termination (non _vsnprintf) path is believed to truncate to
 *        count-1 chars and NUL-terminate at b[count-1]; b[0]==0 (the _s behaviour) is NOT expected for this non-_s function.
 *        Bytes at index >= count must stay untouched (that is the only documented part, in count_too_small).
 */
#include "tc.h"
#include <wchar.h>

#define PTR ((void *)(ULONG_PTR)0x1234)

TC_CASE(count_too_small)
{
    wchar_t b[32]; int i, r; void *volatile p = PTR; const wchar_t *volatile f = L"%p";
    if (!TC_X64) TC_SKIP("x64 only");
    for (i = 0; i < 32; i++) b[i] = 0xCCCC;
    tc_iph_count = 0;
    r = swprintf(b, 8, f, p);
    CHECK(r < 0);
    CHECK_EQ_U(b[8], 0xCCCC);
    CHECK_EQ_U(b[31], 0xCCCC);
}
TC_CASE(probe_truncated_terminated)
{
    wchar_t b[32]; int i; void *volatile p = PTR; const wchar_t *volatile f = L"%p";
    if (!TC_X64) TC_SKIP("x64 only");
    for (i = 0; i < 32; i++) b[i] = 0xCCCC;
    swprintf(b, 8, f, p);
    CHECK_EQ_U(b[7], 0);
    CHECK_EQ_U(b[0], L'0');
}
TC_CASE(probe_handler_not_called_on_overflow)
{
    wchar_t b[32]; void *volatile p = PTR; const wchar_t *volatile f = L"%p";
    if (!TC_X64) TC_SKIP("x64 only");
    tc_iph_count = 0;
    swprintf(b, 8, f, p);
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(fits_control)
{
    wchar_t b[32]; int r; void *volatile p = PTR; const wchar_t *volatile f = L"%p";
    if (!TC_X64) TC_SKIP("x64 only");
    r = swprintf(b, 32, f, p);
    CHECK_EQ_I(r, 16); CHECK_WSTR(b, L"0000000000001234");
}

int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(fits_control); TC_RUN(count_too_small); TC_RUN(probe_truncated_terminated); TC_RUN(probe_handler_not_called_on_overflow);
    return tc_end();
}
