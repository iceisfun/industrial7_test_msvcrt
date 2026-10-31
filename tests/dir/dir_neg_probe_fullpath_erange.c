/* TC
 * id:    dir_neg_probe_fullpath_erange
 * area:  dir
 * funcs: _fullpath _wfullpath
 * crt:   >=80
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _fullpath (returns NULL when the result does not fit); errno value not documented
 * note:  PROBE: when the absolute path does not fit in maxlen, _fullpath/_wfullpath return NULL AND set errno to
 *        ERANGE. The NULL return is documented; the ERANGE errno is from memory of the CRT implementation only.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

static const char *volatile v_x = "x";
static const wchar_t *volatile v_wx = L"x";

TC_CASE(fullpath_small_erange)
{
    char b[4] = { 'Q', 0, 0, 0 };
    tc_iph_install(); errno = 0;
    CHECK(_fullpath(b, v_x, 2) == NULL);
    CHECK_EQ_I(errno, ERANGE);
}

TC_CASE(wfullpath_small_erange)
{
    wchar_t b[4] = { L'Q', 0, 0, 0 };
    tc_iph_install(); errno = 0;
    CHECK(_wfullpath(b, v_wx, 2) == NULL);
    CHECK_EQ_I(errno, ERANGE);
}

int main(void) { tc_begin(); TC_RUN(fullpath_small_erange); TC_RUN(wfullpath_small_erange); return tc_end(); }
