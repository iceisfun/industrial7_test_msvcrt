/* TC
 * id:    fio_neg_remove_readonly
 * area:  stdio_file
 * funcs: remove
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs remove: read-only file => EACCES
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
static const char * volatile fname = "tc_fio_neg_remove_readonly_a.tmp";

static void cleanup(void) { SetFileAttributesA(fname, FILE_ATTRIBUTE_NORMAL); DeleteFileA(fname); }
TC_CASE(readonly)
{
    HANDLE h;
    cleanup();
    h = CreateFileA(fname, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    CHECK(h != INVALID_HANDLE_VALUE);
    if (h == INVALID_HANDLE_VALUE) return;
    CloseHandle(h);
    CHECK(SetFileAttributesA(fname, FILE_ATTRIBUTE_READONLY));
    errno = 0;
    CHECK_EQ_I(remove(fname), -1);
    CHECK_EQ_I(errno, EACCES);
    CHECK(GetFileAttributesA(fname) != INVALID_FILE_ATTRIBUTES);
    CHECK(SetFileAttributesA(fname, FILE_ATTRIBUTE_NORMAL));
    CHECK_EQ_I(remove(fname), 0);
    CHECK(GetFileAttributesA(fname) == INVALID_FILE_ATTRIBUTES);
}
int main(void) { tc_begin(); TC_RUN(readonly); cleanup(); return tc_end(); }
