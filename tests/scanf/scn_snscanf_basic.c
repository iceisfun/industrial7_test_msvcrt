/* TC
 * id:    scn_snscanf_basic
 * area:  scanf
 * funcs: _snscanf
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs sscanf, _sscanf_l, swscanf, _swscanf_l, _snscanf ("count: maximum number of characters to read")
 */
#include "tc.h"
#include <stdio.h>

static volatile size_t cnt;

TC_CASE(int_truncated) { int v = 0, r; cnt = 3; r = _snscanf("12345", cnt, "%d", &v); CHECK_EQ_I(r, 1); CHECK_EQ_I(v, 123); }
TC_CASE(count_ge_len) { int v = 0, r; cnt = 100; r = _snscanf("12345", cnt, "%d", &v); CHECK_EQ_I(r, 1); CHECK_EQ_I(v, 12345); cnt = 5; v = 0; r = _snscanf("12345", cnt, "%d", &v); CHECK_EQ_I(v, 12345); }
TC_CASE(string_truncated) { char b[16]; int r; b[0] = 0; cnt = 3; r = _snscanf("abcdef", cnt, "%s", b); CHECK_EQ_I(r, 1); CHECK_STR(b, "abc"); }
TC_CASE(char_count_two) { char c[3] = {'-','-','-'}; int r; cnt = 2; r = _snscanf("abc", cnt, "%c%c%c", &c[0], &c[1], &c[2]); CHECK_EQ_I(r, 2); CHECK_EQ_I(c[0], 'a'); CHECK_EQ_I(c[1], 'b'); CHECK_EQ_I(c[2], '-'); }
TC_CASE(second_int_cut) { int a = 0, b = 0, r; cnt = 4; r = _snscanf("12 345", cnt, "%d %d", &a, &b); CHECK_EQ_I(r, 2); CHECK_EQ_I(a, 12); CHECK_EQ_I(b, 3); }
TC_CASE(second_int_absent) { int a = 0, b = 77, r; cnt = 3; r = _snscanf("12 345", cnt, "%d %d", &a, &b); CHECK_EQ_I(r, 1); CHECK_EQ_I(a, 12); CHECK_EQ_I(b, 77); }

int main(void) { tc_begin(); TC_RUN(int_truncated); TC_RUN(count_ge_len); TC_RUN(string_truncated); TC_RUN(char_count_two); TC_RUN(second_int_cut); TC_RUN(second_int_absent); return tc_end(); }
