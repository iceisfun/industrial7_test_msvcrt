/* TC
 * id:    tm_ftime32_basic
 * area:  time
 * funcs:
 * procs: _ftime32
 * crt:   >=80
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _ftime, _ftime32
 */
#include "tc.h"
#include <time.h>
#include <stdlib.h>
#include <sys/timeb.h>

typedef void (__cdecl *ft32_t)(struct __timeb32 *);
static ft32_t p_ftime32;
static void set_tz(const char *s) { _putenv(s); _tzset(); }

TC_CASE(missing) { CHECK(p_ftime32 != NULL); }
TC_CASE(range)
{
    struct __timeb32 tb; set_tz("TZ=UTC0"); p_ftime32(&tb);
    CHECK(tb.time > 1500000000L); CHECK(tb.time < 0x7fffffffL);
    CHECK(tb.millitm < 1000);
}
int main(void) { tc_begin(); p_ftime32 = (ft32_t)tc_crt_proc("_ftime32"); if (!p_ftime32) { TC_RUN(missing); return tc_end(); } TC_RUN(range); return tc_end(); }
