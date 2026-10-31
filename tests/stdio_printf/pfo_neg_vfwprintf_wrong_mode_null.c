/* TC
 * id:    pfo_neg_vfwprintf_wrong_mode_null
 * area:  stdio_printf
 * kind:  negative
 * funcs: __stdio_common_vfwprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fwprintf, fwprintf_s (NULL stream/format -> invalid parameter handler, -1, EINVAL; negative return on output error)
 * note:  For the read-only stream only a negative return is asserted.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <stdarg.h>
#include <errno.h>

static int vfw(FILE *f, const wchar_t *fmt, ...)
{
    va_list ap; int r;
    va_start(ap, fmt);
    r = __stdio_common_vfwprintf(0, f, fmt, NULL, ap);
    va_end(ap);
    return r;
}
#define TMP "tc_pfo_neg_vfwprintf_wrong_mode_null.tmp"
static FILE *mk(const char *mode)
{
    DWORD n; HANDLE h = CreateFileA(TMP, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h != INVALID_HANDLE_VALUE) { WriteFile(h, "hello\n", 6, &n, NULL); CloseHandle(h); }
    return fopen(TMP, mode);
}

TC_CASE(null_stream)
{
    FILE *volatile f = NULL; int r;
    tc_iph_count = 0; errno = 0;
    r = vfw(f, L"abc");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_format)
{
    const wchar_t *volatile fmt = NULL; FILE *f = mk("w"); int r;
    if (!f) TC_SKIP("cannot open temp file");
    tc_iph_count = 0; errno = 0;
    r = vfw(f, fmt);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    fclose(f); DeleteFileA(TMP);
}
TC_CASE(readonly_stream)
{
    FILE *f = mk("r"); int r;
    if (!f) TC_SKIP("cannot open temp file");
    r = vfw(f, L"%s", L"abc");
    CHECK(r < 0);
    fclose(f); DeleteFileA(TMP);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) return 2;
    TC_RUN(null_stream); TC_RUN(null_format); TC_RUN(readonly_stream);
    return tc_end();
}
