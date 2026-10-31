/* TC
 * id:    prc_neg_wspawnl_invalid_mode
 * area:  process
 * kind:  negative
 * funcs: _wspawnl
 * crt:   >=80
 * tier:  p0
 * conf:  inferred
 * ref:   MS docs _spawn, _wspawn functions; Parameter validation (EINVAL + invalid parameter handler)
 * note:  Args are '/c exit 0' so nothing blocks if a CRT fails to validate the mode. The invalid-parameter-handler call for a bad mode is not asserted (only -1/EINVAL are documented).
 */
#include "tc.h"
#include <process.h>
#include <errno.h>

TC_CASE(invalid_mode)
{
    intptr_t r; volatile int mode = 99;
    tc_iph_install(); tc_iph_count = 0; errno = 0;
    r = _wspawnl(mode, L"cmd.exe", L"cmd.exe", L"/c", L"exit 0", (const wchar_t *)NULL);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EINVAL);
}

int main(void) { tc_begin(); TC_RUN(invalid_mode); return tc_end(); }
