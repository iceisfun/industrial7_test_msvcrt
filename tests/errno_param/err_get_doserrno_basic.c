/* TC
 * id:    err_get_doserrno_basic
 * area:  errno_param
 * funcs: 
 * procs: _get_doserrno _set_doserrno __doserrno
 * crt:   >=100
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _get_doserrno, _set_doserrno
 */
#include "tc.h"
#include <errno.h>

TC_DECL_PROC(errno_t, _get_doserrno, (unsigned long *));
TC_DECL_PROC(errno_t, _set_doserrno, (unsigned long));
TC_DECL_PROC(unsigned long *, __doserrno, (void));

TC_CASE(set_get)
{
    unsigned long v = 99;
    CHECK_EQ_I(tc_p__set_doserrno(5), 0);
    CHECK_EQ_I(tc_p__get_doserrno(&v), 0); CHECK_EQ_U(v, 5);
}
TC_CASE(clear)
{
    unsigned long v = 99;
    tc_p__set_doserrno(5);
    CHECK_EQ_I(tc_p__set_doserrno(0), 0);
    CHECK_EQ_I(tc_p__get_doserrno(&v), 0); CHECK_EQ_U(v, 0);
}
TC_CASE(via_pointer)
{
    tc_p__set_doserrno(87); CHECK_EQ_U(*tc_p___doserrno(), 87);
    *tc_p___doserrno() = 6;
    { unsigned long v = 0; tc_p__get_doserrno(&v); CHECK_EQ_U(v, 6); }
    tc_p__set_doserrno(0);
}
TC_CASE(roundtrip)
{
    static const unsigned long vals[] = { 2, 3, 0x7FFFFFFFul };
    int i;
    for (i = 0; i < 3; i++) {
        unsigned long v = 0;
        CHECK_EQ_I(tc_p__set_doserrno(vals[i]), 0);
        CHECK_EQ_I(tc_p__get_doserrno(&v), 0); CHECK_EQ_U(v, vals[i]);
    }
    tc_p__set_doserrno(0);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_get_doserrno); TC_LOAD_PROC(_set_doserrno); TC_LOAD_PROC(__doserrno);
    if (!tc_p__get_doserrno || !tc_p__set_doserrno || !tc_p___doserrno) { tc_write("TC FAIL " TC_TEST_ID "::__setup\n"); return 1; }
    TC_RUN(set_get); TC_RUN(clear); TC_RUN(via_pointer); TC_RUN(roundtrip);
    return tc_end();
}
