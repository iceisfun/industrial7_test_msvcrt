/* TC
 * id:    dir_neg_probe_splitpath_s_reset
 * area:  dir
 * kind:  negative
 * funcs:
 * procs: _splitpath_s _set_invalid_parameter_handler
 * crt:   >=80
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _splitpath_s (error handling)
 * note:  Probes that after a buffer-too-small failure ALL four output buffers are left as empty strings (the CRT's
 *        usual reset-on-error convention); not stated explicitly in the docs. Return value is not asserted here.
 */
#include "tc.h"

typedef int (__cdecl *sp_t)(const char *, char *, size_t, char *, size_t, char *, size_t, char *, size_t);
static sp_t sp;
static const char *vs(const char *s) { static volatile const char *v; v = s; return (const char *)v; }

static void run(const char *path, size_t ds, size_t dis, size_t fs, size_t es)
{
    char d[_MAX_DRIVE], di[_MAX_DIR], f[_MAX_FNAME], e[_MAX_EXT];
    d[0] = di[0] = f[0] = e[0] = 'X'; d[1] = di[1] = f[1] = e[1] = 0;
    sp(vs(path), d, ds, di, dis, f, fs, e, es);
    CHECK_STR(d, ""); CHECK_STR(di, ""); CHECK_STR(f, ""); CHECK_STR(e, "");
}
TC_CASE(fname_small) { if (!sp) TC_SKIP("n/a"); run("abcdef", _MAX_DRIVE, _MAX_DIR, 3, _MAX_EXT); }
TC_CASE(drive_small) { if (!sp) TC_SKIP("n/a"); run("C:\\d\\f.e", 2, _MAX_DIR, _MAX_FNAME, _MAX_EXT); }
TC_CASE(dir_small)   { if (!sp) TC_SKIP("n/a"); run("C:\\dir\\sub\\f.e", _MAX_DRIVE, 5, _MAX_FNAME, _MAX_EXT); }
TC_CASE(ext_small)   { if (!sp) TC_SKIP("n/a"); run("C:\\d\\f.ext", _MAX_DRIVE, _MAX_DIR, _MAX_FNAME, 3); }
int main(void)
{
    tc_begin();
    sp = (sp_t)tc_crt_proc("_splitpath_s");
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph api\n"); return 1; }
    TC_RUN(fname_small); TC_RUN(drive_small); TC_RUN(dir_small); TC_RUN(ext_small);
    return tc_end();
}
