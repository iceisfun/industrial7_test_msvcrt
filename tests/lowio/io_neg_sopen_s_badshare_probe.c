/* TC
 * id:    io_neg_sopen_s_badshare_probe
 * area:  lowio
 * kind:  negative
 * funcs: _sopen_s _set_invalid_parameter_handler _unlink
 * crt:   >=90
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _sopen_s (invalid shflag is a parameter validation failure)
 * note:  PROBE: an undefined shflag (0x7777) makes _sopen_s return EINVAL, set *pfh = -1 and call the invalid parameter handler.
 *        Believed from CRT source memory (default case of the share switch); not spelled out in the docs.
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

static const char *volatile fn = "tc_io_neg_sopen_s_bs_" TCV ".tmp";
static volatile int bad = 0x7777;

TC_CASE(bad_shflag_return_and_pfh)
{
    int fh = 1234; errno_t e;
    CHECK(tc_iph_install());
    e = _sopen_s(&fh, fn, _O_CREAT | _O_WRONLY, bad, _S_IREAD | _S_IWRITE);
    CHECK_EQ_I(e, EINVAL);
    CHECK_EQ_I(fh, -1);
    if (fh >= 0) _close(fh);
    _unlink(fn);
}
TC_CASE(bad_shflag_calls_handler)
{
    int fh = 1234;
    CHECK(tc_iph_install());
    tc_iph_count = 0;
    (void)_sopen_s(&fh, fn, _O_CREAT | _O_WRONLY, bad, _S_IREAD | _S_IWRITE);
    CHECK_EQ_I(tc_iph_count, 1);
    if (fh >= 0) _close(fh);
    _unlink(fn);
}
TC_CASE(bad_shflag_creates_nothing)
{
    int fh = 1234;
    CHECK(tc_iph_install());
    _unlink(fn);
    (void)_sopen_s(&fh, fn, _O_CREAT | _O_WRONLY, bad, _S_IREAD | _S_IWRITE);
    CHECK_EQ_I(_access(fn, 0), -1);
    if (fh >= 0) _close(fh);
    _unlink(fn);
}
int main(void) { tc_begin(); TC_RUN(bad_shflag_return_and_pfh); TC_RUN(bad_shflag_calls_handler); TC_RUN(bad_shflag_creates_nothing); return tc_end(); }
