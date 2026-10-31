/* TC
 * id:    stl_neg_itoa_bad_radix
 * area:  stdlib
 * funcs: _itoa
 * crt:   >=140
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _itoa_s (documented failure contract); _itoa on ucrt is believed to forward to it
 * note:  Probes that ucrt's _itoa with radix 1 / 37 / NULL buffer fails with errno EINVAL and one invalid-parameter
 *        handler call. Each runs in a child process (a CRT that does not validate may crash); child exit code is
 *        10 + (errno==EINVAL ? 1 : 0) + (iph_count==1 ? 2 : 0), so 13 is the expected result. Older CRTs do not
 *        validate radix (radix 1 may hang) so they are excluded. The documented contract is asserted in
 *        stl_neg_itoa_s_small_buffer.
 */
#include "tc.h"
#include <errno.h>
#include <stdlib.h>

static void run(const char *name)
{
    DWORD code = 0; int ran = tc_spawn_self(name, &code, 20000);
    CHECK(ran); CHECK_EQ_U(code, 13);
}
TC_CASE(radix1)   { run("r1"); }
TC_CASE(radix37)  { run("r37"); }
TC_CASE(null_buf) { run("null"); }
int main(void)
{
    const char *c; char b[40];
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        tc_iph_install(); tc_iph_count = 0; errno = 0;
        if (c[0] == 'r' && c[1] == '1' && !c[2]) _itoa(5, b, 1);
        else if (c[0] == 'r') _itoa(5, b, 37);
        else _itoa(5, NULL, 10);
        return 10 + (errno == EINVAL ? 1 : 0) + (tc_iph_count == 1 ? 2 : 0);
    }
    TC_RUN(radix1); TC_RUN(radix37); TC_RUN(null_buf);
    return tc_end();
}
