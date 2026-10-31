/* TC
 * id:    exp_ucrt_o_wide_exported
 * area:  exports
 * funcs: 
 * procs: _o__wfopen _o__wfopen_s _o__wremove
 * crt:   ucrtbase
 * tier:  p2
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

static const char *const g_each_resolves[] = { "_o__wfopen", "_o__wfopen_s", "_o__wremove" };
TC_CASE(each_resolves) { check_group(g_each_resolves, (int)(sizeof g_each_resolves / sizeof g_each_resolves[0])); }

int main(void)
{
    tc_begin();
    TC_RUN(each_resolves);
    return tc_end();
}
