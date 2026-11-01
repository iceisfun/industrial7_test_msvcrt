/* TC
 * id:    dat_time_vars_dstbias_probe
 * area:  data_exports
 * funcs: _tzset _putenv _get_dstbias
 * crt:   ucrtbase
 * tier:  p2
 * kind:  positive
 * conf:  observed
 * ref:   MS docs _get_dstbias / _tzset
 * note:  Probe: how _dstbias reacts to TZ. Original guess: PST8PDT -> -3600 and UTC0 -> 0.
 * note:  OBSERVED (Win10 22H2 19045 ucrtbase; raw "b == 0 got=-3600"; analysis: tzset_from_environment_nolock never writes _dstbias, only
 *        tzset_from_system_nolock does): setting TZ (PST8PDT or UTC0) and calling _tzset leaves _dstbias UNCHANGED. The absolute value comes from the
 *        initial default / the system time zone (-3600 on the Pacific reference VM, 0 on a zone without DST), so it is NOT asserted; the test asserts
 *        invariance relative to the value read after the first system-zone _tzset. Class F (time-zone dependence removed). Narrowed to ucrtbase
 *        (no legacy-CRT evidence).
 */
#include "tc.h"
#include <stdlib.h>
#include <time.h>

static long base;
static long tzbias(const char *env) { long b = 12345; _putenv(env); _tzset(); CHECK_EQ_I(_get_dstbias(&b), 0); return b; }
TC_CASE(pst8pdt_dstbias_unchanged) { long b = tzbias("TZ=PST8PDT"); CHECK_EQ_I(b, base); _putenv("TZ="); _tzset(); }
TC_CASE(utc0_dstbias_unchanged) { long b = tzbias("TZ=UTC0"); CHECK_EQ_I(b, base); _putenv("TZ="); _tzset(); }
int main(void) { tc_begin(); _putenv("TZ="); _tzset(); _get_dstbias(&base); TC_RUN(pst8pdt_dstbias_unchanged); TC_RUN(utc0_dstbias_unchanged); return tc_end(); }
