/* TC
 * id:    pfo_neg_probe_vfprintf_readonly_errno
 * area:  stdio_printf
 * kind:  negative
 * funcs: __stdio_common_vfprintf __stdio_common_vfwprintf
 * crt:   ucrtbase
 * tier:  p1
 * conf:  probe
 * ref:   MS docs fprintf, fwprintf (negative value on output error); fopen modes
 * note:  PROBES: writing to a stream opened "r" returns -1 with errno EBADF, no invalid-parameter handler call, ferror set,
 *        and leaves the file contents unchanged. The negative return itself is asserted in pfo_neg_vfwprintf_wrong_mode_null.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <stdarg.h>
#include <errno.h>

#define TMP "tc_pfo_neg_probe_vfprintf_readonly.tmp"
static int cf(FILE *fp, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vfprintf(0, fp, f, NULL, ap); va_end(ap); return r; }
static int cfw(FILE *fp, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vfwprintf(0, fp, f, NULL, ap); va_end(ap); return r; }
static FILE *mk(void)
{
    DWORD n; HANDLE h = CreateFileA(TMP, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h == INVALID_HANDLE_VALUE) return NULL;
    WriteFile(h, "hello\n", 6, &n, NULL); CloseHandle(h);
    return fopen(TMP, "r");
}
static int contents_ok(void)
{
    char buf[16]; DWORD n = 0; int ok;
    HANDLE h = CreateFileA(TMP, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h == INVALID_HANDLE_VALUE) return 0;
    ReadFile(h, buf, sizeof buf, &n, NULL); CloseHandle(h);
    ok = n == 6 && buf[0] == 'h' && buf[4] == 'o' && buf[5] == '\n';
    return ok;
}

TC_CASE(narrow_errno)
{
    FILE *fp = mk(); int r; if (!fp) TC_SKIP("temp");
    errno = 0; r = cf(fp, "x");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EBADF);
    fclose(fp); DeleteFileA(TMP);
}
TC_CASE(wide_errno)
{
    FILE *fp = mk(); int r; if (!fp) TC_SKIP("temp");
    errno = 0; r = cfw(fp, L"x");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EBADF);
    fclose(fp); DeleteFileA(TMP);
}
TC_CASE(no_handler_and_ferror)
{
    FILE *fp = mk(); int r; if (!fp) TC_SKIP("temp");
    tc_iph_count = 0;
    r = cf(fp, "x");
    CHECK(r < 0); CHECK_EQ_I(tc_iph_count, 0); CHECK(ferror(fp) != 0);
    fclose(fp); DeleteFileA(TMP);
}
TC_CASE(contents_unchanged)
{
    FILE *fp = mk(); if (!fp) TC_SKIP("temp");
    cf(fp, "xyz"); fflush(fp); fclose(fp);
    CHECK(contents_ok());
    DeleteFileA(TMP);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) return 2;
    TC_RUN(narrow_errno); TC_RUN(wide_errno); TC_RUN(no_handler_and_ferror); TC_RUN(contents_unchanged);
    return tc_end();
}
