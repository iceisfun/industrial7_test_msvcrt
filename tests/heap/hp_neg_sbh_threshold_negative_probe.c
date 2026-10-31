/* TC
 * id:    hp_neg_sbh_threshold_negative_probe
 * area:  heap
 * kind:  negative
 * funcs: _set_sbh_threshold _get_sbh_threshold
 * crt:   msvcr80 msvcr90 msvcrt
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _set_sbh_threshold (returns 1 on success, 0 on failure; maximum 1016 bytes, 32-bit only)
 * note:  PROBING _set_sbh_threshold with 0, 1016/1017 and (size_t)-1. Values above 1016 must be rejected with 0 and leave the
 *        threshold unchanged. The result for 0 and 1016 is only recorded (TC INFO) because x64 builds have no small-block heap
 *        and may always return 0. The original threshold is restored at the end.
 */
#include "tc.h"
#include <stdlib.h>
#include <malloc.h>

static volatile size_t v_t;
static void info(const char *n, long long v) { char b[32]; tc_i64(v, b); tc_write("TC INFO " TC_TEST_ID " "); tc_write(n); tc_write("="); tc_write(b); tc_write("\n"); }

TC_CASE(zero_value)
{
    size_t before = _get_sbh_threshold(); int r; v_t = 0;
    r = _set_sbh_threshold(v_t);
    CHECK(r == 0 || r == 1);
    info("set0_ret", r); info("get_after", (long long)_get_sbh_threshold());
    if (r == 0) CHECK_EQ_U(_get_sbh_threshold(), before);
    v_t = before; _set_sbh_threshold(v_t);
}
TC_CASE(size_max)
{
    size_t before = _get_sbh_threshold(); v_t = (size_t)-1;
    CHECK_EQ_I(_set_sbh_threshold(v_t), 0);
    CHECK_EQ_U(_get_sbh_threshold(), before);
}
TC_CASE(boundary_1016_1017)
{
    size_t before = _get_sbh_threshold(); int r;
    v_t = 1016; r = _set_sbh_threshold(v_t);
    info("set1016_ret", r); info("get_after1016", (long long)_get_sbh_threshold());
    v_t = 1017;
    CHECK_EQ_I(_set_sbh_threshold(v_t), 0);
    if (r == 1) CHECK_EQ_U(_get_sbh_threshold(), 1016);
    v_t = before; _set_sbh_threshold(v_t);
}
int main(void) { tc_begin(); TC_RUN(zero_value); TC_RUN(size_max); TC_RUN(boundary_1016_1017); return tc_end(); }
