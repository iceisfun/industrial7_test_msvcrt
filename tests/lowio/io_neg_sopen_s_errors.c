/* TC
 * id:    io_neg_sopen_s_errors
 * area:  lowio
 * kind:  negative
 * funcs: _sopen_s _set_invalid_parameter_handler _unlink _close
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _sopen_s, _wsopen_s (returns an errno value; *pfh is set to -1 on error; NULL pfh/filename -> EINVAL via invalid parameter handler)
 */
#include "tc.h"
#define TCS2(x) #x
#define TCS(x) TCS2(x)
#define TCV TCS(TC_CRT_VER) /* per-CRT file names: targets may run concurrently in one directory */
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <share.h>
#include <sys/stat.h>

static const char *volatile fn = "tc_io_neg_sopen_s_" TCV ".tmp";
static const char *volatile nullc = NULL;
static int *volatile nullp = NULL;

TC_CASE(missing_file)
{
    int fh = 1234; errno_t e;
    _unlink(fn);
    e = _sopen_s(&fh, fn, _O_RDONLY, _SH_DENYNO, 0);
    CHECK_EQ_I(e, ENOENT);
    CHECK_EQ_I(fh, -1);
}
TC_CASE(create_excl_existing)
{
    int fh = 1234, a = -1; errno_t e;
    _unlink(fn);
    CHECK_EQ_I(_sopen_s(&a, fn, _O_CREAT | _O_WRONLY, _SH_DENYNO, _S_IREAD | _S_IWRITE), 0);
    if (a >= 0) _close(a);
    e = _sopen_s(&fh, fn, _O_CREAT | _O_EXCL | _O_WRONLY, _SH_DENYNO, _S_IREAD | _S_IWRITE);
    CHECK_EQ_I(e, EEXIST);
    CHECK_EQ_I(fh, -1);
    _unlink(fn);
}
TC_CASE(null_pfh)
{
    errno_t e;
    CHECK(tc_iph_install());
    tc_iph_count = 0;
    e = _sopen_s(nullp, fn, _O_RDONLY, _SH_DENYNO, 0);
    CHECK_EQ_I(e, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_filename)
{
    int fh = 1234; errno_t e;
    CHECK(tc_iph_install());
    tc_iph_count = 0;
    e = _sopen_s(&fh, nullc, _O_RDONLY, _SH_DENYNO, 0);
    CHECK_EQ_I(e, EINVAL);
    CHECK_EQ_I(fh, -1);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(sharing_violation_eacces)
{
    int a = -1, b = 1234; errno_t e;
    _unlink(fn);
    CHECK_EQ_I(_sopen_s(&a, fn, _O_CREAT | _O_RDWR, _SH_DENYRW, _S_IREAD | _S_IWRITE), 0);
    e = _sopen_s(&b, fn, _O_RDONLY, _SH_DENYNO, 0);
    CHECK_EQ_I(e, EACCES);
    CHECK_EQ_I(b, -1);
    if (a >= 0) _close(a);
    _unlink(fn);
}
int main(void)
{
    tc_begin();
    TC_RUN(missing_file); TC_RUN(create_excl_existing); TC_RUN(null_pfh);
    TC_RUN(null_filename); TC_RUN(sharing_violation_eacces);
    return tc_end();
}
