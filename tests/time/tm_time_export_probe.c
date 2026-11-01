/* TC
 * id:    tm_time_export_probe
 * area:  time
 * funcs:
 * procs: _time64 _time32 _gmtime32 _localtime32 _mktime32
 * crt:   >=80
 * tier:  p1
 * conf:  probe
 * ref:   mingw-w64 import libraries list these names for the CRT
 * note:  PROBE: whether the plain/32-bit names are real DLL exports. Believed exported: time, _time32, _gmtime32, _localtime32,
 *        _mktime32 (mingw import libs list them for ucrtbase and msvcr80+). Wine's ucrtbase/msvcr* does not export plain `time`.
 *        Win10 19045 ucrtbase export table (GetProcAddress): _time32 _time64 _gmtime32 _localtime32 _mktime32 present; plain "time" absent, so case time_exported probes _time64.
 */
#include "tc.h"

/* Win10 ucrtbase does not export plain "time" (header inline over _time64); the real export is _time64 */
TC_CASE(time_exported) { CHECK(tc_crt_proc("_time64") != NULL); }
TC_CASE(plain_time_not_exported) {
#if TC_CRT_VER >= 140
    CHECK(tc_crt_proc("time") == NULL);   /* observed: 'export time NO' on Win10 19045 ucrtbase */
#else
    TC_SKIP("ucrtbase-only observation");
#endif
}
TC_CASE(time32_exported) { CHECK(tc_crt_proc("_time32") != NULL); }
TC_CASE(gmtime32_exported) { CHECK(tc_crt_proc("_gmtime32") != NULL); }
TC_CASE(localtime32_exported) { CHECK(tc_crt_proc("_localtime32") != NULL); }
TC_CASE(mktime32_exported) { CHECK(tc_crt_proc("_mktime32") != NULL); }
int main(void) { tc_begin(); TC_RUN(time_exported); TC_RUN(plain_time_not_exported); TC_RUN(time32_exported); TC_RUN(gmtime32_exported); TC_RUN(localtime32_exported); TC_RUN(mktime32_exported); return tc_end(); }
