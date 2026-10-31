/* TC
 * id:    ucr_neg_iob_func_out_of_range_probe
 * area:  ucrt_only
 * kind:  negative
 * funcs: __acrt_iob_func
 * crt:   win10
 * tier:  p1
 * conf:  probe
 * ref:   UCRT implementation detail (__acrt_iob_func is pointer arithmetic over the std stream array)
 * note:  PROBES that __acrt_iob_func(3) returns a non-NULL pointer different from the indices 0..2, continuing the same stride, and
 *        does not call the invalid-parameter handler. The returned pointer is never dereferenced.
 */
#include "tc.h"
#include <stdio.h>

static volatile unsigned v0 = 0, v1 = 1, v2 = 2, v3 = 3;

TC_CASE(index3_nonnull_distinct)
{
    char *a = (char *)__acrt_iob_func(v0), *b = (char *)__acrt_iob_func(v1), *c = (char *)__acrt_iob_func(v2), *d;
    tc_iph_count = 0;
    d = (char *)__acrt_iob_func(v3);
    CHECK(d != NULL); CHECK(d != a); CHECK(d != b); CHECK(d != c);
}
TC_CASE(index3_stride)
{
    char *b = (char *)__acrt_iob_func(v1), *c = (char *)__acrt_iob_func(v2), *d = (char *)__acrt_iob_func(v3);
    CHECK((d - c) == (c - b));
}
TC_CASE(iph_untouched)
{
    tc_iph_count = 0;
    (void)__acrt_iob_func(v3);
    CHECK_EQ_I(tc_iph_count, 0);
}

int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(index3_nonnull_distinct); TC_RUN(index3_stride); TC_RUN(iph_untouched);
    return tc_end();
}
