/* TC
 * id:    dir_neg_splitpath_long_components
 * area:  dir
 * kind:  negative
 * funcs: _splitpath _wsplitpath _splitpath_s
 * crt:   ucrtbase
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _splitpath_s (ERANGE when a component does not fit its buffer)
 * note:  Probes _splitpath_s with components longer than _MAX_FNAME/_MAX_DIR using full-size (_MAX_*) buffers:
 *        expects ERANGE, one invalid-parameter call, and (probe) all output buffers reset to empty strings.
 *        Controls: empty path and ".gitignore" (no fname, ext is the whole name).
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

static const char *vs(const char *s) { static volatile const char *v; v = s; return (const char *)v; }

static void fillall(char *d, char *di, char *f, char *e)
{ d[0] = di[0] = f[0] = e[0] = 'X'; d[1] = di[1] = f[1] = e[1] = 0; }

TC_CASE(long_fname)
{
    char d[_MAX_DRIVE], di[_MAX_DIR], f[_MAX_FNAME], e[_MAX_EXT], p[300]; int r, i;
    p[0] = 'C'; p[1] = ':'; p[2] = '\\'; p[3] = 'd'; p[4] = '\\';
    for (i = 0; i < 260; i++) p[5 + i] = 'n';
    p[265] = '.'; p[266] = 'e'; p[267] = 0;
    fillall(d, di, f, e); tc_iph_count = 0;
    r = _splitpath_s(vs(p), d, _MAX_DRIVE, di, _MAX_DIR, f, _MAX_FNAME, e, _MAX_EXT);
    CHECK_EQ_I(r, ERANGE); CHECK_EQ_I(tc_iph_count, 1);
    CHECK_STR(d, ""); CHECK_STR(di, ""); CHECK_STR(f, ""); CHECK_STR(e, "");
}
TC_CASE(long_dir)
{
    char d[_MAX_DRIVE], di[_MAX_DIR], f[_MAX_FNAME], e[_MAX_EXT], p[320]; int r, i;
    p[0] = '\\';
    for (i = 0; i < 300; i++) p[1 + i] = 'd';
    p[301] = '\\'; p[302] = 'f'; p[303] = '.'; p[304] = 'e'; p[305] = 0;
    fillall(d, di, f, e); tc_iph_count = 0;
    r = _splitpath_s(vs(p), d, _MAX_DRIVE, di, _MAX_DIR, f, _MAX_FNAME, e, _MAX_EXT);
    CHECK_EQ_I(r, ERANGE); CHECK_EQ_I(tc_iph_count, 1);
    CHECK_STR(di, "");
}
TC_CASE(empty_control)
{
    char d[_MAX_DRIVE], di[_MAX_DIR], f[_MAX_FNAME], e[_MAX_EXT]; int r;
    fillall(d, di, f, e); tc_iph_count = 0;
    r = _splitpath_s(vs(""), d, _MAX_DRIVE, di, _MAX_DIR, f, _MAX_FNAME, e, _MAX_EXT);
    CHECK_EQ_I(r, 0); CHECK_EQ_I(tc_iph_count, 0);
    CHECK_STR(d, ""); CHECK_STR(di, ""); CHECK_STR(f, ""); CHECK_STR(e, "");
}
TC_CASE(only_ext_control)
{
    char d[_MAX_DRIVE], di[_MAX_DIR], f[_MAX_FNAME], e[_MAX_EXT]; int r;
    wchar_t wd[_MAX_DRIVE], wdi[_MAX_DIR], wf[_MAX_FNAME], we[_MAX_EXT];
    static volatile const wchar_t *wp = L".gitignore";
    fillall(d, di, f, e); tc_iph_count = 0;
    r = _splitpath_s(vs(".gitignore"), d, _MAX_DRIVE, di, _MAX_DIR, f, _MAX_FNAME, e, _MAX_EXT);
    CHECK_EQ_I(r, 0); CHECK_EQ_I(tc_iph_count, 0);
    CHECK_STR(f, ""); CHECK_STR(e, ".gitignore");
    fillall(d, di, f, e);
    _splitpath(vs(".gitignore"), d, di, f, e);
    CHECK_STR(f, ""); CHECK_STR(e, ".gitignore");
    _wsplitpath((const wchar_t *)wp, wd, wdi, wf, we);
    CHECK_WSTR(wf, L""); CHECK_WSTR(we, L".gitignore");
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph api\n"); return 1; }
    TC_RUN(long_fname); TC_RUN(long_dir); TC_RUN(empty_control); TC_RUN(only_ext_control);
    return tc_end();
}
