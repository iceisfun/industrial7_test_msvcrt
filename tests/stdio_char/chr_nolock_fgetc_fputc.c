/* TC
 * id:    chr_nolock_fgetc_fputc
 * area:  stdio_char
 * funcs:
 * procs: _fgetc_nolock _fputc_nolock _lock_file _unlock_file
 * crt:   win10
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _fgetc_nolock, _fputc_nolock (UCRT)
 * note:  procs because mingw's import libs are incomplete for older CRTs; ucrtbase only.
 */
#include "tc.h"
#include <stdio.h>

static void wr(const char *n, const void *d, size_t len)
{ FILE *f = fopen(n, "wb"); if (f) { if (len) fwrite(d, 1, len, f); fclose(f); } }
static size_t rd(const char *n, unsigned char *b, size_t max)
{ FILE *f = fopen(n, "rb"); size_t r = 0; if (f) { r = fread(b, 1, max, f); fclose(f); } return r; }
#define TC_S_(x) #x
#define TC_S(x) TC_S_(x)
#define FN "tc_chr_nolock_fgetc_fputc_" TC_S(TC_CRT_VER) ".tmp"
TC_DECL_PROC(int, _fgetc_nolock, (FILE *));
TC_DECL_PROC(int, _fputc_nolock, (int, FILE *));
TC_DECL_PROC(void, _lock_file, (FILE *));
TC_DECL_PROC(void, _unlock_file, (FILE *));

TC_CASE(exported)
{
    CHECK(tc_p__fgetc_nolock != NULL); CHECK(tc_p__fputc_nolock != NULL);
    CHECK(tc_p__lock_file != NULL); CHECK(tc_p__unlock_file != NULL);
}
TC_CASE(put_get)
{
    unsigned char b[8]; size_t n; FILE *f;
    if (!tc_p__fputc_nolock || !tc_p__fgetc_nolock || !tc_p__lock_file || !tc_p__unlock_file) TC_SKIP("exports missing (see exported)");
    f = fopen(FN, "wb"); CHECK(f != NULL); if (!f) return;
    tc_p__lock_file(f);
    CHECK_EQ_I(tc_p__fputc_nolock('p', f), 'p'); CHECK_EQ_I(tc_p__fputc_nolock('q', f), 'q'); CHECK_EQ_I(tc_p__fputc_nolock(0x80, f), 0x80);
    tc_p__unlock_file(f);
    fclose(f); n = rd(FN, b, 8); CHECK_EQ_U(n, 3); CHECK_MEM(b, "pq\x80", 3);
    f = fopen(FN, "rb"); CHECK(f != NULL); if (!f) return;
    tc_p__lock_file(f);
    CHECK_EQ_I(tc_p__fgetc_nolock(f), 'p'); CHECK_EQ_I(tc_p__fgetc_nolock(f), 'q'); CHECK_EQ_I(tc_p__fgetc_nolock(f), 0x80);
    CHECK_EQ_I(tc_p__fgetc_nolock(f), EOF);
    tc_p__unlock_file(f);
    fclose(f); remove(FN);
}
TC_CASE(text_crlf)
{
    unsigned char b[8]; size_t n; FILE *f;
    if (!tc_p__fputc_nolock || !tc_p__fgetc_nolock || !tc_p__lock_file || !tc_p__unlock_file) TC_SKIP("exports missing (see exported)");
    f = fopen(FN, "w"); CHECK(f != NULL); if (!f) return;
    tc_p__lock_file(f);
    CHECK_EQ_I(tc_p__fputc_nolock('a', f), 'a'); CHECK_EQ_I(tc_p__fputc_nolock('\n', f), '\n');
    tc_p__unlock_file(f);
    fclose(f); n = rd(FN, b, 8); CHECK_EQ_U(n, 3); CHECK_MEM(b, "a\r\n", 3);
    f = fopen(FN, "r"); CHECK(f != NULL); if (!f) return;
    tc_p__lock_file(f);
    CHECK_EQ_I(tc_p__fgetc_nolock(f), 'a'); CHECK_EQ_I(tc_p__fgetc_nolock(f), '\n'); CHECK_EQ_I(tc_p__fgetc_nolock(f), EOF);
    tc_p__unlock_file(f);
    fclose(f); remove(FN);
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(_fgetc_nolock); TC_LOAD_PROC(_fputc_nolock); TC_LOAD_PROC(_lock_file); TC_LOAD_PROC(_unlock_file);
    TC_RUN(exported); TC_RUN(put_get); TC_RUN(text_crlf); return tc_end();
}
