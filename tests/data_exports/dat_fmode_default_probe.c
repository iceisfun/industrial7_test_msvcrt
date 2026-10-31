/* TC
 * id:    dat_fmode_default_probe
 * area:  data_exports
 * funcs: _get_fmode _set_fmode
 * procs: __p__fmode
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _fmode (default text translation mode)
 * note:  PROBE: (1) the literal initial value of _fmode in a program not linked with binmode.obj is 0 (docs only say "text").
 *        (2) the undocumented __p__fmode accessor returns the address of the same variable _get_fmode/_set_fmode use.
 *        The test exe is built by mingw, whose startup might alter _fmode; a non-zero default here means that, not a CRT bug.
 */
#include "tc.h"
#include <stdlib.h>
#include <fcntl.h>

typedef int *(__cdecl *pf_t)(void);
static pf_t p_fmode;
static int initial = -1, initial_ok = -1;
static int cur(void) { int m = -99; _get_fmode(&m); return m; }

TC_CASE(default_is_zero)
{
    CHECK_EQ_I(initial_ok, 0);
    CHECK_EQ_I(initial, 0);
}
TC_CASE(accessor_reflects_set)
{
    if (!p_fmode) TC_SKIP("__p__fmode not exported");
    CHECK(p_fmode() != NULL);
    CHECK_EQ_I(_set_fmode(_O_BINARY), 0);
    CHECK_EQ_I(*p_fmode(), _O_BINARY);
    *p_fmode() = _O_TEXT;
    CHECK_EQ_I(cur(), _O_TEXT);
    *p_fmode() = initial;
}

int main(void)
{
    tc_begin();
    p_fmode = (pf_t)tc_crt_proc("__p__fmode");
    initial_ok = _get_fmode(&initial);
    TC_RUN(default_is_zero); TC_RUN(accessor_reflects_set);
    return tc_end();
}
