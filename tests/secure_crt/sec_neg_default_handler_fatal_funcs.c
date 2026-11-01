/* TC
 * id:    sec_neg_default_handler_fatal_funcs
 * area:  secure_crt
 * funcs: strcpy_s strcat_s memcpy_s _itoa_s qsort_s fopen_s _splitpath_s getenv_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs Parameter validation / set_invalid_parameter_handler: default handler terminates the process
 * note:  Termination only; exact status asserted in sec_neg_default_handler_status_probe.
 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
static volatile size_t v0 = 0, v8 = 8;
static volatile int v99 = 99;
static int cmp(void *c, const void *a, const void *b) { (void)c; (void)a; (void)b; return 0; }
static int child_main(const char *n)
{
    char d[8] = "ab", e[8]; int arr[4] = {3, 1, 2, 0}; size_t rs; FILE *f;
    tc_iph_default();
    if (tc_strequ(n, "strcpy_s")) strcpy_s(d, v0, "x");
    else if (tc_strequ(n, "strcat_s")) strcat_s(NULL, v8, "x");
    else if (tc_strequ(n, "memcpy_s")) memcpy_s(d, v8, "0123456789abcdef", 16);
    else if (tc_strequ(n, "itoa_s")) _itoa_s(5, e, v8, v99);
    else if (tc_strequ(n, "qsort_s")) qsort_s(arr, 4, sizeof arr[0], NULL, NULL);
    else if (tc_strequ(n, "fopen_s")) fopen_s(NULL, "tc_sec_never.tmp", "r");
    else if (tc_strequ(n, "splitpath_s")) _splitpath_s(NULL, NULL, 0, NULL, 0, NULL, 0, NULL, 0);
    else if (tc_strequ(n, "getenv_s")) getenv_s(NULL, e, v8, "PATH");
    (void)cmp; (void)rs; (void)f;
    return 77;
}
static void term(const char *n)
{
    DWORD code = 77; int ran = tc_spawn_self(n, &code, 20000);
    CHECK(ran); CHECK(code != 0); CHECK(code != 77);
}
TC_CASE(child_strcpy_s_size0_terminates) { term("strcpy_s"); }
TC_CASE(child_strcat_s_null_dest_terminates) { term("strcat_s"); }
TC_CASE(child_memcpy_s_range_terminates) { term("memcpy_s"); }
TC_CASE(child_itoa_s_radix_99_terminates) { term("itoa_s"); }
TC_CASE(child_qsort_s_null_compare_terminates) { term("qsort_s"); }
TC_CASE(child_fopen_s_null_pfile_terminates) { term("fopen_s"); }
TC_CASE(child_splitpath_s_null_path_terminates) { term("splitpath_s"); }
TC_CASE(child_getenv_s_null_retsize_terminates) { term("getenv_s"); }
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) return child_main(c);
    TC_RUN(child_strcpy_s_size0_terminates); TC_RUN(child_strcat_s_null_dest_terminates);
    TC_RUN(child_memcpy_s_range_terminates); TC_RUN(child_itoa_s_radix_99_terminates);
    TC_RUN(child_qsort_s_null_compare_terminates); TC_RUN(child_fopen_s_null_pfile_terminates);
    TC_RUN(child_splitpath_s_null_path_terminates); TC_RUN(child_getenv_s_null_retsize_terminates);
    return tc_end();
}
