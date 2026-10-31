/* TC
 * id:    gio_sopen_dispatch_probe
 * area:  lowio
 * funcs: _close _write
 * procs: _sopen_dispatch _wsopen_dispatch
 * crt:   ucrtbase
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   none (undocumented ucrtbase exports; not in MS headers)
 * note:  PROBES the signature errno_t _sopen_dispatch(const char *path, int oflag, int shflag, int pmode, int *pfh, int bSecure)
 *        (and the wchar_t twin) and that success returns 0 with *pfh a usable descriptor.
 */
#include "tc.h"
#include <io.h>
#include <fcntl.h>
#include <share.h>
#include <sys/stat.h>

typedef int (__cdecl *sd_t)(const char *, int, int, int, int *, int);
typedef int (__cdecl *wsd_t)(const wchar_t *, int, int, int, int *, int);
static sd_t sd; static wsd_t wsd;
static const char *volatile fn = "tc_gio_sopen_dispatch_probe.tmp";
static const wchar_t *volatile wfn = L"tc_gio_sopen_dispatch_probe_w.tmp";
#define PM (_S_IREAD | _S_IWRITE)
#define OF (_O_CREAT | _O_RDWR | _O_BINARY)

TC_CASE(secure_create)
{
    int fh = -2, r;
    if (!sd) TC_SKIP("_sopen_dispatch not exported");
    DeleteFileA(fn);
    r = sd(fn, OF, _SH_DENYNO, PM, &fh, 1);
    CHECK_EQ_I(r, 0); CHECK(fh >= 3);
    if (fh >= 0) _close(fh);
    DeleteFileA(fn);
}
TC_CASE(insecure_create)
{
    int fh = -2, r;
    if (!sd) TC_SKIP("_sopen_dispatch not exported");
    DeleteFileA(fn);
    r = sd(fn, OF, _SH_DENYNO, PM, &fh, 0);
    CHECK_EQ_I(r, 0); CHECK(fh >= 3);
    if (fh >= 0) _close(fh);
    DeleteFileA(fn);
}
TC_CASE(wide_secure)
{
    int fh = -2, r;
    if (!wsd) TC_SKIP("_wsopen_dispatch not exported");
    DeleteFileW(wfn);
    r = wsd(wfn, OF, _SH_DENYNO, PM, &fh, 1);
    CHECK_EQ_I(r, 0); CHECK(fh >= 3);
    if (fh >= 0) _close(fh);
    DeleteFileW(wfn);
}
TC_CASE(file_exists_after)
{
    int fh = -2, r;
    if (!sd) TC_SKIP("_sopen_dispatch not exported");
    DeleteFileA(fn);
    r = sd(fn, OF, _SH_DENYNO, PM, &fh, 1);
    CHECK_EQ_I(r, 0);
    CHECK(GetFileAttributesA(fn) != INVALID_FILE_ATTRIBUTES);
    if (fh >= 0) { CHECK_EQ_I(_write(fh, "ab", 2), 2); CHECK_EQ_I(_close(fh), 0); }
    DeleteFileA(fn);
}
int main(void)
{
    tc_begin();
    sd = (sd_t)tc_crt_proc("_sopen_dispatch"); wsd = (wsd_t)tc_crt_proc("_wsopen_dispatch");
    TC_RUN(secure_create); TC_RUN(insecure_create); TC_RUN(wide_secure); TC_RUN(file_exists_after);
    return tc_end();
}
