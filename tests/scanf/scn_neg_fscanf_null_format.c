/* TC
 * id:    scn_neg_fscanf_null_format
 * area:  scanf
 * funcs: fscanf fwscanf fopen fclose
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs fscanf, fwscanf: NULL format invokes the invalid parameter handler, returns EOF, errno EINVAL
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

static const char *volatile vfmt;
static const wchar_t *volatile vwfmt;
#define TMP "tc_scn_neg_fscanf_null_format.tmp"

static FILE *mk(void)
{
    FILE *f = fopen(TMP, "wb");
    if (!f) return NULL;
    fputs("42 99\n", f); fclose(f);
    return fopen(TMP, "r");
}
TC_CASE(fscanf_null_fmt)
{
    FILE *f = mk(); int a = 0, r; vfmt = NULL;
    if (!f) TC_SKIP("cannot create temp file");
    CHECK(tc_iph_install()); tc_iph_count = 0; errno = 0;
    r = fscanf(f, vfmt);
    CHECK_EQ_I(r, EOF); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(ftell(f), 0);
    r = fscanf(f, "%d", &a);   /* stream still usable and unconsumed */
    CHECK_EQ_I(r, 1); CHECK_EQ_I(a, 42);
    fclose(f); remove(TMP);
}
TC_CASE(fwscanf_null_fmt)
{
    FILE *f = mk(); int a = 0, r; vwfmt = NULL;
    if (!f) TC_SKIP("cannot create temp file");
    CHECK(tc_iph_install()); tc_iph_count = 0; errno = 0;
    r = fwscanf(f, vwfmt);
    CHECK_EQ_I(r, EOF); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(ftell(f), 0);
    r = fscanf(f, "%d", &a);
    CHECK_EQ_I(r, 1); CHECK_EQ_I(a, 42);
    fclose(f); remove(TMP);
}
int main(void) { tc_begin(); TC_RUN(fscanf_null_fmt); TC_RUN(fwscanf_null_fmt); return tc_end(); }
