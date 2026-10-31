/* TC
 * id:    dat_mb_cur_max_accessor_win7
 * area:  data_exports
 * funcs: __p___mb_cur_max ___mb_cur_max_func
 * crt:   msvcr120 msvcr110 msvcr100
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   (undocumented) __p___mb_cur_max is the accessor behind MB_CUR_MAX in these CRT headers
 * note:  Probe: the pointer returned by __p___mb_cur_max points to an int equal to ___mb_cur_max_func() and is 1 in the C locale.
 */
#include "tc.h"
#include <stdlib.h>

TC_CASE(data_equals_func)
{
    int *p = __p___mb_cur_max(); CHECK(p != NULL);
    if (p) CHECK_EQ_I(*p, ___mb_cur_max_func());
}
TC_CASE(accessor_ptr_equals_func_value) { int *p = __p___mb_cur_max(), *q = __p___mb_cur_max(); CHECK_EQ_P(p, q); if (p) CHECK_EQ_I(*p, MB_CUR_MAX); }
TC_CASE(c_locale_1) { int *p = __p___mb_cur_max(); if (p) CHECK_EQ_I(*p, 1); }
int main(void) { tc_begin(); TC_RUN(data_equals_func); TC_RUN(accessor_ptr_equals_func_value); TC_RUN(c_locale_1); return tc_end(); }
