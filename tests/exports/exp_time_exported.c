/* TC
 * id:    exp_time_exported
 * area:  exports
 * funcs: 
 * procs: time clock gmtime localtime asctime strftime wcsftime _mktime64 _time64 _localtime64 _gmtime64 _difftime64 _tzset _strdate _strtime _ftime64 _utime64
 * crt:   all
 * tier:  p0
 * conf:  observed
 * ref:   MS docs time functions (64-bit variants VS2005+, and msvcrt.dll XP+)
 * note:  difftime, mktime and ctime are covered per name by exp_probe_time_inline_exports (they are inline wrappers in some CRT headers).
 *        Fix: ucrtbase does not export time/gmtime/localtime (observed: GetProcAddress NULL); the skip was keyed on TC_UCRT, which the build never defines (it defines _UCRT).
 */
#include "tc.h"

static void need_all(const char *const *names, int n)
{
    int i;
    for (i = 0; i < n; i++)
        if (!tc_crt_proc(names[i])) tc_fail_expr(__FILE__, __LINE__, names[i]);
}
#define NEEDS(arr) need_all(arr, (int)(sizeof(arr) / sizeof((arr)[0])))

TC_CASE(iso_c_group)
{
    static const char *const names[] = { "clock", "asctime", "strftime", "wcsftime" };
    NEEDS(names);
}

TC_CASE(old_iso_c_group)
{
#if defined(TC_UCRT) || defined(_UCRT)   /* tc.h documents TC_UCRT but the build only defines _UCRT */
    TC_SKIP("ucrtbase exports only the _time32/_time64 families; time/gmtime/localtime are inline in its headers");
#else
    static const char *const names[] = { "time", "gmtime", "localtime" };
    NEEDS(names);
#endif
}

TC_CASE(time64_group)
{
    static const char *const names[] = { "_mktime64", "_time64", "_localtime64", "_gmtime64", "_difftime64" };
    NEEDS(names);
}

TC_CASE(legacy_group)
{
    static const char *const names[] = { "_tzset", "_strdate", "_strtime", "_ftime64", "_utime64" };
    NEEDS(names);
}

int main(void)
{
    tc_begin();
    TC_RUN(iso_c_group);
    TC_RUN(old_iso_c_group);
    TC_RUN(time64_group);
    TC_RUN(legacy_group);
    return tc_end();
}
