/* TC
 * id:    exp_neg_probe_lasterror
 * area:  exports
 * kind:  negative
 * funcs: 
 * procs: malloc
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs GetProcAddress, system error codes (127)
 * note:  Probing: GetLastError() == ERROR_PROC_NOT_FOUND (127) after GetProcAddress fails on a loaded module (docs only say 'call GetLastError').
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


TC_CASE(proc_not_found)
{
    HMODULE m = GetModuleHandleA(TC_CRT_DLL);
    FARPROC p;
    CHECK(m != NULL);
    SetLastError(0);
    p = GetProcAddress(m, "tc_no_such_function");
    CHECK(p == NULL);
    CHECK_EQ_U(GetLastError(), ERROR_PROC_NOT_FOUND);
}

int main(void)
{
    tc_begin();
    TC_RUN(proc_not_found);
    return tc_end();
}
