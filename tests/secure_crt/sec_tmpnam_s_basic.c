/* TC
 * id:    sec_tmpnam_s_basic
 * area:  secure_crt
 * funcs:
 * procs: tmpnam_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * conf:  documented
 * ref:   MS docs tmpnam_s: returns 0 and a name in the buffer; successive calls give distinct names
 * note:  mingw's L_tmpnam_s differs from MS's, so no assertion depends on it (see sec_tmpnam_s_exact_buffer_probe).
 */
#include "tc.h"
#include <stddef.h>

typedef int (__cdecl *tmpnam_s_t)(char *, size_t);
static tmpnam_s_t p_tmpnam_s;
static size_t slen(const char *s) { size_t n = 0; while (s[n]) n++; return n; }

TC_CASE(returns_0_nonempty)
{
    char b[260]; int r; b[0] = 0;
    r = p_tmpnam_s(b, sizeof b); CHECK_EQ_I(r, 0); CHECK(b[0] != 0);
}
TC_CASE(two_calls_differ)
{
    char a[260], b[260]; int r1, r2;
    r1 = p_tmpnam_s(a, sizeof a); r2 = p_tmpnam_s(b, sizeof b);
    CHECK_EQ_I(r1, 0); CHECK_EQ_I(r2, 0);
    CHECK(!tc_strequ(a, b));
}
int main(void)
{
    tc_begin();
    p_tmpnam_s = (tmpnam_s_t)tc_crt_proc("tmpnam_s");
    CHECK(p_tmpnam_s); if (!p_tmpnam_s) return tc_end();
    TC_RUN(returns_0_nonempty); TC_RUN(two_calls_differ);
    return tc_end();
}
