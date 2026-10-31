/* TC
 * id:    exp_ucrt_o_memory_exported
 * area:  exports
 * funcs: 
 * procs: _o_malloc _o_calloc _o_realloc _o_free _o__msize _o__expand _o__aligned_malloc _o__aligned_free _o__aligned_realloc _o__recalloc
 * crt:   ucrtbase
 * tier:  p0
 * conf:  observed
 * ref:   ucrtbase export table
 * note:  Undocumented _o_ heap aliases.
 */
#include "tc.h"

static void check_group(const char *const *names, int n)
{
    int i;
    for (i = 0; i < n; i++)
        if (!tc_crt_proc(names[i])) { tc_fail_expr(__FILE__, __LINE__, names[i]); }
}

static const char *const g_malloc_family[] = { "_o_malloc", "_o_calloc", "_o_realloc", "_o_free", "_o__msize", "_o__expand" };
TC_CASE(malloc_family) { check_group(g_malloc_family, (int)(sizeof g_malloc_family / sizeof g_malloc_family[0])); }

static const char *const g_aligned_family[] = { "_o__aligned_malloc", "_o__aligned_free", "_o__aligned_realloc", "_o__recalloc" };
TC_CASE(aligned_family) { check_group(g_aligned_family, (int)(sizeof g_aligned_family / sizeof g_aligned_family[0])); }

int main(void)
{
    tc_begin();
    TC_RUN(malloc_family);
    TC_RUN(aligned_family);
    return tc_end();
}
