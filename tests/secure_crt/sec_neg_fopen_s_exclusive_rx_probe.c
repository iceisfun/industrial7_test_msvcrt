/* TC
 * id:    sec_neg_fopen_s_exclusive_rx_probe
 * area:  secure_crt
 * funcs: fopen_s
 * procs: 
 * crt:   ucrtbase
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   C11 'x' exclusive-create mode in UCRT fopen_s
 * note:  PROBE: fopen_s(&f, name, "rx") is rejected with EINVAL ("x" believed valid only with w/a). Guess about UCRT mode parsing.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

#define FN "tc_sec_neg_exclrx.tmp"
TC_CASE(x_without_w_invalid_EINVAL) { FILE *f = (FILE *)(ULONG_PTR)0x1; tc_iph_install(); tc_iph_count = 0; CHECK_EQ_I(fopen_s(&f, FN, "rx"), EINVAL); if (f && f != (FILE *)(ULONG_PTR)0x1) fclose(f); }
int main(void) { tc_begin(); TC_RUN(x_without_w_invalid_EINVAL); remove(FN); return tc_end(); }
