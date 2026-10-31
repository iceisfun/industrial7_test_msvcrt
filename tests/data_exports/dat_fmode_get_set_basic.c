/* TC
 * id:    dat_fmode_get_set_basic
 * area:  data_exports
 * funcs: _get_fmode _set_fmode
 * procs: __p__fmode
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _fmode, _set_fmode, _get_fmode
 * note:  _set_fmode(0) is invalid (0 is not _O_TEXT), so the original value is restored via __p__fmode (undocumented accessor, used
 *        only for restoring). _set_fmode accepts _O_TEXT (0x4000) and _O_BINARY (0x8000); the value is stored as given.
 *        The literal default value and accessor/_get_fmode agreement are in dat_fmode_default_probe.
 */
#include "tc.h"
#include <stdlib.h>
#include <fcntl.h>

typedef int *(__cdecl *pf_t)(void);
static pf_t p_fmode;
static int initial = -1, initial_ok = -1;

static int cur(void) { int m = -99; _get_fmode(&m); return m; }

TC_CASE(get_succeeds)
{
    CHECK_EQ_I(initial_ok, 0);
}
TC_CASE(set_binary_roundtrip)
{
    CHECK_EQ_I(_set_fmode(_O_BINARY), 0);
    CHECK_EQ_I(cur(), _O_BINARY);
}
TC_CASE(set_text_roundtrip)
{
    CHECK_EQ_I(_set_fmode(_O_TEXT), 0);
    CHECK_EQ_I(cur(), _O_TEXT);
}
TC_CASE(restore_original)
{
    /* 0 itself is not an accepted _set_fmode argument (only _O_TEXT/_O_BINARY/...), so restore through the data accessor */
    if (!p_fmode) TC_SKIP("__p__fmode not exported");
    *p_fmode() = initial;
    CHECK_EQ_I(cur(), initial);
}

int main(void)
{
    tc_begin();
    p_fmode = (pf_t)tc_crt_proc("__p__fmode");
    initial_ok = _get_fmode(&initial);
    TC_RUN(get_succeeds); TC_RUN(set_binary_roundtrip); TC_RUN(set_text_roundtrip);
    TC_RUN(restore_original);
    return tc_end();
}
