/* TC
 * id:    dir_neg_splitpath_s_small
 * area:  dir
 * kind:  negative
 * funcs:
 * procs: _splitpath_s _wsplitpath_s _set_invalid_parameter_handler
 * crt:   >=80
 * tier:  p0
 * conf:  observed
 * ref:   MS docs _splitpath_s (error conditions table: buffer too small -> ERANGE, invalid parameter handler invoked)
 * note:  MS docs _splitpath_s error table says ERANGE with the invalid parameter handler; OBSERVED on Win10 22H2 ucrtbase:
 *        every too-small-buffer case returns ERANGE but the handler is NOT invoked, so the handler count assertion is now 0.
 */
#include "tc.h"
#include <errno.h>

typedef int (__cdecl *sp_t)(const char *, char *, size_t, char *, size_t, char *, size_t, char *, size_t);
typedef int (__cdecl *wsp_t)(const wchar_t *, wchar_t *, size_t, wchar_t *, size_t, wchar_t *, size_t, wchar_t *, size_t);
static sp_t sp; static wsp_t wsp;
static const char *vs(const char *s) { static volatile const char *v; v = s; return (const char *)v; }

static void fill(char *d, char *di, char *f, char *e)
{ d[0] = 'X'; d[1] = 0; di[0] = 'X'; di[1] = 0; f[0] = 'X'; f[1] = 0; e[0] = 'X'; e[1] = 0; }

/* run with given sizes; assert ERANGE and iph==0 (observed on Win10 22H2: ERANGE without a handler call, unlike the MS docs table) (buffer state is probed in dir_neg_probe_splitpath_s_reset) */
static void run(const char *path, size_t ds, size_t dis, size_t fs, size_t es)
{
    char d[_MAX_DRIVE], di[_MAX_DIR], f[_MAX_FNAME], e[_MAX_EXT];
    int r;
    fill(d, di, f, e);
    tc_iph_count = 0; errno = 0;
    r = sp(vs(path), d, ds, di, dis, f, fs, e, es);
    CHECK_EQ_I(r, ERANGE);
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(fname_small) { if (!sp) TC_SKIP("n/a"); run("abcdef", _MAX_DRIVE, _MAX_DIR, 3, _MAX_EXT); }
TC_CASE(drive_small) { if (!sp) TC_SKIP("n/a"); run("C:\\d\\f.e", 2, _MAX_DIR, _MAX_FNAME, _MAX_EXT); }
TC_CASE(dir_small)   { if (!sp) TC_SKIP("n/a"); run("C:\\dir\\sub\\f.e", _MAX_DRIVE, 5, _MAX_FNAME, _MAX_EXT); }
TC_CASE(ext_small)   { if (!sp) TC_SKIP("n/a"); run("C:\\d\\f.ext", _MAX_DRIVE, _MAX_DIR, _MAX_FNAME, 3); }
TC_CASE(wide_small)
{
    wchar_t d[_MAX_DRIVE], di[_MAX_DIR], f[_MAX_FNAME], e[_MAX_EXT]; int r;
    static volatile const wchar_t *p = L"abcdef";
    if (!wsp) TC_SKIP("n/a");
    tc_iph_count = 0; errno = 0;
    r = wsp((const wchar_t *)p, d, _MAX_DRIVE, di, _MAX_DIR, f, 3, e, _MAX_EXT);
    CHECK_EQ_I(r, ERANGE);
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    sp = (sp_t)tc_crt_proc("_splitpath_s"); wsp = (wsp_t)tc_crt_proc("_wsplitpath_s");
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph api\n"); return 1; }
    TC_RUN(fname_small); TC_RUN(drive_small); TC_RUN(dir_small); TC_RUN(ext_small); TC_RUN(wide_small);
    return tc_end();
}
