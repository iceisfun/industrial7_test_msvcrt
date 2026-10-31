/* TC
 * id:    mb_probe_restartable_exports
 * area:  mbcs
 * funcs:
 * procs: mbrtowc mbrlen wcrtomb mbsrtowcs wcsrtombs btowc wctob
 * crt:   ucrtbase msvcr120 msvcr110
 * tier:  p1
 * conf:  probe
 * ref:   MS docs mbrtowc etc. (restartable conversion functions)
 * note:  PROBES: whether msvcr110.dll and msvcr120.dll export the C99 restartable conversion functions. ucrtbase certainly does.
 *        The mingw msvcr110/120 import libs list them but only as static libmingwex stubs, so exports are resolved at run time.
 *        Other tests in this area skip these functions if the CRT lacks them.
 */
#include "tc.h"

TC_CASE(exported_all)
{
    CHECK(tc_crt_proc("mbrtowc") != NULL);
    CHECK(tc_crt_proc("mbrlen") != NULL);
    CHECK(tc_crt_proc("wcrtomb") != NULL);
    CHECK(tc_crt_proc("mbsrtowcs") != NULL);
    CHECK(tc_crt_proc("wcsrtombs") != NULL);
    CHECK(tc_crt_proc("btowc") != NULL);
    CHECK(tc_crt_proc("wctob") != NULL);
}
int main(void) { tc_begin(); TC_RUN(exported_all); return tc_end(); }
