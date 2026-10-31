/* TC
 * id:    exp_recalloc_versions_probe
 * area:  exports
 * funcs: 
 * procs: _recalloc
 * crt:   msvcr90 msvcr100 msvcr110 msvcr120 ucrtbase
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _recalloc (VS2008+)
 * note:  Probing: _recalloc is exported by msvcr90 and later. Whether the grown tail is zeroed is documented behaviour (the point of _recalloc); msvcr80 deliberately excluded.
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
    HAS("_recalloc");
}

TC_CASE(zero_fills_growth)
{
    void *(__cdecl *rc)(void *, size_t, size_t) = (void *(__cdecl *)(void *, size_t, size_t))tc_crt_proc("_recalloc");
    unsigned char *p; int i, bad = 0;
    if (!rc) TC_SKIP("_recalloc not exported");
    p = (unsigned char *)rc(NULL, 4, 8);
    CHECK(p != NULL);
    if (!p) return;
    for (i = 0; i < 32; i++) if (p[i]) bad++;
    CHECK_EQ_I(bad, 0);
    memset(p, 0x77, 32);
    p = (unsigned char *)rc(p, 10, 8);
    CHECK(p != NULL);
    if (!p) return;
    for (bad = 0, i = 32; i < 80; i++) if (p[i]) bad++;
    CHECK_EQ_I(bad, 0);
    CHECK_EQ_U(p[0], 0x77);
    free(p);
}

int main(void)
{
    tc_begin();
    TC_RUN(exported);
    TC_RUN(zero_fills_growth);
    return tc_end();
}
