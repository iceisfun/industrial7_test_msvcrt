/* TC
 * id:    dat_neg_get_tzname_boundary_probe
 * area:  data_exports
 * funcs: _get_tzname _tzset _putenv
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _get_tzname (ERANGE, pReturnValue = required size incl. NUL)
 * note:  PROBE: exact size boundary, *ret on ERANGE, buf[0]==0 after ERANGE, no writes beyond size. Uses TZ=PST8PDT.
 */
#include "tc.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* buffer 16 bytes filled with 0x7e; call with `size`; sentinel check covers bytes [size, 16) */
static char g[16]; static size_t gn; static int gr;
static void run(size_t size, int idx)
{ memset(g, 0x7e, sizeof g); gn = 99; gr = _get_tzname(&gn, g, size, idx); }
static void sentinel(size_t from) { size_t i; for (i = from; i < sizeof g; i++) if (g[i] != 0x7e) { CHECK(0 && "write past size"); break; } }

TC_CASE(size3_erange) { run(3, 0); CHECK_EQ_I(gr, ERANGE); CHECK_EQ_U(gn, 4); CHECK_EQ_I(g[0], 0); sentinel(3); }
TC_CASE(size4_ok) { run(4, 0); CHECK_EQ_I(gr, 0); CHECK_STR(g, "PST"); CHECK_EQ_U(gn, 4); sentinel(4); }
TC_CASE(size5_ok_tail_untouched) { run(5, 0); CHECK_EQ_I(gr, 0); CHECK_STR(g, "PST"); sentinel(5); }
TC_CASE(size1_erange) { run(1, 0); CHECK_EQ_I(gr, ERANGE); sentinel(1); }
TC_CASE(idx1_boundary)
{ run(3, 1); CHECK_EQ_I(gr, ERANGE); CHECK_EQ_U(gn, 4); sentinel(3);
  run(4, 1); CHECK_EQ_I(gr, 0); CHECK_STR(g, "PDT"); CHECK_EQ_U(gn, 4); sentinel(4); }

int main(void)
{
    tc_begin(); tc_iph_install();
    _putenv("TZ=PST8PDT"); _tzset();
    TC_RUN(size3_erange); TC_RUN(size4_ok); TC_RUN(size5_ok_tail_untouched); TC_RUN(size1_erange); TC_RUN(idx1_boundary);
    _putenv("TZ="); _tzset();
    return tc_end();
}
