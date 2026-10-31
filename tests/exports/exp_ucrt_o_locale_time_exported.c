/* TC
 * id:    exp_ucrt_o_locale_time_exported
 * area:  exports
 * funcs: 
 * procs: _o_setlocale _o__create_locale _o__free_locale _o_localeconv _o__configthreadlocale _o_asctime_s _o_strftime _o_wcsftime
 * crt:   ucrtbase
 * tier:  p1
 * conf:  observed
 * ref:   ucrtbase export table
 * note:  Undocumented _o_ aliases.
 */
#include "tc.h"

static void check_group(const char *const *names, int n)
{
    int i;
    for (i = 0; i < n; i++)
        if (!tc_crt_proc(names[i])) { tc_fail_expr(__FILE__, __LINE__, names[i]); }
}

static const char *const g_locale_group[] = { "_o_setlocale", "_o__create_locale", "_o__free_locale", "_o_localeconv", "_o__configthreadlocale" };
TC_CASE(locale_group) { check_group(g_locale_group, (int)(sizeof g_locale_group / sizeof g_locale_group[0])); }

static const char *const g_time_group[] = { "_o_asctime_s", "_o_strftime", "_o_wcsftime" };
TC_CASE(time_group) { check_group(g_time_group, (int)(sizeof g_time_group / sizeof g_time_group[0])); }

int main(void)
{
    tc_begin();
    TC_RUN(locale_group);
    TC_RUN(time_group);
    return tc_end();
}
