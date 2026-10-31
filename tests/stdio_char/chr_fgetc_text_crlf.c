/* TC
 * id:    chr_fgetc_text_crlf
 * area:  stdio_char
 * funcs: fgetc fopen fclose
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fopen (text mode: CR-LF translated to LF on input)
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static void mk(const char *p, const void *d, DWORD n)
{
    DWORD w; HANDLE h = CreateFileA(p, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h != INVALID_HANDLE_VALUE) { WriteFile(h, d, n, &w, NULL); CloseHandle(h); }
}
static DWORD slurp(const char *p, unsigned char *b, DWORD cap)
{
    DWORD r = 0; HANDLE h = CreateFileA(p, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h != INVALID_HANDLE_VALUE) { if (!ReadFile(h, b, cap, &r, NULL)) r = 0; CloseHandle(h); }
    return r;
}
static const char *volatile path = "tc_chr_fgetc_text_crlf_" TC_CRT_DLL ".tmp";

static void rd(const char *mode, const char *data, DWORD n, int *out, int cap, int *cnt)
{
    FILE *f; int c, i = 0; mk(path, data, n); f = fopen(path, mode); *cnt = 0; if (!f) return;
    while (i < cap && (c = (fgetc)(f)) != EOF) out[i++] = c;
    *cnt = i; fclose(f);
}
TC_CASE(crlf_to_lf) { int o[8], n; rd("rt", "a\r\nb", 4, o, 8, &n); CHECK_EQ_I(n, 3); CHECK_EQ_I(o[0], 'a'); CHECK_EQ_I(o[1], '\n'); CHECK_EQ_I(o[2], 'b'); }
TC_CASE(lone_lf) { int o[8], n; rd("rt", "a\nb", 3, o, 8, &n); CHECK_EQ_I(n, 3); CHECK_EQ_I(o[1], '\n'); }
TC_CASE(lone_cr) { int o[8], n; rd("rt", "a\rb", 3, o, 8, &n); CHECK_EQ_I(n, 3); CHECK_EQ_I(o[0], 'a'); CHECK_EQ_I(o[1], '\r'); CHECK_EQ_I(o[2], 'b'); }
TC_CASE(double_crlf) { int o[8], n; rd("rt", "a\r\n\r\nb", 6, o, 8, &n); CHECK_EQ_I(n, 4); CHECK_EQ_I(o[0], 'a'); CHECK_EQ_I(o[1], '\n'); CHECK_EQ_I(o[2], '\n'); CHECK_EQ_I(o[3], 'b'); }
TC_CASE(binary_keeps_both) { int o[8], n; rd("rb", "a\r\nb", 4, o, 8, &n); CHECK_EQ_I(n, 4); CHECK_EQ_I(o[1], '\r'); CHECK_EQ_I(o[2], '\n'); CHECK_EQ_I(o[3], 'b'); }
int main(void)
{
    tc_begin();
    TC_RUN(crlf_to_lf); TC_RUN(lone_lf); TC_RUN(lone_cr); TC_RUN(double_crlf); TC_RUN(binary_keeps_both);
    DeleteFileA(path);
    return tc_end();
}
