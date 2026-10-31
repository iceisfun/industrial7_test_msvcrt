/* TC
 * id:    prc_neg_system_null_command_errno
 * area:  process
 * funcs: system _wsystem
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   C11 7.22.4.8 (system(NULL) reports availability of a command processor); MS docs system/_wsystem
 * note:  system(NULL) is not an invalid call: it returns nonzero when an interpreter exists. No errno change and no
 *        invalid-parameter call are expected on a normal Windows install (the errno/iph part is observed behaviour).
 *        Requires COMSPEC set and pointing at an existing file (true on a normal install; the CRT answers from COMSPEC).
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

TC_CASE(system_null)
{
    int r; tc_iph_install(); tc_iph_count = 0; errno = 0;
    r = system(NULL);
    CHECK(r != 0);
    CHECK_EQ_I(errno, 0);
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(wsystem_null)
{
    int r; tc_iph_install(); tc_iph_count = 0; errno = 0;
    r = _wsystem(NULL);
    CHECK(r != 0);
    CHECK_EQ_I(errno, 0);
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void) { tc_begin(); TC_RUN(system_null); TC_RUN(wsystem_null); return tc_end(); }
