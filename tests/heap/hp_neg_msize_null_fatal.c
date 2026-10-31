/* TC
 * id:    hp_neg_msize_null_fatal
 * area:  heap
 * funcs: _msize
 * crt:   win10
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs Parameter validation: default invalid parameter handler terminates
 * note:  Probes the exact status (fast-fail 0xC0000409 expected); docs only promise termination. Child process.
 */
#include "tc.h"
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include <stdint.h>

TC_CASE(child_terminates)
{
    DWORD code = 0; int ran = tc_spawn_self("msize_null", &code, 20000);
    CHECK(ran);
    CHECK(code != 77);
    CHECK(code != 0);
    CHECK_EQ_U(code, TC_STATUS_FASTFAIL);
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) { void *volatile p = NULL; (void)_msize(p); return 77; }
    TC_RUN(child_terminates);
    return tc_end();
}
