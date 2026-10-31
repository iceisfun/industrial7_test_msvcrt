/* TC
 * id:    dat_time_vars_dstbias_probe
 * area:  data_exports
 * funcs: _tzset _putenv _get_dstbias
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _get_dstbias / _tzset
 * note:  Probe: with TZ=PST8PDT, _dstbias is -3600 (seconds; sign convention from CRT behaviour); with TZ=UTC0 (no DST name) _dstbias is 0.
 */
#include "tc.h"
#include <stdlib.h>
#include <time.h>

TC_CASE(pst8pdt_dstbias_minus3600) { long b = 12345; _putenv("TZ=PST8PDT"); _tzset(); CHECK_EQ_I(_get_dstbias(&b), 0); CHECK_EQ_I(b, -3600); _putenv("TZ="); _tzset(); }
TC_CASE(utc0_dstbias_zero_or_default) { long b = 12345; _putenv("TZ=UTC0"); _tzset(); CHECK_EQ_I(_get_dstbias(&b), 0); CHECK_EQ_I(b, 0); _putenv("TZ="); _tzset(); }
int main(void) { tc_begin(); TC_RUN(pst8pdt_dstbias_minus3600); TC_RUN(utc0_dstbias_zero_or_default); return tc_end(); }
