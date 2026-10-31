/* TC
 * id:    scn_neg_sscanf_s_small_buf_errno
 * area:  scanf
 * kind:  negative
 * funcs: sscanf_s
 * crt:   >=140
 * tier:  p1
 * conf:  probe
 * ref:   MS docs scanf_s; Parameter validation
 * note:  Probes errno and invalid-parameter-handler behaviour after sscanf_s("%s") into a too-small buffer.
 *        Guess: errno == ERANGE and the invalid-parameter handler IS invoked at least once (secure-CRT buffer-too-small
 *        convention, cf. scn_neg_child_sscanf_s_overflow_fatal). Either could differ.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static volatile const char *v;
TC_CASE(errno_range) { char a[8]; int n; tc_iph_install(); tc_iph_count = 0; errno = 0; v = "abcdef"; n = sscanf_s((const char *)v, "%s", a, (unsigned)4); CHECK_EQ_I(n, 0); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(handler_called) { char a[8]; int n; tc_iph_install(); tc_iph_count = 0; v = "abcdef"; n = sscanf_s((const char *)v, "%s", a, (unsigned)4); CHECK_EQ_I(n, 0); CHECK(tc_iph_count >= 1); }
int main(void) { tc_begin(); TC_RUN(errno_range); TC_RUN(handler_called); return tc_end(); }
