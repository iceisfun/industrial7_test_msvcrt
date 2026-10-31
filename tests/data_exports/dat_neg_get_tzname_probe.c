/* TC
 * id:    dat_neg_get_tzname_probe
 * area:  data_exports
 * kind:  negative
 * funcs: _get_tzname _tzset _putenv
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _get_tzname (silent on these edge cases)
 * note:  Probes (0) buffer too small returns ERANGE, (1) buffer NULL + size 0 is a size query (pReturnValue = 4 for "PST"), (2) on ERANGE the buffer is reset to an empty
 *        string, (3) on ERANGE *pReturnValue is still the required size, (4) non-NULL buffer with size 0 is EINVAL.
 */
#include "tc.h"
#include <errno.h>
#include <stdlib.h>
#include <time.h>

static volatile size_t v_sz;

TC_CASE(buffer_too_small_erange) { size_t r = 0; char b[8]; v_sz = 2; CHECK_EQ_I(_get_tzname(&r, b, v_sz, 0), ERANGE); }
TC_CASE(size_zero_null_buf_queries_required_size) { size_t r = 99; char *volatile p = NULL; v_sz = 0; CHECK_EQ_I(_get_tzname(&r, p, v_sz, 0), 0); CHECK_EQ_U(r, 4); }
TC_CASE(erange_leaves_buffer_empty_string) { size_t r = 0; char b[8] = "xxxxxxx"; v_sz = 2; CHECK_EQ_I(_get_tzname(&r, b, v_sz, 0), ERANGE); CHECK_EQ_I(b[0], 0); }
TC_CASE(erange_sets_returnvalue_required_size) { size_t r = 0; char b[8]; v_sz = 2; CHECK_EQ_I(_get_tzname(&r, b, v_sz, 0), ERANGE); CHECK_EQ_U(r, 4); }
TC_CASE(size_zero_nonnull_buf_einval) { size_t r = 0; char b[8]; v_sz = 0; CHECK_EQ_I(_get_tzname(&r, b, v_sz, 0), EINVAL); }

int main(void)
{
    tc_begin(); tc_iph_install();
    _putenv("TZ=PST8PDT"); _tzset();
    TC_RUN(buffer_too_small_erange); TC_RUN(size_zero_null_buf_queries_required_size); TC_RUN(erange_leaves_buffer_empty_string);
    TC_RUN(erange_sets_returnvalue_required_size); TC_RUN(size_zero_nonnull_buf_einval);
    return tc_end();
}
