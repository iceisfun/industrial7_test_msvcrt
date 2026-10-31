/* TC
 * id:    stl_div_struct_layout
 * area:  stdlib
 * funcs: div ldiv
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs div, ldiv (div_t, ldiv_t); LLP64 data model
 */
#include "tc.h"
#include <stdlib.h>
#include <stddef.h>
#include <limits.h>

static volatile long la, lb; static volatile int ia, ib;

TC_CASE(div_t_layout) { div_t d; ia = 17; ib = 5; d = div(ia, ib); CHECK_EQ_I(d.quot, 3); CHECK_EQ_I(d.rem, 2);
    CHECK_EQ_U(offsetof(div_t, quot), 0); CHECK_EQ_U(offsetof(div_t, rem), sizeof(int)); CHECK_EQ_U(sizeof(div_t), 2 * sizeof(int)); }
TC_CASE(ldiv_t_layout) { CHECK_EQ_U(offsetof(ldiv_t, quot), 0); CHECK_EQ_U(offsetof(ldiv_t, rem), sizeof(long)); CHECK_EQ_U(sizeof(ldiv_t), 8); }
TC_CASE(long_is_32bit) { CHECK_EQ_U(sizeof(long), 4); CHECK_EQ_I(LONG_MAX, 2147483647L); }
TC_CASE(ldiv_member_use)
{
    ldiv_t r; unsigned char raw[8];
    int i, q, m;
    la = 2000000000L; lb = 3; r = ldiv(la, lb);
    CHECK_EQ_I(r.quot, 666666666L); CHECK_EQ_I(r.rem, 2L);
    for (i = 0; i < 8; i++) raw[i] = ((unsigned char *)&r)[i];
    q = (int)(raw[0] | raw[1] << 8 | raw[2] << 16 | (unsigned)raw[3] << 24);
    m = (int)(raw[4] | raw[5] << 8 | raw[6] << 16 | (unsigned)raw[7] << 24);
    CHECK_EQ_I(q, 666666666); CHECK_EQ_I(m, 2); /* little-endian: quot first, then rem */
}

int main(void) { tc_begin(); TC_RUN(div_t_layout); TC_RUN(ldiv_t_layout); TC_RUN(long_is_32bit); TC_RUN(ldiv_member_use); return tc_end(); }
