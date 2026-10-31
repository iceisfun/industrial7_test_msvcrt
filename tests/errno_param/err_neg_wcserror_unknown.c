/* TC
 * id:    err_neg_wcserror_unknown
 * area:  errno_param
 * kind:  negative
 * funcs: _wcserror
 * crt:   all
 * tier:  p2
 * conf:  inferred
 * ref:   MS docs _wcserror (returns a message string); unmapped-number handling is not documented, inferred from strerror
 */
#include "tc.h"
#include <errno.h>
#include <wchar.h>

TC_CASE(minus1) { volatile int e = -1; CHECK(_wcserror(e) != NULL); }
TC_CASE(e9999)
{
    volatile int e = 9999; const wchar_t *w = _wcserror(e);
    CHECK(w != NULL);
    if (w) CHECK(w[0] != 0);
}
int main(void)
{
    tc_begin();
    TC_RUN(minus1); TC_RUN(e9999);
    return tc_end();
}
