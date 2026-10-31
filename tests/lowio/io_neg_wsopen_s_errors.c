/* TC
 * id:    io_neg_wsopen_s_errors
 * area:  lowio
 * kind:  negative
 * funcs: _wsopen_s _set_invalid_parameter_handler
 * crt:   >=90
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _sopen_s, _wsopen_s
 */
#include "tc.h"
#define TCS2(x) #x
#define TCS(x) TCS2(x)
#define TCV TCS(TC_CRT_VER) /* per-CRT file names: targets may run concurrently in one directory */
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <share.h>
#include <wchar.h>
#include <sys/stat.h>

static const wchar_t *volatile wfn = L"tc_io_neg_wsopen_s_\u00e9_" TCV ".tmp";
static const wchar_t *volatile nullw = NULL;
static int *volatile nullp = NULL;

TC_CASE(missing_file)
{
    int fh = 1234; errno_t e;
    e = _wsopen_s(&fh, wfn, _O_RDONLY, _SH_DENYNO, 0);
    CHECK_EQ_I(e, ENOENT);
    CHECK_EQ_I(fh, -1);
}
TC_CASE(null_pfh)
{
    errno_t e;
    CHECK(tc_iph_install());
    tc_iph_count = 0;
    e = _wsopen_s(nullp, wfn, _O_RDONLY, _SH_DENYNO, 0);
    CHECK_EQ_I(e, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_path)
{
    int fh = 1234; errno_t e;
    CHECK(tc_iph_install());
    tc_iph_count = 0;
    e = _wsopen_s(&fh, nullw, _O_RDONLY, _SH_DENYNO, 0);
    CHECK_EQ_I(e, EINVAL);
    CHECK_EQ_I(fh, -1);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void) { tc_begin(); TC_RUN(missing_file); TC_RUN(null_pfh); TC_RUN(null_path); return tc_end(); }
