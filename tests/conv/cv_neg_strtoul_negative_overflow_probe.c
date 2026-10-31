/* TC
 * id:    cv_neg_strtoul_negative_overflow_probe
 * area:  conv
 * funcs: strtoul
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.22.1.4 (magnitude out of range -> ERANGE); Windows behaviour unconfirmed
 * note:  PROBE: strtoul("-4294967296") on Windows: expected ULONG_MAX + ERANGE (magnitude exceeds unsigned long). Windows may instead return 0 or wrap without ERANGE.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

TC_CASE(neg_2p32)
{
    volatile const char *s = "-4294967296"; char *e; unsigned long r;
    errno = 0; r = strtoul((const char *)s, &e, 10);
    CHECK_EQ_U(r, ULONG_MAX); CHECK_EQ_I(errno, ERANGE);
}

int main(void) { tc_begin(); TC_RUN(neg_2p32); return tc_end(); }
