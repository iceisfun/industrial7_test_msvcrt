/* TC
 * id:    dat_sys_nerr_data_win7
 * area:  data_exports
 * funcs:
 * procs: _sys_nerr _sys_errlist
 * crt:   msvcr120 msvcr110 msvcr100 msvcr80
 * tier:  p1
 * conf:  observed
 * ref:   MS docs _sys_errlist, _sys_nerr
 * note:  Variables are documented; the count (43) and message texts are observed behaviour, not documented.
 *        Data exports resolved with GetProcAddress (address of the variable / of the pointer array).
 */
#include "tc.h"
#include <errno.h>

static int *p_nerr; static char **p_list;

TC_CASE(exported) { CHECK(p_nerr != NULL); CHECK(p_list != NULL); }
TC_CASE(nerr_ge_43) { if (!p_nerr) TC_SKIP("no _sys_nerr"); CHECK(*p_nerr >= 43); }
TC_CASE(errlist_enoent_text) { if (!p_list) TC_SKIP("no _sys_errlist"); CHECK_STR(p_list[ENOENT], "No such file or directory"); }
TC_CASE(errlist_eacces_text) { if (!p_list) TC_SKIP("no _sys_errlist"); CHECK_STR(p_list[EACCES], "Permission denied"); }

int main(void)
{
    tc_begin();
    p_nerr = (int *)tc_crt_proc("_sys_nerr"); p_list = (char **)tc_crt_proc("_sys_errlist");
    TC_RUN(exported); TC_RUN(nerr_ge_43); TC_RUN(errlist_enoent_text); TC_RUN(errlist_eacces_text);
    return tc_end();
}
