/* TC
 * id:    gio_neg_sopen_dispatch_probe
 * area:  lowio
 * funcs:
 * procs: _sopen_dispatch _wsopen_dispatch
 * crt:   ucrtbase
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _sopen_s, _wsopen_s (error contract) - assumed to be what the dispatch helper implements
 * note:  PROBES error returns of the undocumented _sopen_dispatch(path,oflag,shflag,pmode,int*pfh,bSecure): returns an errno value
 *        (not -1), sets *pfh = -1, and in secure mode invokes the invalid-parameter handler for NULL path / NULL pfh.
 */
#include "tc.h"
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <share.h>
#include <sys/stat.h>

typedef int (__cdecl *sd_t)(const char *, int, int, int, int *, int);
typedef int (__cdecl *wsd_t)(const wchar_t *, int, int, int, int *, int);
static sd_t sd; static wsd_t wsd;
static const char *volatile missing = "tc_gio_neg_dispatch_missing.tmp";
#define PM (_S_IREAD | _S_IWRITE)

TC_CASE(secure_null_path)
{
    int fh = -2, r, n0;
    if (!sd) TC_SKIP("not exported");
    n0 = tc_iph_count;
    r = sd(NULL, _O_RDONLY, _SH_DENYNO, PM, &fh, 1);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(fh, -1); CHECK_EQ_I(tc_iph_count, n0 + 1);
    if (fh >= 0) _close(fh);
}
TC_CASE(secure_missing)
{
    int fh = -2, r;
    if (!sd) TC_SKIP("not exported");
    DeleteFileA(missing);
    r = sd(missing, _O_RDONLY | _O_BINARY, _SH_DENYNO, PM, &fh, 1);
    CHECK_EQ_I(r, ENOENT); CHECK_EQ_I(fh, -1);
    if (fh >= 0) _close(fh);
}
TC_CASE(insecure_missing)
{
    int fh = -2, r;
    if (!sd) TC_SKIP("not exported");
    r = sd(missing, _O_RDONLY | _O_BINARY, _SH_DENYNO, PM, &fh, 0);
    CHECK_EQ_I(r, ENOENT); CHECK_EQ_I(fh, -1);
    if (fh >= 0) _close(fh);
}
TC_CASE(null_pfh_secure)
{
    int r, n0;
    if (!sd) TC_SKIP("not exported");
    n0 = tc_iph_count;
    r = sd(missing, _O_RDONLY | _O_BINARY, _SH_DENYNO, PM, NULL, 1);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, n0 + 1);
}
TC_CASE(wide_null_path_secure)
{
    int fh = -2, r, n0;
    if (!wsd) TC_SKIP("not exported");
    n0 = tc_iph_count;
    r = wsd(NULL, _O_RDONLY, _SH_DENYNO, PM, &fh, 1);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(fh, -1); CHECK_EQ_I(tc_iph_count, n0 + 1);
    if (fh >= 0) _close(fh);
}
int main(void)
{
    tc_begin();
    sd = (sd_t)tc_crt_proc("_sopen_dispatch"); wsd = (wsd_t)tc_crt_proc("_wsopen_dispatch");
    tc_iph_install();
    TC_RUN(secure_null_path); TC_RUN(secure_missing); TC_RUN(insecure_missing);
    TC_RUN(null_pfh_secure); TC_RUN(wide_null_path_secure);
    return tc_end();
}
