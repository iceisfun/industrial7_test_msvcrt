/* TC
 * id:    ucr_get_stream_buffer_pointers_probe
 * area:  ucrt_only
 * funcs: fopen fclose remove
 * procs: _get_stream_buffer_pointers
 * crt:   win10
 * tier:  p2
 * conf:  probe
 * ref:   ucrtbase export (undocumented): errno_t _get_stream_buffer_pointers(FILE*, char***, char***, int**)
 * note:  PROBE: returns 0 for a valid stream with non-NULL out pointers; NULL out-pointers permitted. (NULL-stream behaviour is in ucr_neg_get_stream_buffer_pointers_null_probe.)
 */
#include "tc.h"
#include <stdio.h>

typedef int (__cdecl *gsbp_t)(FILE *, char ***, char ***, int **);
static const char *fn = "tc_ucr_gsbp.tmp";

TC_CASE(valid_stream)
{
    gsbp_t f = (gsbp_t)tc_crt_proc("_get_stream_buffer_pointers");
    FILE *fp; char **b = NULL, **p = NULL; int *c = NULL;
    if (!f) TC_SKIP("not exported");
    fp = fopen(fn, "w+");
    CHECK(fp != NULL); if (!fp) return;
    CHECK_EQ_I(f(fp, &b, &p, &c), 0);
    CHECK(b != NULL); CHECK(p != NULL); CHECK(c != NULL);
    fclose(fp); remove(fn);
}
TC_CASE(null_outs)
{
    gsbp_t f = (gsbp_t)tc_crt_proc("_get_stream_buffer_pointers");
    FILE *fp;
    if (!f) TC_SKIP("not exported");
    fp = fopen(fn, "w+");
    CHECK(fp != NULL); if (!fp) return;
    CHECK_EQ_I(f(fp, NULL, NULL, NULL), 0);
    fclose(fp); remove(fn);
}
int main(void) { tc_begin(); TC_RUN(valid_stream); TC_RUN(null_outs); return tc_end(); }
