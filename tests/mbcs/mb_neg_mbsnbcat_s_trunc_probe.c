/* TC
 * id:    mb_neg_mbsnbcat_s_trunc_probe
 * area:  mbcs
 * funcs:
 * procs: _mbsnbcat_s
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs strncat_s (_TRUNCATE); MBCS variants assumed to follow
 * note:  PROBE: count == _TRUNCATE with a too-small dst returns STRUNCATE (80), copies what fits, NUL terminates, no handler call.
 */
#include "tc.h"
#include <mbstring.h>
#include <errno.h>
#ifndef STRUNCATE
#define STRUNCATE 80
#endif
typedef int (__cdecl *ncat_t)(unsigned char *, size_t, const unsigned char *, size_t);
static ncat_t p_ncat;
static unsigned char sb[] = "cdefgh";
static const unsigned char * volatile src = sb;
TC_CASE(truncate_returns_STRUNCATE)
{
    unsigned char d[5] = {'a','b',0};
    CHECK(p_ncat); if (!p_ncat) return;
    tc_iph_count = 0;
    CHECK_EQ_I(p_ncat(d, 5, src, (size_t)-1), STRUNCATE);
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(truncate_result_terminated)
{
    unsigned char d[5] = {'a','b',0};
    CHECK(p_ncat); if (!p_ncat) return;
    p_ncat(d, 5, src, (size_t)-1);
    CHECK_STR((char *)d, "abcd");
}
int main(void)
{
    tc_begin(); tc_iph_install();
    p_ncat = (ncat_t)tc_crt_proc("_mbsnbcat_s");
    TC_RUN(truncate_returns_STRUNCATE); TC_RUN(truncate_result_terminated);
    return tc_end();
}
