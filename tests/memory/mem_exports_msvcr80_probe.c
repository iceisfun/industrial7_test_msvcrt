/* TC
 * id:    mem_exports_msvcr80_probe
 * area:  memory
 * funcs: memcpy
 * procs: memcpy_s memmove_s _memicmp_l wmemcpy_s wmemmove_s memccpy _memccpy
 * crt:   msvcr80
 * tier:  p2
 * conf:  probe
 * ref:   MS docs memcpy_s, memmove_s, _memicmp_l, _memccpy (VS2005+ secure CRT)
 * note:  Probes which mem* secure/extra exports msvcr80.dll has. One case per name so a wrong guess is isolated.
 *        wmemcpy_s/wmemmove_s are believed to be inline-only in headers before VS2010 (not exported): asserted absent (guess).
 *        _memccpy (underscored, documented) is asserted present; unprefixed memccpy (POSIX name) is asserted absent.
 */
#include "tc.h"

static FARPROC P(const char *n) { return tc_crt_proc(n); }
TC_CASE(memcpy_s_present)   { CHECK(P("memcpy_s") != NULL); }
TC_CASE(memmove_s_present)  { CHECK(P("memmove_s") != NULL); }
TC_CASE(memicmp_l_present)  { CHECK(P("_memicmp_l") != NULL); }
TC_CASE(wmemcpy_s_absent)   { CHECK(P("wmemcpy_s") == NULL); }
TC_CASE(wmemmove_s_absent)  { CHECK(P("wmemmove_s") == NULL); }
TC_CASE(underscore_memccpy_present) { CHECK(P("_memccpy") != NULL); }
TC_CASE(memccpy_absent_recorded) { CHECK(P("memccpy") == NULL); }

int main(void)
{
    tc_begin();
    TC_RUN(memcpy_s_present); TC_RUN(memmove_s_present); TC_RUN(memicmp_l_present);
    TC_RUN(wmemcpy_s_absent); TC_RUN(wmemmove_s_absent); TC_RUN(underscore_memccpy_present); TC_RUN(memccpy_absent_recorded);
    return tc_end();
}
