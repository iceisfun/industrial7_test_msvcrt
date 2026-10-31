/* TC
 * id:    dat_mb_cur_max_dbcs_probe
 * area:  data_exports
 * funcs: ___mb_cur_max_func setlocale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs setlocale (".code-page" locale string), MB_CUR_MAX
 * note:  Probe: setlocale(LC_ALL, ".932"/".936"/".1252") succeeds (ANSI tables built in) and MB_CUR_MAX is 2/2/1. Case is skipped if setlocale returns NULL.
 */
#include "tc.h"
#include <stdlib.h>
#include <locale.h>

TC_CASE(cp1252_is_1)
{ if (!setlocale(LC_ALL, ".1252")) TC_SKIP("cp1252 unavailable"); CHECK_EQ_I(___mb_cur_max_func(), 1); setlocale(LC_ALL, "C"); }
TC_CASE(cp932_is_2)
{ if (!setlocale(LC_ALL, ".932")) TC_SKIP("cp932 unavailable"); CHECK_EQ_I(___mb_cur_max_func(), 2); setlocale(LC_ALL, "C"); }
TC_CASE(cp936_is_2)
{ if (!setlocale(LC_ALL, ".936")) TC_SKIP("cp936 unavailable"); CHECK_EQ_I(___mb_cur_max_func(), 2); setlocale(LC_ALL, "C"); }
TC_CASE(cp1252_restored_to_1)
{
    if (!setlocale(LC_ALL, ".932")) TC_SKIP("cp932 unavailable");
    CHECK_EQ_I(___mb_cur_max_func(), 2);
    CHECK_STR(setlocale(LC_ALL, "C"), "C");
    CHECK_EQ_I(___mb_cur_max_func(), 1);
}
int main(void) { tc_begin(); TC_RUN(cp1252_is_1); TC_RUN(cp932_is_2); TC_RUN(cp936_is_2); TC_RUN(cp1252_restored_to_1); setlocale(LC_ALL, "C"); return tc_end(); }
