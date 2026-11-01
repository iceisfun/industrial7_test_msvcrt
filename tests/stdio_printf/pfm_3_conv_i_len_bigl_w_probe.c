/* TC
 * id:    pfm_3_conv_i_len_bigl_w_probe
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   ucrtbase
 * tier:  p1
 * kind:  positive
 * conf:  observed
 * ref:   MS docs printf size prefixes (L, w)
 * note:  Observed on Windows 10 22H2 ucrtbase: %Li %wi %LX %wX (L / w size prefix with an integer conversion) are rejected: return -1, errno EINVAL,
 *        one invalid-parameter handler call, nothing written (buf[0] stays 0).
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

static char b[64];
static volatile int v = 42;
#define BODY_T(f, want) do { int r_; (void)want; b[0] = 0; tc_iph_install(); tc_iph_count = 0; errno = 0; r_ = sprintf(b, f "|%d", v, 7); \
    CHECK_EQ_I(r_, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(b[0], 0); } while (0);
TC_CASE(Li) { BODY_T("%Li", "42") }
TC_CASE(wi) { BODY_T("%wi", "42") }
TC_CASE(LX) { BODY_T("%LX", "2A") }
TC_CASE(wX) { BODY_T("%wX", "2A") }
int main(void) { tc_begin(); TC_RUN(Li); TC_RUN(wi); TC_RUN(LX); TC_RUN(wX); return tc_end(); }
