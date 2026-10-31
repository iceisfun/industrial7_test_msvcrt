/* TC
 * id:    dat_neg_get_tzname
 * area:  data_exports
 * kind:  negative
 * funcs: _get_tzname _tzset _putenv
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _get_tzname: EINVAL for index not 0/1, NULL pReturnValue, NULL buffer with size>0
 * note:  Behaviour not spelled out in the docs (incl. the ERANGE small-buffer path) lives in dat_neg_get_tzname_probe.
 */
#include "tc.h"
#include <errno.h>
#include <stdlib.h>
#include <time.h>

static volatile int v_idx; static volatile size_t v_sz;

TC_CASE(index_2_einval) { size_t r = 0; char b[16]; int c = tc_iph_count; v_idx = 2; CHECK_EQ_I(_get_tzname(&r, b, sizeof b, v_idx), EINVAL); CHECK(tc_iph_count > c); }
TC_CASE(index_neg1_einval) { size_t r = 0; char b[16]; int c = tc_iph_count; v_idx = -1; CHECK_EQ_I(_get_tzname(&r, b, sizeof b, v_idx), EINVAL); CHECK(tc_iph_count > c); }
TC_CASE(null_retval_ptr_einval) { char b[16]; int c = tc_iph_count; size_t *volatile p = NULL; CHECK_EQ_I(_get_tzname(p, b, sizeof b, 0), EINVAL); CHECK(tc_iph_count > c); }
TC_CASE(null_buf_with_size_einval) { size_t r = 0; int c = tc_iph_count; char *volatile p = NULL; v_sz = 10; CHECK_EQ_I(_get_tzname(&r, p, v_sz, 0), EINVAL); CHECK(tc_iph_count > c); }
TC_CASE(valid_index0_index1)
{
    size_t r = 0; char b[16];
    CHECK_EQ_I(_get_tzname(&r, b, sizeof b, 0), 0); CHECK_EQ_U(r, 4); CHECK_STR(b, "PST");
    CHECK_EQ_I(_get_tzname(&r, b, sizeof b, 1), 0); CHECK_EQ_U(r, 4); CHECK_STR(b, "PDT");
}

int main(void)
{
    tc_begin(); tc_iph_install();
    _putenv("TZ=PST8PDT"); _tzset();
    TC_RUN(index_2_einval); TC_RUN(index_neg1_einval); TC_RUN(null_retval_ptr_einval); TC_RUN(null_buf_with_size_einval);
    TC_RUN(valid_index0_index1);
    return tc_end();
}
