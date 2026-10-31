/* TC
 * id:    fio_neg_wremove_args
 * area:  stdio_file
 * funcs: _wremove
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs remove, _wremove: ENOENT for missing path
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <wchar.h>
static const wchar_t * volatile wname = L"tc_fio_neg_wremove_args_\x00e9.tmp";
static const wchar_t * volatile wmissing = L"tc_fio_neg_wremove_args_missing.tmp";

TC_CASE(missing)
{
    errno = 0;
    CHECK_EQ_I(_wremove(wmissing), -1);
    CHECK_EQ_I(errno, ENOENT);
}
TC_CASE(existing)
{
    HANDLE h = CreateFileW(wname, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    CHECK(h != INVALID_HANDLE_VALUE);
    if (h == INVALID_HANDLE_VALUE) return;
    CloseHandle(h);
    CHECK_EQ_I(_wremove(wname), 0);
    CHECK(GetFileAttributesW(wname) == INVALID_FILE_ATTRIBUTES);
}
int main(void) { tc_begin(); TC_RUN(missing); TC_RUN(existing); DeleteFileW(wname); return tc_end(); }
