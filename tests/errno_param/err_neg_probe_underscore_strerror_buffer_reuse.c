/* TC
 * id:    err_neg_probe_underscore_strerror_buffer_reuse
 * area:  errno_param
 * kind:  negative
 * funcs: _strerror strerror
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _strerror, strerror ("may be overwritten by subsequent call")
 * note:  PROBE: _strerror(NULL) returns the same per-thread buffer on every call, so the first text is overwritten by the second
 *        (the docs only say the result may be overwritten). strerror's reuse of a single buffer is NOT asserted for known errnos
 *        (the CRT may return static literals); it is only recorded as a TC INFO line.
 */
#include "tc.h"
#include <string.h>
#include <errno.h>

TC_CASE(underscore_strerror_same_buffer)
{
    char *p1, *p2; volatile int e1 = ENOENT, e2 = EACCES;
    errno = e1; p1 = _strerror(NULL);
    CHECK(p1 != NULL); if (!p1) return;
    errno = e2; p2 = _strerror(NULL);
    CHECK(p2 != NULL);
    CHECK_EQ_P(p1, p2);
    CHECK_STR(p1, "Permission denied\n");   /* first result now holds the second text */
}
TC_CASE(strerror_info)
{
    char *p1, *p2; char b[24];
    p1 = strerror(ENOENT); p2 = strerror(EACCES);
    CHECK(p1 != NULL && p2 != NULL);
    tc_write("TC INFO strerror same_ptr="); tc_i64(p1 == p2, b); tc_write(b); tc_write("\n");
}
int main(void) { tc_begin(); TC_RUN(underscore_strerror_same_buffer); TC_RUN(strerror_info); errno = 0; return tc_end(); }
