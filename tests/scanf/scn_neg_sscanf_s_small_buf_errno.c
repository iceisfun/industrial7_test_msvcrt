/* TC
 * id:    scn_neg_sscanf_s_small_buf_errno
 * area:  scanf
 * kind:  negative
 * funcs: sscanf_s
 * crt:   >=140
 * tier:  p1
 * conf:  observed
 * ref:   MS docs scanf_s; Parameter validation
 * note:  Probes errno and invalid-parameter-handler behaviour after sscanf_s("%s") into a too-small buffer.
 *        Guess: errno == ERANGE and the invalid-parameter handler IS invoked at least once (secure-CRT buffer-too-small
 *        convention, cf. scn_neg_child_sscanf_s_overflow_fatal). Either could differ.
 *        CORRECTED for Win10 22H2 ucrtbase (observed 2026-10-31): a too-small _s destination returns 0 with errno=ENOMEM (12) (not ERANGE) and does NOT call the invalid parameter handler (count 0).
 *        MS docs do not document either. Stable (2 runs).
 *        Observed on Win10 22H2 ucrtbase only; the same expectation on the other CRTs (>=140) is [unverified] and must be confirmed when those CRTs are run in the VM.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static volatile const char *v;
TC_CASE(errno_range) { char a[8]; int n; tc_iph_install(); tc_iph_count = 0; errno = 0; v = "abcdef"; n = sscanf_s((const char *)v, "%s", a, (unsigned)4); CHECK_EQ_I(n, 0); CHECK_EQ_I(errno, ENOMEM); }
TC_CASE(handler_called) { char a[8]; int n; tc_iph_install(); tc_iph_count = 0; v = "abcdef"; n = sscanf_s((const char *)v, "%s", a, (unsigned)4); CHECK_EQ_I(n, 0); CHECK_EQ_I(tc_iph_count, 0); }
int main(void) { tc_begin(); TC_RUN(errno_range); TC_RUN(handler_called); return tc_end(); }
