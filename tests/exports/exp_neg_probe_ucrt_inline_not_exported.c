/* TC
 * id:    exp_neg_probe_ucrt_inline_not_exported
 * area:  exports
 * kind:  negative
 * funcs: 
 * procs: malloc
 * crt:   ucrtbase
 * tier:  p1
 * conf:  probe
 * ref:   MS docs UCRT breaking changes (printf/scanf families became inline wrappers over __stdio_common_*)
 * note:  Probing: ucrtbase.dll does not export printf/sprintf/scanf-style names (they are header inlines over __stdio_common_*). The *_absent cases assert absence; present_core asserts the replacements exist.
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


TC_CASE(printf_family_absent)
{
    HASNOT("printf");
    HASNOT("fprintf");
    HASNOT("sprintf");
    HASNOT("scanf");
    HASNOT("sscanf");
    HASNOT("fscanf");
}

TC_CASE(v_family_absent)
{
    HASNOT("vprintf");
    HASNOT("vfprintf");
    HASNOT("vsprintf");
}

TC_CASE(present_core)
{
    HAS("__stdio_common_vfprintf");
    HAS("__stdio_common_vsprintf");
    HAS("__stdio_common_vfscanf");
}

int main(void)
{
    tc_begin();
    TC_RUN(printf_family_absent);
    TC_RUN(v_family_absent);
    TC_RUN(present_core);
    return tc_end();
}
