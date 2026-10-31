/* TC
 * id:    ucr_fmode_accessors
 * area:  ucrt_only
 * funcs: __p__fmode _set_fmode _get_fmode
 * crt:   win10
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _fmode, _set_fmode, _get_fmode: default translation mode for fopen/_open
 */
#include "tc.h"
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

#define FN "tc_ucr_fmode_accessors.tmp"
static long fsize(void)
{
    HANDLE h = CreateFileA(FN, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL); DWORD n;
    if (h == INVALID_HANDLE_VALUE) return -1;
    n = GetFileSize(h, NULL); CloseHandle(h); return (long)n;
}
static long write_nl(void)
{
    FILE *f = fopen(FN, "w"); long n;
    if (!f) return -1;
    fputs("a\n", f); fclose(f);
    n = fsize(); DeleteFileA(FN); return n;
}
TC_CASE(default_text) { CHECK_EQ_I(write_nl(), 3); }  /* fopen "w" without t/b: default text mode */
TC_CASE(binary)
{
    int m = -1;
    CHECK_EQ_I(_set_fmode(_O_BINARY), 0);
    CHECK_EQ_I(_get_fmode(&m), 0); CHECK_EQ_I(m, _O_BINARY);
    CHECK_EQ_I(*__p__fmode(), _O_BINARY);
    CHECK_EQ_I(write_nl(), 2);
}
TC_CASE(restore_text)
{
    int m = -1;
    CHECK_EQ_I(_set_fmode(_O_TEXT), 0);
    CHECK_EQ_I(_get_fmode(&m), 0); CHECK_EQ_I(m, _O_TEXT);
    CHECK_EQ_I(*__p__fmode(), _O_TEXT);
    CHECK_EQ_I(write_nl(), 3);
}
int main(void) { tc_begin(); TC_RUN(default_text); TC_RUN(binary); TC_RUN(restore_text); DeleteFileA(FN); return tc_end(); }
