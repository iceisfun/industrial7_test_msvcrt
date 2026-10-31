/* TC
 * id:    gio_neg_probe_sopen_fd_leak
 * area:  lowio
 * funcs: _sopen _close _unlink
 * crt:   all
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   none (lowest-free descriptor allocation is not documented by MS)
 * note:  PROBES that a failed _sopen (sharing violation) does not consume a descriptor slot: with fd X freed and a
 *        second file held open DENYRW, a failed open followed by a successful open must return the same lowest-free X.
 */
#include "tc.h"
#define TCS2(x) #x
#define TCS(x) TCS2(x)
#define TCV TCS(TC_CRT_VER)
#include <io.h>
#include <fcntl.h>
#include <share.h>
#include <sys/stat.h>

static const char *volatile fn = "tc_gio_neg_fdleak_a_" TCV ".tmp";
static const char *volatile fn2 = "tc_gio_neg_fdleak_b_" TCV ".tmp";
#define PM (_S_IREAD | _S_IWRITE)

TC_CASE(failed_open_no_fd_leak)
{
    int a, x, x2, b;
    _unlink(fn); _unlink(fn2);
    a = _sopen(fn, _O_CREAT | _O_RDWR | _O_BINARY, _SH_DENYRW, PM);
    CHECK(a >= 3);
    x = _sopen(fn2, _O_CREAT | _O_RDWR | _O_BINARY, _SH_DENYNO, PM);
    CHECK(x >= 3);
    if (x >= 0) _close(x);                 /* x is now the lowest free slot */
    b = _sopen(fn, _O_RDONLY | _O_BINARY, _SH_DENYNO);
    CHECK_EQ_I(b, -1);
    if (b >= 0) _close(b);
    x2 = _sopen(fn2, _O_RDWR | _O_BINARY, _SH_DENYNO);
    CHECK_EQ_I(x2, x);
    if (x2 >= 0) _close(x2);
    if (a >= 0) _close(a);
    _unlink(fn); _unlink(fn2);
}
int main(void)
{
    tc_begin();
    TC_RUN(failed_open_no_fd_leak);
    return tc_end();
}
