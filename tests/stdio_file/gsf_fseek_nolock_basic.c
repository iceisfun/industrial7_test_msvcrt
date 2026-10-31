/* TC
 * id:    gsf_fseek_nolock_basic
 * area:  stdio_file
 * funcs: _fseek_nolock _ftelli64_nolock fopen fwrite fgetc feof fclose
 * crt:   >=80
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _fseek_nolock, fseek, _ftelli64
 * note:  Binary mode, so no text-mode seek restrictions.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>
int __cdecl _fseek_nolock(FILE *, long, int);
__int64 __cdecl _ftelli64_nolock(FILE *);

#define TC_S2(x) #x
#define TC_S1(x) TC_S2(x)
static void mk(const char *n, const char *d, size_t len)
{
    FILE *f = fopen(n, "wb");
    if (f) { fwrite(d, 1, len, f); fclose(f); }
}
static const char *fn = "tc_gsf_fseek_nolock_basic_" TC_S1(TC_CRT_VER) ".tmp";
static volatile int vorg = SEEK_SET;
static volatile long vpos;

TC_CASE(seek_set_cur_end)
{
    FILE *fp; mk(fn, "0123456789", 10);
    fp = fopen(fn, "rb"); CHECK(fp != NULL); if (!fp) return;
    vpos = 4; vorg = SEEK_SET;
    CHECK_EQ_I(_fseek_nolock(fp, vpos, vorg), 0);
    CHECK_EQ_I(fgetc(fp), 0x34);
    vpos = 2; vorg = SEEK_CUR;      /* position is 5 -> 7 */
    CHECK_EQ_I(_fseek_nolock(fp, vpos, vorg), 0);
    CHECK_EQ_I(_ftelli64_nolock(fp), 7);
    CHECK_EQ_I(fgetc(fp), 0x37);
    vpos = -1; vorg = SEEK_END;
    CHECK_EQ_I(_fseek_nolock(fp, vpos, vorg), 0);
    CHECK_EQ_I(fgetc(fp), 0x39);
    fclose(fp); remove(fn);
}
TC_CASE(clears_eof)
{
    FILE *fp; mk(fn, "0123456789", 10);
    fp = fopen(fn, "rb"); CHECK(fp != NULL); if (!fp) return;
    vpos = 0; vorg = SEEK_END;
    CHECK_EQ_I(_fseek_nolock(fp, vpos, vorg), 0);
    CHECK_EQ_I(fgetc(fp), EOF);
    CHECK(feof(fp) != 0);
    vorg = SEEK_CUR;
    CHECK_EQ_I(_fseek_nolock(fp, vpos, vorg), 0);
    CHECK_EQ_I(feof(fp), 0);
    fclose(fp); remove(fn);
}
TC_CASE(seek_past_end_binary)
{
    FILE *fp; mk(fn, "0123456789", 10);
    fp = fopen(fn, "rb"); CHECK(fp != NULL); if (!fp) return;
    vpos = 20; vorg = SEEK_SET;
    CHECK_EQ_I(_fseek_nolock(fp, vpos, vorg), 0);
    CHECK_EQ_I(fgetc(fp), EOF);
    fclose(fp); remove(fn);
}
TC_CASE(seek_past_end_then_write_extends)
{
    FILE *fp, *g; long n = 0;
    mk(fn, "0123456789", 10);
    fp = fopen(fn, "r+b"); CHECK(fp != NULL); if (!fp) return;
    vpos = 14; vorg = SEEK_SET;
    CHECK_EQ_I(_fseek_nolock(fp, vpos, vorg), 0);
    CHECK_EQ_U(fwrite("Z", 1, 1, fp), 1);
    fclose(fp);
    g = fopen(fn, "rb"); CHECK(g != NULL); if (!g) { remove(fn); return; }
    fseek(g, 0, SEEK_END); n = ftell(g);
    CHECK_EQ_I(n, 15);
    fclose(g); remove(fn);
}
int main(void) { tc_begin(); TC_RUN(seek_set_cur_end); TC_RUN(clears_eof); TC_RUN(seek_past_end_binary); TC_RUN(seek_past_end_then_write_extends); return tc_end(); }
