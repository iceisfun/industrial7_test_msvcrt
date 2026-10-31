/* TC
 * id:    err_neg_default_fatal_win7
 * area:  errno_param
 * kind:  negative
 * funcs: fclose
 * procs: strcpy_s
 * crt:   msvcr90 msvcr100 msvcr110 msvcr120
 * tier:  p1
 * conf:  documented
 * ref:   MS docs "Parameter validation"; _set_invalid_parameter_handler
 * note:  Win7-era CRTs: default invalid-parameter handling terminates the process. Exact status deliberately not asserted (see err_neg_default_fatal_status_win7_probe). strcpy_s resolved at run time (mingw libmsvcr90.a lacks it).
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

TC_DECL_PROC(int, strcpy_s, (char *, size_t, const char *));

static void run(const char *name)
{
    DWORD code = 0; int ran = tc_spawn_self(name, &code, 20000);
    CHECK(ran);
    CHECK(code != 0);
    CHECK(code != 77);
}
TC_CASE(strcpy_s_null) { run("strcpy_s"); }
TC_CASE(fclose_null)   { run("fclose"); }

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        FILE *volatile fp = NULL; char *volatile np = NULL;
        TC_LOAD_PROC(strcpy_s);
        if (!strcmp(c, "strcpy_s") && tc_p_strcpy_s) tc_p_strcpy_s(np, 10, "abc");
        else if (!strcmp(c, "fclose")) fclose(fp);
        return 77;
    }
    TC_RUN(strcpy_s_null); TC_RUN(fclose_null);
    return tc_end();
}
