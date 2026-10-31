/* TC
 * id:    pfo_neg_vfprintf_closed_file
 * area:  stdio_printf
 * kind:  negative
 * funcs: __stdio_common_vfprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fprintf, fprintf_s ("If stream is a null pointer, the invalid parameter handler is invoked ... returns -1, errno EINVAL"; returns a negative value on output error)
 * note:  Despite the id, a closed FILE* is never used (UB). Cases: NULL stream, NULL format, stream opened read-only. For the read-only
 *        stream only a negative return is asserted (errno/handler not asserted).
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

static int vf(FILE *f, const char *fmt, ...)
{
    va_list ap; int r;
    va_start(ap, fmt);
    r = __stdio_common_vfprintf(0, f, fmt, NULL, ap);
    va_end(ap);
    return r;
}

TC_CASE(null_stream)
{
    FILE *volatile f = NULL; int r;
    tc_iph_count = 0; errno = 0;
    r = vf(f, "abc");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_format)
{
    const char *volatile fmt = NULL; FILE *f; int r;
    DWORD n; HANDLE h = CreateFileA("tc_pfo_neg_vfprintf_closed_file_a.tmp", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h != INVALID_HANDLE_VALUE) { WriteFile(h, "x", 1, &n, NULL); CloseHandle(h); }
    f = fopen("tc_pfo_neg_vfprintf_closed_file_a.tmp", "w");
    if (!f) TC_SKIP("cannot open temp file");
    tc_iph_count = 0; errno = 0;
    r = vf(f, fmt);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    fclose(f);
    DeleteFileA("tc_pfo_neg_vfprintf_closed_file_a.tmp");
}
TC_CASE(readonly_stream)
{
    FILE *f; int r; DWORD n;
    HANDLE h = CreateFileA("tc_pfo_neg_vfprintf_closed_file_b.tmp", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h != INVALID_HANDLE_VALUE) { WriteFile(h, "hello\n", 6, &n, NULL); CloseHandle(h); }
    f = fopen("tc_pfo_neg_vfprintf_closed_file_b.tmp", "r");
    if (!f) TC_SKIP("cannot open temp file");
    r = vf(f, "%s", "abc");
    CHECK(r < 0);
    fclose(f);
    DeleteFileA("tc_pfo_neg_vfprintf_closed_file_b.tmp");
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) return 2;
    TC_RUN(null_stream); TC_RUN(null_format); TC_RUN(readonly_stream);
    return tc_end();
}
