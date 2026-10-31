/* TC
 * id:    scn_neg_sscanf_s_small_buf
 * area:  scanf
 * kind:  negative
 * funcs: sscanf_s
 * crt:   >=140
 * tier:  p0
 * conf:  probe
 * ref:   MS docs scanf_s, _scanf_s_l, wscanf_s: buffer size must follow %s/%c/%[ pointer; too small a buffer fails the read
 * note:  Probes what happens when the buffer-size argument to sscanf_s is too small for the token. Expected (documented
 *        in the "Security Features in the CRT" page): conversion fails, return value is the count converted so far (0),
 *        buffer[0] is set to NUL, no overflow past the given size. Whether errno becomes ERANGE and the invalid
 *        parameter handler runs is NOT asserted here (see scn_neg_sscanf_s_small_buf_errno).
 */
#include "tc.h"
#include <stdio.h>

static volatile const char *v;
TC_CASE(too_small_s) { char a[8]; int n, i; for (i = 0; i < 8; i++) a[i] = 'Z'; tc_iph_install(); v = "abcdef"; n = sscanf_s((const char *)v, "%s", a, (unsigned)4); CHECK_EQ_I(n, 0); CHECK_EQ_I(a[0], 0); CHECK_EQ_I(a[4], 'Z'); CHECK_EQ_I(a[7], 'Z'); }
TC_CASE(exact_fits) { char a[8]; int n, i; for (i = 0; i < 8; i++) a[i] = 'Z'; v = "abc"; n = sscanf_s((const char *)v, "%s", a, (unsigned)4); CHECK_EQ_I(n, 1); CHECK_STR(a, "abc"); CHECK_EQ_I(a[4], 'Z'); }
TC_CASE(char_too_small) { char a[4]; int n, i; for (i = 0; i < 4; i++) a[i] = 'Z'; tc_iph_install(); v = "abcd"; n = sscanf_s((const char *)v, "%3c", a, (unsigned)2); CHECK_EQ_I(n, 0); CHECK_EQ_I(a[2], 'Z'); CHECK_EQ_I(a[3], 'Z'); }
int main(void) { tc_begin(); TC_RUN(too_small_s); TC_RUN(exact_fits); TC_RUN(char_too_small); return tc_end(); }
