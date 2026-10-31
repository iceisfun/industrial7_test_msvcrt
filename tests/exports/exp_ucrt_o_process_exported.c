/* TC
 * id:    exp_ucrt_o_process_exported
 * area:  exports
 * funcs: 
 * procs: _o_exit _o_abort _o__exit _o__cexit _o_system _o__wsystem _o_getenv _o__putenv _o__getcwd _o__chdir _o__mkdir
 * crt:   ucrtbase
 * tier:  p1
 * conf:  observed
 * ref:   ucrtbase export table
 * note:  Undocumented _o_ aliases. Resolve only; never call exit/abort/system.
 */
#include "tc.h"

static void check_group(const char *const *names, int n)
{
    int i;
    for (i = 0; i < n; i++)
        if (!tc_crt_proc(names[i])) { tc_fail_expr(__FILE__, __LINE__, names[i]); }
}

static const char *const g_exit_group[] = { "_o_exit", "_o_abort", "_o__exit", "_o__cexit", "_o_system", "_o__wsystem" };
TC_CASE(exit_group) { check_group(g_exit_group, (int)(sizeof g_exit_group / sizeof g_exit_group[0])); }

static const char *const g_env_group[] = { "_o_getenv", "_o__putenv" };
TC_CASE(env_group) { check_group(g_env_group, (int)(sizeof g_env_group / sizeof g_env_group[0])); }

static const char *const g_dir_group[] = { "_o__getcwd", "_o__chdir", "_o__mkdir" };
TC_CASE(dir_group) { check_group(g_dir_group, (int)(sizeof g_dir_group / sizeof g_dir_group[0])); }

int main(void)
{
    tc_begin();
    TC_RUN(exit_group);
    TC_RUN(env_group);
    TC_RUN(dir_group);
    return tc_end();
}
