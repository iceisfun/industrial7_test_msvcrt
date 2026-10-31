/* TC
 * id:    scn_sscanf_wide_specs_narrow
 * area:  scanf
 * funcs: sscanf
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs scanf type field characters (C/S wide in scanf; hs/hc narrow)
 * note:  ASCII input only, C locale. In narrow scanf %S/%C/%lc/%ls store wchar_t.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <string.h>

static volatile const char *v;
TC_CASE(lc) { wchar_t w = 0; int n; v = "A"; n = sscanf((const char *)v, "%lc", &w); CHECK_EQ_I(n, 1); CHECK_EQ_U(w, 'A'); }
TC_CASE(ls) { wchar_t w[16]; int n, i; for (i = 0; i < 16; i++) w[i] = 0x7777; v = "hello world"; n = sscanf((const char *)v, "%ls", w); CHECK_EQ_I(n, 1); CHECK_WSTR(w, L"hello"); CHECK_EQ_U(w[6], 0x7777); }
TC_CASE(S) { wchar_t w[16]; int n; v = "abc def"; n = sscanf((const char *)v, "%S", w); CHECK_EQ_I(n, 1); CHECK_WSTR(w, L"abc"); }
TC_CASE(C) { wchar_t w = 0; int n; v = "Z"; n = sscanf((const char *)v, "%C", &w); CHECK_EQ_I(n, 1); CHECK_EQ_U(w, 'Z'); }
TC_CASE(hs_hc) { char s[16], c = 0; int n; v = "xyz Q"; n = sscanf((const char *)v, "%hs %hc", s, &c); CHECK_EQ_I(n, 2); CHECK_STR(s, "xyz"); CHECK_EQ_I(c, 'Q'); }
TC_CASE(lc_width) { wchar_t w[4]; int n, i; for (i = 0; i < 4; i++) w[i] = 0x7777; v = "abcd"; n = sscanf((const char *)v, "%3lc", w); CHECK_EQ_I(n, 1); CHECK_EQ_U(w[0], 'a'); CHECK_EQ_U(w[1], 'b'); CHECK_EQ_U(w[2], 'c'); CHECK_EQ_U(w[3], 0x7777); }
int main(void) { tc_begin(); TC_RUN(lc); TC_RUN(ls); TC_RUN(S); TC_RUN(C); TC_RUN(hs_hc); TC_RUN(lc_width); return tc_end(); }
