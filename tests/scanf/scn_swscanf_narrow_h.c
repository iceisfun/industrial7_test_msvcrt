/* TC
 * id:    scn_swscanf_narrow_h
 * area:  scanf
 * funcs: swscanf
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs scanf type field characters: h with s/c in wscanf = single-byte char
 * note:  In swscanf, %hs/%hc/%h[ store narrow chars (ASCII input only).
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static volatile const wchar_t *v;

TC_CASE(hs) { char b[16]; int r, i; for (i = 0; i < 16; i++) b[i] = 'Z'; v = L"abc def";
    r = swscanf((const wchar_t *)v, L"%hs", b); CHECK_EQ_I(r, 1); CHECK_STR(b, "abc"); CHECK_EQ_I(b[4], 'Z'); }
TC_CASE(hc) { char b[4]; int r, i; for (i = 0; i < 4; i++) b[i] = 'Z'; v = L"qrs";
    r = swscanf((const wchar_t *)v, L"%hc", b); CHECK_EQ_I(r, 1); CHECK_EQ_I(b[0], 'q'); CHECK_EQ_I(b[1], 'Z'); }
TC_CASE(hs_two) { char a[8], b[8]; int r; v = L"foo bar";
    r = swscanf((const wchar_t *)v, L"%hs %hs", a, b); CHECK_EQ_I(r, 2); CHECK_STR(a, "foo"); CHECK_STR(b, "bar"); }
TC_CASE(hs_width) { char b[16]; int r, i; for (i = 0; i < 16; i++) b[i] = 'Z'; v = L"abcdef";
    r = swscanf((const wchar_t *)v, L"%2hs", b); CHECK_EQ_I(r, 1); CHECK_STR(b, "ab"); CHECK_EQ_I(b[3], 'Z'); }
int main(void) { tc_begin(); TC_RUN(hs); TC_RUN(hc); TC_RUN(hs_two); TC_RUN(hs_width); return tc_end(); }
