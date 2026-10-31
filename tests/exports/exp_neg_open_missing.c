/* TC
 * id:    exp_neg_open_missing
 * area:  exports
 * kind:  negative
 * funcs: _open
 * procs: 
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _open: returns -1, errno ENOENT if file or path not found
 * note:  
 */
#include "tc.h"
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

static const char *volatile nofile = "tc_exp_neg_open_missing_nonexistent.tmp";
static const char *volatile nodir = "tc_exp_no_such_dir_xyz\\f.tmp";

TC_CASE(minus1_enoent) { int fd; errno = 0; fd = _open(nofile, _O_RDONLY); CHECK_EQ_I(fd, -1); CHECK_EQ_I(errno, ENOENT); if (fd >= 0) _close(fd); }
TC_CASE(no_creat_flag) { int fd; errno = 0; fd = _open(nofile, _O_RDWR | _O_BINARY); CHECK_EQ_I(fd, -1); CHECK_EQ_I(errno, ENOENT); if (fd >= 0) _close(fd); }
TC_CASE(dir_missing_enoent) { int fd; errno = 0; fd = _open(nodir, _O_WRONLY | _O_CREAT, _S_IREAD | _S_IWRITE); CHECK_EQ_I(fd, -1); CHECK_EQ_I(errno, ENOENT); if (fd >= 0) _close(fd); }

int main(void) { tc_begin(); TC_RUN(minus1_enoent); TC_RUN(no_creat_flag); TC_RUN(dir_missing_enoent); return tc_end(); }
