/* TC
 * id:    tm_time_export_probe
 * area:  time
 * funcs:
 * procs: time _time32 _gmtime32 _localtime32 _mktime32
 * crt:   >=80
 * tier:  p1
 * conf:  probe
 * ref:   mingw-w64 import libraries list these names for the CRT
 * note:  PROBE: whether the plain/32-bit names are real DLL exports. Believed exported: time, _time32, _gmtime32, _localtime32,
 *        _mktime32 (mingw import libs list them for ucrtbase and msvcr80+). Wine's ucrtbase/msvcr* does not export plain `time`.
 */
#include "tc.h"

TC_CASE(time_exported) { CHECK(tc_crt_proc("time") != NULL); }
TC_CASE(time32_exported) { CHECK(tc_crt_proc("_time32") != NULL); }
TC_CASE(gmtime32_exported) { CHECK(tc_crt_proc("_gmtime32") != NULL); }
TC_CASE(localtime32_exported) { CHECK(tc_crt_proc("_localtime32") != NULL); }
TC_CASE(mktime32_exported) { CHECK(tc_crt_proc("_mktime32") != NULL); }
int main(void) { tc_begin(); TC_RUN(time_exported); TC_RUN(time32_exported); TC_RUN(gmtime32_exported); TC_RUN(localtime32_exported); TC_RUN(mktime32_exported); return tc_end(); }
