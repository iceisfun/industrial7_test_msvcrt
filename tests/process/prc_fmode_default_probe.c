/* TC
 * id:    prc_fmode_default_probe
 * area:  process
 * procs: __p__fmode _get_fmode
 * crt:   >=80
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _fmode (default text mode)
 * note:  Probes the initial _fmode value: numeric 0 vs _O_TEXT (0x4000) is not clear from the docs. A mingw-built test exe's startup
 *        code (txtmode.o) copies its own _fmode (_O_TEXT) into the CRT, so 0 and _O_TEXT are both accepted; anything else is a finding.
 */
#include "tc.h"
#include <fcntl.h>

TC_DECL_PROC(int *, __p__fmode, (void));
TC_DECL_PROC(int, _get_fmode, (int *));

TC_CASE(startup_value) { CHECK(tc_p___p__fmode != NULL); if (!tc_p___p__fmode) return; { int v = *tc_p___p__fmode(); CHECK(v == 0 || v == _O_TEXT); } }
TC_CASE(get_fmode)
{
    int m = -1;
    CHECK(tc_p__get_fmode != NULL); if (!tc_p__get_fmode) return;
    CHECK_EQ_I(tc_p__get_fmode(&m), 0);
    CHECK(m == 0 || m == _O_TEXT);
}

int main(void)
{
    tc_begin();
    TC_LOAD_PROC(__p__fmode); TC_LOAD_PROC(_get_fmode);
    TC_RUN(startup_value); TC_RUN(get_fmode);
    return tc_end();
}
