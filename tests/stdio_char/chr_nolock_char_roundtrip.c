/* TC
 * id:    chr_nolock_char_roundtrip
 * area:  stdio_char
 * funcs: _lock_file _unlock_file
 * procs: _putc_nolock _getc_nolock _ungetc_nolock
 * crt:   win10
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _getc_nolock, _putc_nolock, _ungetc_nolock, _lock_file, _unlock_file
 * note:  resolved at run time: the MS headers define _putc_nolock/_getc_nolock as macros and older CRTs may not export them;
 *        ucrtbase exports them.
 */
#include "tc.h"
#include <stdio.h>

static void wr(const char *n, const void *d, size_t len)
{ FILE *f = fopen(n, "wb"); if (f) { if (len) fwrite(d, 1, len, f); fclose(f); } }
static size_t rd(const char *n, unsigned char *b, size_t max)
{ FILE *f = fopen(n, "rb"); size_t r = 0; if (f) { r = fread(b, 1, max, f); fclose(f); } return r; }
#define TC_S_(x) #x
#define TC_S(x) TC_S_(x)
#define FN "tc_chr_nolock_char_roundtrip_" TC_S(TC_CRT_VER) ".tmp"
TC_DECL_PROC(int, _putc_nolock, (int, FILE *));
TC_DECL_PROC(int, _getc_nolock, (FILE *));
TC_DECL_PROC(int, _ungetc_nolock, (int, FILE *));

TC_CASE(put_binary)
{
    unsigned char b[8]; size_t n; FILE *f;
    CHECK(tc_p__putc_nolock != NULL); if (!tc_p__putc_nolock) return;
    f = fopen(FN, "wb"); CHECK(f != NULL); if (!f) return;
    _lock_file(f);
    CHECK_EQ_I(tc_p__putc_nolock('x', f), 'x'); CHECK_EQ_I(tc_p__putc_nolock('y', f), 'y'); CHECK_EQ_I(tc_p__putc_nolock(0xFF, f), 0xFF);
    _unlock_file(f);
    fclose(f); n = rd(FN, b, 8); CHECK_EQ_U(n, 3); CHECK_MEM(b, "xy\xff", 3); remove(FN);
}
TC_CASE(get_binary)
{
    FILE *f; wr(FN, "xyz", 3);
    CHECK(tc_p__getc_nolock != NULL); if (!tc_p__getc_nolock) return;
    f = fopen(FN, "rb"); CHECK(f != NULL); if (!f) return;
    _lock_file(f);
    CHECK_EQ_I(tc_p__getc_nolock(f), 'x'); CHECK_EQ_I(tc_p__getc_nolock(f), 'y'); CHECK_EQ_I(tc_p__getc_nolock(f), 'z');
    CHECK_EQ_I(tc_p__getc_nolock(f), EOF);
    _unlock_file(f);
    CHECK(feof(f) != 0);
    fclose(f); remove(FN);
}
TC_CASE(unget)
{
    FILE *f; wr(FN, "xy", 2);
    CHECK(tc_p__getc_nolock != NULL && tc_p__ungetc_nolock != NULL); if (!tc_p__getc_nolock || !tc_p__ungetc_nolock) return;
    f = fopen(FN, "rb"); CHECK(f != NULL); if (!f) return;
    _lock_file(f);
    CHECK_EQ_I(tc_p__getc_nolock(f), 'x');
    CHECK_EQ_I(tc_p__ungetc_nolock('x', f), 'x'); CHECK_EQ_I(tc_p__getc_nolock(f), 'x');
    CHECK_EQ_I(tc_p__ungetc_nolock('Q', f), 'Q'); CHECK_EQ_I(tc_p__getc_nolock(f), 'Q'); CHECK_EQ_I(tc_p__getc_nolock(f), 'y');
    _unlock_file(f);
    fclose(f); remove(FN);
}
TC_CASE(text_newline)
{
    unsigned char b[8]; size_t n; FILE *f;
    CHECK(tc_p__putc_nolock != NULL); if (!tc_p__putc_nolock) return;
    f = fopen(FN, "w"); CHECK(f != NULL); if (!f) return;
    _lock_file(f);
    CHECK_EQ_I(tc_p__putc_nolock('a', f), 'a'); CHECK_EQ_I(tc_p__putc_nolock('\n', f), '\n'); CHECK_EQ_I(tc_p__putc_nolock('b', f), 'b');
    _unlock_file(f);
    fclose(f); n = rd(FN, b, 8); CHECK_EQ_U(n, 4); CHECK_MEM(b, "a\r\nb", 4); remove(FN);
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(_putc_nolock); TC_LOAD_PROC(_getc_nolock); TC_LOAD_PROC(_ungetc_nolock);
    TC_RUN(put_binary); TC_RUN(get_binary); TC_RUN(unget); TC_RUN(text_newline); return tc_end();
}
