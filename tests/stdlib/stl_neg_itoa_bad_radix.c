/* TC
 * id:    stl_neg_itoa_bad_radix
 * area:  stdlib
 * funcs: _itoa
 * crt:   >=140
 * tier:  p2
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _itoa_s (documented failure contract); _itoa on ucrt is believed to forward to it
 * note:  Each case runs in a child process; child exit code is 10 + (errno==EINVAL ? 1 : 0) + (iph_count==1 ? 2 : 0) when the
 *        call returns. OBSERVED on Win10 22H2 ucrtbase: _itoa(5,b,1) and _itoa(5,NULL,10) crash with an access violation
 *        (0xC0000005); _itoa(5,b,37) returns without errno/handler (exit 10). The _itoa_s-style EINVAL contract does not apply
 *        to _itoa.
 */
#include "tc.h"
#include <errno.h>
#include <stdlib.h>

static void run(const char *name, DWORD want)
{
    DWORD code = 0; int ran = tc_spawn_self(name, &code, 20000);
    CHECK(ran); CHECK_EQ_U(code, want);
}
/* Observed on Win10 22H2 ucrtbase: radix 1 and a NULL buffer fault (class C, 0xC0000005); radix 37 is not rejected
 * (no errno, no handler call: child code 10). */
TC_CASE(radix1)   { run("r1", 0xC0000005u); }
TC_CASE(radix37)  { run("r37", 10); }
TC_CASE(null_buf) { run("null", 0xC0000005u); }
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
