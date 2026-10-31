/* TC
 * id:    pfl_vsprintf_l_va
 * area:  stdio_printf
 * funcs: _vsprintf_l _vsnprintf_l _vfprintf_l _vscprintf_l _create_locale _free_locale fopen fclose fread
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs vsprintf, _vsprintf_l, _vsnprintf_l, _vscprintf_l, _vfprintf_l
 * note:  Wide va variant is in pfl_vswprintf_l_wide. German cases skip when the locale is not installed.
 *        Temp file tc_pfl_vsprintf_l_va_<crt>.tmp.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <locale.h>

static const char *fname = "tc_pfl_vsprintf_l_va_" TC_CRT_DLL ".tmp";

static int vs(char *b, _locale_t l, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = _vsprintf_l(b, f, l, ap); va_end(ap); return r; }
static int vn(char *b, size_t n, _locale_t l, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = _vsnprintf_l(b, n, f, l, ap); va_end(ap); return r; }
static int vc(_locale_t l, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = _vscprintf_l(f, l, ap); va_end(ap); return r; }
static int vf(FILE *fp, _locale_t l, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = _vfprintf_l(fp, f, l, ap); va_end(ap); return r; }

TC_CASE(vsprintf_l_c)
{
    _locale_t l = _create_locale(LC_ALL, "C"); char b[64]; int r; volatile double d = 2.5;
    CHECK(l != NULL); if (!l) return;
    r = vs(b, l, "%d|%.1f|%s", 7, d, "str");
    CHECK_STR(b, "7|2.5|str"); CHECK_EQ_I(r, 9);
    _free_locale(l);
}
TC_CASE(vsprintf_l_de)
{
    _locale_t l = _create_locale(LC_ALL, "German_Germany.1252"); char b[64]; int r; volatile double d = 2.5;
    if (!l) TC_SKIP("German locale not installed");
    r = vs(b, l, "%d|%.1f|%s", 7, d, "str");
    CHECK_STR(b, "7|2,5|str"); CHECK_EQ_I(r, 9);
    _free_locale(l);
}
TC_CASE(vscprintf_l_len_equals_written)
{
    _locale_t l = _create_locale(LC_ALL, "C"); char b[64]; int c, r; volatile double d = 1234.5678;
    CHECK(l != NULL); if (!l) return;
    c = vc(l, "<%8.2f|%-4d|%s>", d, 3, "hello");
    r = vs(b, l, "<%8.2f|%-4d|%s>", d, 3, "hello");
    CHECK_EQ_I(c, r); CHECK_EQ_I(c, 21);
    _free_locale(l);
}
TC_CASE(vsnprintf_l_truncation)
{
    _locale_t l = _create_locale(LC_ALL, "C"); char b[16]; int i, r;
    CHECK(l != NULL); if (!l) return;
    for (i = 0; i < 16; i++) b[i] = 'Z';
    r = vn(b, 4, l, "%s", "abcdefgh");
    CHECK_EQ_I(r, -1);
    CHECK(b[0] == 'a' && b[1] == 'b' && b[2] == 'c' && b[3] == 'd'); CHECK(b[4] == 'Z');
    r = vn(b, 5, l, "%s", "abcd");
    CHECK_EQ_I(r, 4); CHECK_STR(b, "abcd");
    r = vn(b, 4, l, "%s", "abcd");
    CHECK_EQ_I(r, 4); CHECK(b[3] == 'd');   /* exactly count chars: returned count, no terminator */
    _free_locale(l);
}
TC_CASE(vfprintf_l_file)
{
    _locale_t l = _create_locale(LC_ALL, "C"); FILE *f; char b[32]; int r; size_t n;
    CHECK(l != NULL); if (!l) return;
    f = fopen(fname, "wb"); CHECK(f != NULL); if (!f) { _free_locale(l); return; }
    r = vf(f, l, "%d-%s", 99, "xy"); fclose(f);
    CHECK_EQ_I(r, 5);
    f = fopen(fname, "rb"); CHECK(f != NULL);
    if (f) { n = fread(b, 1, sizeof b - 1, f); b[n] = 0; fclose(f); CHECK_STR(b, "99-xy"); }
    _free_locale(l);
}

int main(void)
{
    tc_begin();
    TC_RUN(vsprintf_l_c); TC_RUN(vsprintf_l_de); TC_RUN(vscprintf_l_len_equals_written); TC_RUN(vsnprintf_l_truncation); TC_RUN(vfprintf_l_file);
    DeleteFileA(fname);
    return tc_end();
}
