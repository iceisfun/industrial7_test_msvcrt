/* TC
 * id:    pfw_neg_setmode_bad_mode
 * area:  stdio_printf
 * kind:  negative
 * funcs: _setmode _open _close _unlink
 * crt:   >=80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _setmode: invalid mode -> invalid parameter handler, errno EINVAL, returns -1; returns previous mode on success
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

#define FN "tc_pfw_neg_setmode_bad_mode_a.tmp"

TC_CASE(modes)
{
    int fd = _open(FN, _O_CREAT | _O_RDWR | _O_TRUNC, _S_IREAD | _S_IWRITE); volatile int bad = 0x1234; volatile int zero = 0; int r;
    if (fd < 0) TC_SKIP("cannot create temp file");
    tc_iph_count = 0; errno = 0;
    r = _setmode(fd, bad);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    tc_iph_count = 0; errno = 0;
    r = _setmode(fd, zero);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    /* still usable: previous mode of a fresh fd is text */
    r = _setmode(fd, _O_BINARY);
    CHECK_EQ_I(r, _O_TEXT);
    r = _setmode(fd, _O_TEXT);
    CHECK_EQ_I(r, _O_BINARY);
    _close(fd); _unlink(FN);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL no iph API\n"); return 1; }
    TC_RUN(modes);
    return tc_end();
}
