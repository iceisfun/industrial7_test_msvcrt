/* TC
 * id:    fio_wrename_basic
 * area:  stdio_file
 * funcs: _wrename _wfopen fclose
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs rename, _wrename
 * note:  Unicode names built from wide escapes.
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>

static const wchar_t * volatile wa = L"tc_fio_wrename_basic_\x00e9\x4e2d_a.tmp";
static const wchar_t * volatile wb = L"tc_fio_wrename_basic_\x00e9\x4e2d_b.tmp";

TC_CASE(rename_unicode)
{
    FILE *f = _wfopen(wa, L"wb"); CHECK(f != NULL); if (!f) return;
    fputc('q', f); fclose(f);
    CHECK_EQ_I(_wrename(wa, wb), 0);
    CHECK_EQ_U(GetFileAttributesW(wa), INVALID_FILE_ATTRIBUTES);
    CHECK(GetFileAttributesW(wb) != INVALID_FILE_ATTRIBUTES);
}
int main(void)
{
    tc_begin();
    DeleteFileW(wa); DeleteFileW(wb);
    TC_RUN(rename_unicode);
    DeleteFileW(wa); DeleteFileW(wb);
    return tc_end();
}
