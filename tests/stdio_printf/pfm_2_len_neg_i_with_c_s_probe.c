/* TC
 * id:    pfm_2_len_neg_i_with_c_s_probe
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs printf size prefixes (I, I32, I64 documented only for d i o u x X)
 * note:  PROBE: I/I32/I64 with %c %s %p %e must not crash. Either the call fails (return <0 and the
 *        invalid-parameter handler ran) or it succeeds with a result whose length equals the return value.
 *        Only the consistency of that contract is asserted; the actual outcome is for the Windows run to record.
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>

static volatile int cx = 'x';
static const char sabbuf[8] = "ab";
static const char *volatile sab = sabbuf;
static volatile double onehalf = 1.5;
static void *volatile ptr = (void *)0;

static void contract(int n, const char *b, int iph_before)
{
    if (n < 0) { CHECK(tc_iph_count > iph_before); CHECK_EQ_I(errno, EINVAL); }
    else CHECK_EQ_U(strlen(b), n);
}

TC_CASE(Ic) { char b[64]; int k = tc_iph_count, n; b[0] = 0; errno = 0; n = sprintf(b, "%Ic", cx); contract(n, b, k); }
TC_CASE(I64s) { char b[64]; int k = tc_iph_count, n; b[0] = 0; errno = 0; n = sprintf(b, "%I64s", sab); contract(n, b, k); }
TC_CASE(I32p) { char b[64]; int k = tc_iph_count, n; b[0] = 0; errno = 0; n = sprintf(b, "%I32p", ptr); contract(n, b, k); }
TC_CASE(I64e) { char b[64]; int k = tc_iph_count, n; b[0] = 0; errno = 0; n = sprintf(b, "%I64e", onehalf); contract(n, b, k); }

int main(void) { tc_begin(); tc_iph_install(); TC_RUN(Ic); TC_RUN(I64s); TC_RUN(I32p); TC_RUN(I64e); return tc_end(); }
