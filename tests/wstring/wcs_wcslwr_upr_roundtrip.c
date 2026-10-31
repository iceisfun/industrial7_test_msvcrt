/* TC
 * id:    wcs_wcslwr_upr_roundtrip
 * area:  wstring
 * funcs: _wcslwr _wcsupr
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _wcslwr, _wcsupr (C locale)
 */
#include "tc.h"
#include <wchar.h>

static void fill(wchar_t *b) { int i; for (i = 1; i < 128; i++) b[i - 1] = (wchar_t)i; b[127] = 0; }

TC_CASE(lwr_maps_only_AZ)
{
    wchar_t b[128]; int i; wchar_t *volatile p = b; fill(b); _wcslwr(p);
    for (i = 1; i < 128; i++) {
        wchar_t want = (i >= 'A' && i <= 'Z') ? (wchar_t)(i + 32) : (wchar_t)i;
        if (b[i - 1] != want) { CHECK_EQ_U(b[i - 1], want); break; }
    }
    CHECK_EQ_U(b[127], 0);
}
TC_CASE(upr_maps_only_az)
{
    wchar_t b[128]; int i; wchar_t *volatile p = b; fill(b); _wcsupr(p);
    for (i = 1; i < 128; i++) {
        wchar_t want = (i >= 'a' && i <= 'z') ? (wchar_t)(i - 32) : (wchar_t)i;
        if (b[i - 1] != want) { CHECK_EQ_U(b[i - 1], want); break; }
    }
    CHECK_EQ_U(b[127], 0);
}
TC_CASE(full_buffer_length)
{
    wchar_t b[128]; int n = 0; wchar_t *volatile p = b; fill(b); _wcslwr(p); _wcsupr(p);
    while (b[n]) n++;
    CHECK_EQ_I(n, 127);
}
int main(void) { tc_begin(); TC_RUN(lwr_maps_only_AZ); TC_RUN(upr_maps_only_az); TC_RUN(full_buffer_length); return tc_end(); }
