/* TC
 * id:    exp_neg_bogus_names
 * area:  exports
 * kind:  negative
 * funcs: 
 * procs: malloc
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs GetProcAddress (lookup is exact and case-sensitive; returns NULL on failure)
 * note:  GetProcAddress on a CRT with names that are not exports must return NULL. Windows API names must not be re-exported by the CRT.
 */
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <wctype.h>
#include <ctype.h>
#include <math.h>
#include <float.h>
#include <malloc.h>
#include <stdio.h>
#include <errno.h>
#include <stdint.h>
#define HAS(n) do { if (!tc_crt_proc(n)) tc_fail_expr(__FILE__, __LINE__, "not exported: " n); } while (0)
#define HASNOT(n) do { if (tc_crt_proc(n)) tc_fail_expr(__FILE__, __LINE__, "unexpectedly exported: " n); } while (0)


TC_CASE(nonexistent)
{
    HASNOT("tc_no_such_function");
    HASNOT("strlen_s_not_real");
    HASNOT("malloc2");
    HASNOT("_malloc_x");
}

TC_CASE(case_sensitive)
{
    HASNOT("MALLOC");
    HASNOT("Malloc");
    HASNOT("STRLEN");
    HASNOT("Strlen");
    HASNOT("SIN");
}

TC_CASE(decorated_or_cpp)
{
    HASNOT("strlen@4");
    HASNOT("_strlen");
    HASNOT("?strlen@@YAIPEBD@Z");
    HASNOT("@strlen@4");
}

TC_CASE(windows_api_not_crt)
{
    HASNOT("CreateFileA");
    HASNOT("GetLastError");
    HASNOT("ExitProcess");
}

int main(void)
{
    tc_begin();
    TC_RUN(nonexistent);
    TC_RUN(case_sensitive);
    TC_RUN(decorated_or_cpp);
    TC_RUN(windows_api_not_crt);
    return tc_end();
}
