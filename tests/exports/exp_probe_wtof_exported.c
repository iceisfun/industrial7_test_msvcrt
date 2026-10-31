/* TC
 * id:    exp_probe_wtof_exported
 * area:  exports
 * funcs: 
 * procs: _wtof
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _wtof
 * note:  Probing: whether _wtof is exported by every CRT including old msvcrt.dll (docs list msvcrt.dll in Requirements, but it may only exist from a later Windows version).
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


TC_CASE(exported)
{
    HAS("_wtof");
}

int main(void)
{
    tc_begin();
    TC_RUN(exported);
    return tc_end();
}
