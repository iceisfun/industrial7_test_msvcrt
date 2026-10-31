/* TC
 * id:    scn_neg_swscanf_s_small_buf
 * area:  scanf
 * kind:  negative
 * funcs: swscanf_s
 * crt:   >=90
 * tier:  p0
 * conf:  probe
 * ref:   MS docs wscanf_s: size must cover the token plus NUL; too small a buffer fails the conversion
 * note:  Probes: swscanf_s %s with size 3 and input abcdef returns 0 and writes nothing at or past a[3] (case too_small); separately (case too_small_reset) a[0] is set to NUL. Handler installed so a continuing handler is assumed.
 */
#include "tc.h"
#include <wchar.h>

static volatile const wchar_t *v;
TC_CASE(too_small) { wchar_t a[8]; int n, i; for (i = 0; i < 8; i++) a[i] = L'Z'; tc_iph_install(); v = L"abcdef"; n = swscanf_s((const wchar_t *)v, L"%s", a, (unsigned)3); CHECK_EQ_I(n, 0); CHECK_EQ_U(a[3], L'Z'); CHECK_EQ_U(a[7], L'Z'); }
TC_CASE(too_small_reset) { wchar_t a[8]; int n, i; for (i = 0; i < 8; i++) a[i] = L'Z'; tc_iph_install(); v = L"abcdef"; n = swscanf_s((const wchar_t *)v, L"%s", a, (unsigned)3); (void)n; CHECK_EQ_U(a[0], 0); }
int main(void) { tc_begin(); TC_RUN(too_small); TC_RUN(too_small_reset); return tc_end(); }
