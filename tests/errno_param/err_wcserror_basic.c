/* TC
 * id:    err_wcserror_basic
 * area:  errno_param
 * funcs: _wcserror strerror
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs strerror, _strerror, _wcserror, __wcserror
 */
#include "tc.h"
#include <string.h>
#include <errno.h>
#include <wchar.h>

TC_CASE(enoent_matches_strerror)
{
    const wchar_t *w = _wcserror(ENOENT); const char *s = strerror(ENOENT); size_t i;
    CHECK(w != NULL);
    if (!w) return;
    for (i = 0; s[i]; i++) CHECK_EQ_I(w[i], (unsigned char)s[i]);
    CHECK_EQ_I(w[i], 0);
}
TC_CASE(zero_nonnull) { volatile int e = 0; CHECK(_wcserror(e) != NULL); }
TC_CASE(range_nonnull)
{
    int e;
    for (e = 0; e <= 42; e++) {
        volatile int v = e;
        const wchar_t *w = _wcserror(v);
        CHECK(w != NULL);
        if (w) CHECK(w[0] != 0);
    }
}
int main(void)
{
    tc_begin();
    TC_RUN(enoent_matches_strerror); TC_RUN(zero_nonnull); TC_RUN(range_nonnull);
    return tc_end();
}
