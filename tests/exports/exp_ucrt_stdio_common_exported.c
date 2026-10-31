/* TC
 * id:    exp_ucrt_stdio_common_exported
 * area:  exports
 * funcs: __stdio_common_vfprintf __stdio_common_vsprintf __stdio_common_vfscanf __stdio_common_vsscanf
 * procs: 
 * crt:   ucrtbase
 * tier:  p0
 * kind:  positive
 * conf:  observed
 * ref:   stdio.h inline printf/scanf implementations in the UCRT headers
 * note:  Workhorses are imported by every UCRT-built exe; exercised here through the inline wrappers. Remaining __stdio_common_* names: exp_ucrt_stdio_common_more_exported.
 */
#include "tc.h"

#include <stdio.h>

#define FN "tc_exp_ucrt_stdio_common_exported.tmp"

TC_CASE(vsprintf_works) { char b[32]; int n = sprintf(b, "%d-%s", 12, "ab"); CHECK_EQ_I(n, 5); CHECK_STR(b, "12-ab"); }
TC_CASE(vsscanf_works) { int a = 0, n; volatile const char *s = "77 x"; n = sscanf((const char *)s, "%d", &a); CHECK_EQ_I(n, 1); CHECK_EQ_I(a, 77); }
TC_CASE(vfprintf_vfscanf_roundtrip)
{
    FILE *f = fopen(FN, "w+");
    int a = 0, n;
    CHECK(f != NULL);
    if (!f) return;
    n = fprintf(f, "%d\n", 4242); CHECK_EQ_I(n, 5);
    rewind(f);
    n = fscanf(f, "%d", &a); CHECK_EQ_I(n, 1); CHECK_EQ_I(a, 4242);
    fclose(f); remove(FN);
}
int main(void)
{
    tc_begin();
    TC_RUN(vsprintf_works); TC_RUN(vsscanf_works); TC_RUN(vfprintf_vfscanf_roundtrip);
    return tc_end();
}
