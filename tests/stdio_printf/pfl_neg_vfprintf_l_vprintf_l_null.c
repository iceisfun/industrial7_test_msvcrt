/* TC
 * id:    pfl_neg_vfprintf_l_vprintf_l_null
 * area:  stdio_printf
 * kind:  negative
 * funcs: _vfprintf_l _vprintf_l _create_locale _free_locale fopen fclose
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs vfprintf, _vfprintf_l, vprintf, _vprintf_l ("Parameter validation": NULL stream or format -> EINVAL, -1)
 * note:  Temp file tc_pfl_neg_vfprintf_l_vprintf_l_null.tmp.
 */
#include "tc.h"
#include "pfl_shim.h"
#include <errno.h>

static _locale_t loc;
static FILE * volatile v_nofile;
static const char * volatile v_nof;
static const char *fname = "tc_pfl_neg_vfprintf_l_vprintf_l_null.tmp";

static int vf(FILE *f, const char *fmt, ...)
{ va_list ap; int r; va_start(ap, fmt); r = pl_vfprintf_l(f, fmt, loc, ap); va_end(ap); return r; }
static int vp(const char *fmt, ...)
{ va_list ap; int r; va_start(ap, fmt); r = pl_vprintf_l(fmt, loc, ap); va_end(ap); return r; }

TC_CASE(vfprintf_l_null_stream)
{
    int r; tc_iph_count = 0; errno = 0;
    r = vf(v_nofile, "x");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(vfprintf_l_null_format)
{
    int r; tc_iph_count = 0; errno = 0;
    r = vf(stdout, v_nof, 1);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(vprintf_l_null_format)
{
    int r; tc_iph_count = 0; errno = 0;
    r = vp(v_nof, 1);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(valid_vfprintf_l_after)
{
    FILE *f; int r;
    f = fopen(fname, "w"); CHECK(f != NULL); if (!f) return;
    tc_iph_count = 0;
    r = vf(f, "%d:%s", 12, "ab");
    CHECK_EQ_I(r, 5); CHECK_EQ_I(tc_iph_count, 0);
    fclose(f); remove(fname);
}

int main(void)
{
    tc_begin(); tc_iph_install();
    loc = _create_locale(LC_ALL, "C");
    if (!loc) { tc_write("TC FAIL " TC_TEST_ID "::main _create_locale(C) failed\n"); return 1; }
    TC_RUN(vfprintf_l_null_stream); TC_RUN(vfprintf_l_null_format); TC_RUN(vprintf_l_null_format); TC_RUN(valid_vfprintf_l_after);
    _free_locale(loc);
    remove(fname);
    return tc_end();
}
