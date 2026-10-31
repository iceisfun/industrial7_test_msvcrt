/* TC
 * id:    fio_neg_fopen_readonly_file_write
 * area:  stdio_file
 * kind:  negative
 * funcs: fopen fclose
 * procs: 
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fopen / errno; Win32 ERROR_ACCESS_DENIED maps to EACCES
 * note:  Uses SetFileAttributesA to make the file read-only; the file is created with kernel32 only.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static const char * volatile fname = "tc_fio_neg_fopen_readonly_file_write_a.tmp";

static void cleanup(void)
{
    SetFileAttributesA(fname, FILE_ATTRIBUTE_NORMAL);
    DeleteFileA(fname);
}
TC_CASE(write_denied)
{
    FILE *f; errno = 0;
    f = fopen(fname, "w");
    CHECK(f == NULL);
    CHECK_EQ_I(errno, EACCES);
    if (f) fclose(f);
}
TC_CASE(append_denied)
{
    FILE *f; errno = 0;
    f = fopen(fname, "a");
    CHECK(f == NULL);
    CHECK_EQ_I(errno, EACCES);
    if (f) fclose(f);
}
TC_CASE(readplus_denied)
{
    FILE *f; errno = 0;
    f = fopen(fname, "r+");
    CHECK(f == NULL);
    CHECK_EQ_I(errno, EACCES);
    if (f) fclose(f);
}
TC_CASE(read_ok)
{
    FILE *f = fopen(fname, "r");
    CHECK(f != NULL);
    if (f) CHECK_EQ_I(fclose(f), 0);
}
int main(void)
{
    HANDLE h;
    tc_begin();
    cleanup();
    h = CreateFileA(fname, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h == INVALID_HANDLE_VALUE) { tc_write("TC FAIL setup\n"); return 2; }
    CloseHandle(h);
    if (!SetFileAttributesA(fname, FILE_ATTRIBUTE_READONLY)) { cleanup(); tc_write("TC FAIL setattr\n"); return 2; }
    TC_RUN(write_denied); TC_RUN(append_denied); TC_RUN(readplus_denied); TC_RUN(read_ok);
    cleanup();
    return tc_end();
}
