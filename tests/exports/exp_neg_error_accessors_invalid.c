/* TC
 * id:    exp_neg_error_accessors_invalid
 * area:  exports
 * kind:  negative
 * funcs:
 * procs: _get_errno _get_doserrno _set_error_mode _set_invalid_parameter_handler
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _get_errno, _get_doserrno ("returns EINVAL if pointer is NULL"), _set_error_mode ("-1 if invalid mode")
 */
#include "tc.h"

TC_DECL_PROC(int, _get_errno, (int *));
TC_DECL_PROC(int, _get_doserrno, (unsigned long *));
TC_DECL_PROC(int, _set_error_mode, (int));

TC_CASE(get_errno_null)
{
    if (!tc_p__get_errno) TC_SKIP("not exported");
    tc_iph_count = 0;
    CHECK_EQ_I(tc_p__get_errno(NULL), 22);   /* EINVAL */
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(get_doserrno_null)
{
    if (!tc_p__get_doserrno) TC_SKIP("not exported");
    tc_iph_count = 0;
    CHECK_EQ_I(tc_p__get_doserrno(NULL), 22);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(error_mode_invalid)
{
    if (!tc_p__set_error_mode) TC_SKIP("not exported");
    tc_iph_count = 0;
    CHECK_EQ_I(tc_p__set_error_mode(99), -1);
    CHECK_EQ_I(tc_p__set_error_mode(-5), -1);
    CHECK(tc_iph_count >= 1);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_get_errno); TC_LOAD_PROC(_get_doserrno); TC_LOAD_PROC(_set_error_mode);
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__setup no _set_invalid_parameter_handler\n"); return 1; }
    TC_RUN(get_errno_null); TC_RUN(get_doserrno_null); TC_RUN(error_mode_invalid);
    return tc_end();
}
