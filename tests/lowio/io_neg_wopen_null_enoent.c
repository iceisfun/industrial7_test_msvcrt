/* TC
 * id:    io_neg_wopen_null_enoent
 * area:  lowio
 * kind:  negative
 * funcs: _wopen _set_invalid_parameter_handler
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _open, _wopen (returns -1 and sets errno; invalid parameters invoke the invalid parameter handler); parameter validation
 */
#include "tc.h"
#define TCS2(x) #x
#define TCS(x) TCS2(x)
#define TCV TCS(TC_CRT_VER) /* per-CRT file names: targets may run concurrently in one directory */
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <wchar.h>
#include <sys/stat.h>

static const wchar_t *volatile nullw = NULL;
static const wchar_t *volatile missing = L"tc_io_neg_wopen_missing_\u00e9_" TCV ".tmp";
static const wchar_t *volatile missing_dir = L"tc_io_neg_wopen_nodir\\x_" TCV ".tmp";

TC_CASE(null_path)
{
    int fd;
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0;
    fd = _wopen(nullw, _O_RDONLY);
    CHECK_EQ_I(fd, -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(missing_file)
{
    int fd;
    tc_iph_count = 0; errno = 0;
    fd = _wopen(missing, _O_RDONLY);
    CHECK_EQ_I(fd, -1);
    CHECK_EQ_I(errno, ENOENT);
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(missing_directory)
{
    int fd;
    errno = 0;
    fd = _wopen(missing_dir, _O_CREAT | _O_WRONLY, _S_IREAD | _S_IWRITE);
    CHECK_EQ_I(fd, -1);
    CHECK_EQ_I(errno, ENOENT);
}
int main(void) { tc_begin(); TC_RUN(null_path); TC_RUN(missing_file); TC_RUN(missing_directory); return tc_end(); }
