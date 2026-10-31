/* TC
 * id:    ucr_o_alias_presence
 * area:  ucrt_only
 * funcs:
 * procs: _o_malloc _o_free _o_calloc _o_realloc _o_exit _o_abort _o_terminate _o_fopen _o_fclose _o_fread _o_fwrite _o_getenv _o_setlocale _o_qsort _o_bsearch _o_rand _o_srand _o_system _o_tolower _o_toupper _o_atoi _o_strtod _o_sin _o__itoa_s _o_strcpy_s _o_strcat_s _o_memcpy_s _o_wcscpy_s _o__strdup _o__stricmp _o__wfopen _o__close _o__fileno _o__lseeki64
 * crt:   win10
 * tier:  p1
 * conf:  observed
 * ref:   ucrtbase.dll export table (_o_ forwarding aliases used by the Windows-internal "optimized" import set)
 */
#include "tc.h"

static const char *const names[] = {
    "_o_malloc",
    "_o_free",
    "_o_calloc",
    "_o_realloc",
    "_o_exit",
    "_o_abort",
    "_o_terminate",
    "_o_fopen",
    "_o_fclose",
    "_o_fread",
    "_o_fwrite",
    "_o_getenv",
    "_o_setlocale",
    "_o_qsort",
    "_o_bsearch",
    "_o_rand",
    "_o_srand",
    "_o_system",
    "_o_tolower",
    "_o_toupper",
    "_o_atoi",
    "_o_strtod",
    "_o_sin",
    "_o__itoa_s",
    "_o_strcpy_s",
    "_o_strcat_s",
    "_o_memcpy_s",
    "_o_wcscpy_s",
    "_o__strdup",
    "_o__stricmp",
    "_o__wfopen",
    "_o__close",
    "_o__fileno",
    "_o__lseeki64"
};

TC_CASE(all_exported)
{
    size_t i;
    for (i = 0; i < sizeof names / sizeof names[0]; i++) {
        FARPROC p = tc_crt_proc(names[i]);
        if (!p) { tc_failhdr(__FILE__, __LINE__); tc_write("missing export "); tc_write(names[i]); tc_write("\n"); }
    }
}
int main(void) { tc_begin(); TC_RUN(all_exported); return tc_end(); }
