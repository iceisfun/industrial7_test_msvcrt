/* TC
 * id:    scn_common_vfscanf_basic
 * area:  scanf
 * funcs: __stdio_common_vfscanf __stdio_common_vfwscanf fopen fclose
 * crt:   ucrtbase
 * tier:  p1
 * conf:  documented
 * ref:   stdio.h __stdio_common_vfscanf / __stdio_common_vfwscanf (options _CRT_INTERNAL_SCANF_*); MS docs fscanf, fscanf_s, fwscanf; MS docs "Unicode stream I/O in text and binary modes" (wide read of an ANSI text stream)
 * note:  Test file is created with kernel32 and read through the CRT FILE.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

static const char *fname = "tc_scn_common_vfscanf_basic.tmp";
static FILE *mk(const char *content, DWORD len)
{
    HANDLE h = CreateFileA(fname, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); DWORD w;
    if (h == INVALID_HANDLE_VALUE) return NULL;
    WriteFile(h, content, len, &w, NULL); CloseHandle(h);
    return fopen(fname, "r");
}
static void done(FILE *f) { if (f) fclose(f); DeleteFileA(fname); }
static int cf(unsigned long long o, FILE *f, const char *fmt, ...)
{ va_list ap; int r; va_start(ap, fmt); r = __stdio_common_vfscanf(o, f, fmt, NULL, ap); va_end(ap); return r; }
static int cfw(unsigned long long o, FILE *f, const wchar_t *fmt, ...)
{ va_list ap; int r; va_start(ap, fmt); r = __stdio_common_vfwscanf(o, f, fmt, NULL, ap); va_end(ap); return r; }

TC_CASE(narrow_ints)
{
    FILE *f; int a = 0, b = 0, n;
    f = mk("5 -6\n", 5); CHECK(f != NULL); if (!f) return;
    n = cf(0, f, "%d %d", &a, &b);
    CHECK_EQ_I(n, 2); CHECK_EQ_I(a, 5); CHECK_EQ_I(b, -6);
    done(f);
}
TC_CASE(wide_ints)
{
    FILE *f; int a = 0, b = 0, n;
    f = mk("15 26\n", 6); CHECK(f != NULL); if (!f) return;
    n = cfw(2, f, L"%d %d", &a, &b);
    CHECK_EQ_I(n, 2); CHECK_EQ_I(a, 15); CHECK_EQ_I(b, 26);
    done(f);
}
TC_CASE(secure_s)
{
    FILE *f; char s[8]; int n, v = 0;
    f = mk("tok 3\n", 6); CHECK(f != NULL); if (!f) return;
    n = cf(1, f, "%s %d", s, (unsigned)8, &v);
    CHECK_EQ_I(n, 2); CHECK_STR(s, "tok"); CHECK_EQ_I(v, 3);
    done(f);
}
TC_CASE(secure_wide_s)
{
    FILE *f; wchar_t s[8]; int n, v = 0;
    f = mk("tok 3\n", 6); CHECK(f != NULL); if (!f) return;
    n = cfw(3, f, L"%s %d", s, (unsigned)8, &v);
    CHECK_EQ_I(n, 2); CHECK_WSTR(s, L"tok"); CHECK_EQ_I(v, 3);
    done(f);
}
TC_CASE(eof_empty)
{
    FILE *f; int a = 1, n;
    f = mk("", 0); CHECK(f != NULL); if (!f) return;
    n = cf(0, f, "%d", &a);
    CHECK_EQ_I(n, EOF); CHECK_EQ_I(a, 1);
    done(f);
}
int main(void)
{
    tc_begin();
    TC_RUN(narrow_ints); TC_RUN(wide_ints); TC_RUN(secure_s); TC_RUN(secure_wide_s); TC_RUN(eof_empty);
    return tc_end();
}
