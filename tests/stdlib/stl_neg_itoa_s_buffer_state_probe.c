/* TC
 * id:    stl_neg_itoa_s_buffer_state_probe
 * area:  stdlib
 * funcs:
 * procs: _itoa_s
 * crt:   >=80
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _itoa_s (error-condition table: state of buffer after failure)
 * note:  Probes the buffer contents after failed _itoa_s: too small buffer (ERANGE) and bad radix (EINVAL) leave an
 *        empty string (b[0]==0); size 0 leaves the buffer untouched. Each is its own case.
 */
#include "tc.h"
#include <errno.h>
#include <string.h>

TC_DECL_PROC(errno_t, _itoa_s, (int, char *, size_t, int));

TC_CASE(too_small_empties) { char b[8]; memset(b, 'x', sizeof b); tc_iph_install(); (void)tc_p__itoa_s(12345, b, 3, 10); CHECK_EQ_I(b[0], 0); }
TC_CASE(bad_radix_empties) { char b[8]; memset(b, 'x', sizeof b); tc_iph_install(); (void)tc_p__itoa_s(5, b, sizeof b, 37); CHECK_EQ_I(b[0], 0); }
TC_CASE(size0_untouched)   { char b[8]; memset(b, 'x', sizeof b); tc_iph_install(); (void)tc_p__itoa_s(5, b, 0, 10); CHECK_EQ_I(b[0], 'x'); }
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_itoa_s);
    if (!tc_p__itoa_s) { tc_write("TC FAIL stl_neg_itoa_s_buffer_state_probe::__import _itoa_s missing\n"); return 1; }
    TC_RUN(too_small_empties); TC_RUN(bad_radix_empties); TC_RUN(size0_untouched);
    return tc_end();
}
