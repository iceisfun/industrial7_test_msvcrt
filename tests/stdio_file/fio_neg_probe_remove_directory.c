/* TC
 * id:    fio_neg_probe_remove_directory
 * area:  stdio_file
 * funcs: remove
 * crt:   all
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs remove: path is a directory => EACCES
 * note:  PROBE: remove() on a directory fails with -1; errno EACCES is the documented value (DeleteFile on a directory gives ERROR_ACCESS_DENIED), directory survives.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
static const char * volatile dname = "tc_fio_neg_probe_remove_directory_d.tmp";

TC_CASE(dir)
{
    DWORD a;
    RemoveDirectoryA(dname);
    CHECK(CreateDirectoryA(dname, NULL));
    errno = 0;
    CHECK_EQ_I(remove(dname), -1);
    CHECK_EQ_I(errno, EACCES);
    a = GetFileAttributesA(dname);
    CHECK(a != INVALID_FILE_ATTRIBUTES && (a & FILE_ATTRIBUTE_DIRECTORY));
}
int main(void) { tc_begin(); TC_RUN(dir); RemoveDirectoryA(dname); return tc_end(); }
