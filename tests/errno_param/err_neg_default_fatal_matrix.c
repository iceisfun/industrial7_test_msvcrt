/* TC
 * id:    err_neg_default_fatal_matrix
 * area:  errno_param
 * kind:  negative
 * funcs: strcpy_s memcpy_s wcscpy_s fclose _close
 * crt:   >=140
 * tier:  p0
 * conf:  documented
 * ref:   MS docs "Parameter validation": without a handler an invalid parameter terminates the process
 * note:  Each case is its own child process. Only "terminated abnormally" (code != 0, code != 77) is asserted; the exact status is probed in err_neg_default_handler_fatal.
 */
#include "tc.h"
#include <string.h>
#include <wchar.h>
#include <stdio.h>
#include <io.h>

static void run(const char *name)
{
    DWORD code = 0; int ran = tc_spawn_self(name, &code, 20000);
    CHECK(ran);
    CHECK(code != 0);
    CHECK(code != 77);   /* 77 = child survived the call */
}
TC_CASE(strcpy_s_null_dest) { run("strcpy_s"); }
TC_CASE(memcpy_s_null_dest) { run("memcpy_s"); }
TC_CASE(wcscpy_s_size0)     { run("wcscpy_s"); }
TC_CASE(fclose_null)        { run("fclose"); }
TC_CASE(close_minus1)       { run("close"); }

static int child_main(const char *c)
{
    char *volatile np = NULL; volatile int z = 0; (void)z;
    tc_iph_default();
    if (!strcmp(c, "strcpy_s")) strcpy_s(np, 10, "abc");
    else if (!strcmp(c, "memcpy_s")) memcpy_s(np, 10, "abc", 3);
    else if (!strcmp(c, "wcscpy_s")) { wchar_t d[4]; wcscpy_s(d, (size_t)z, L"a"); }
    else if (!strcmp(c, "fclose")) fclose((FILE *volatile)np);
    else if (!strcmp(c, "close")) _close(-1 + z);
    return 77;
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) return child_main(c);
    TC_RUN(strcpy_s_null_dest); TC_RUN(memcpy_s_null_dest); TC_RUN(wcscpy_s_size0); TC_RUN(fclose_null); TC_RUN(close_minus1);
    return tc_end();
}
