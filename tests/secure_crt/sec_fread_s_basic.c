/* TC
 * id:    sec_fread_s_basic
 * area:  secure_crt
 * funcs: fread_s fopen_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * conf:  documented
 * ref:   MS docs fread_s (returns number of full items read; may be less than count at EOF)
 */
#include "tc.h"
#include <stdio.h>

#define TC_STR2(x) #x
#define TC_STR(x) TC_STR2(x)
static const char *fname = "tc_sec_fread_s_basic_" TC_STR(TC_CRT_VER) ".tmp";
static void mk(void)
{
    FILE *f = NULL; volatile int one = 1; (void)one;
    if (fopen_s(&f, fname, "wb") == 0 && f) { fwrite("0123456789", 1, 10, f); fclose(f); }
}
TC_CASE(bufsize_equals_count_times_size)
{
    FILE *f = NULL; char b[8]; size_t n; mk();
    CHECK_EQ_I(fopen_s(&f, fname, "rb"), 0); if (!f) return;
    n = fread_s(b, 8, 2, 4, f);
    CHECK_EQ_U(n, 4); CHECK_MEM(b, "01234567", 8);
    fclose(f);
}
TC_CASE(bufsize_larger)
{
    FILE *f = NULL; char b[32]; size_t n; mk();
    CHECK_EQ_I(fopen_s(&f, fname, "rb"), 0); if (!f) return;
    n = fread_s(b, 32, 1, 5, f);
    CHECK_EQ_U(n, 5); CHECK_MEM(b, "01234", 5);
    fclose(f);
}
TC_CASE(short_read_at_eof_returns_items_read)
{
    FILE *f = NULL; char b[64]; size_t n; mk();
    CHECK_EQ_I(fopen_s(&f, fname, "rb"), 0); if (!f) return;
    n = fread_s(b, 64, 4, 8, f);   /* 10 bytes = 2 full items */
    CHECK_EQ_U(n, 2); CHECK_MEM(b, "01234567", 8);
    fclose(f);
}
TC_CASE(count_zero_returns_0)
{
    FILE *f = NULL; char b[8]; size_t n; mk();
    CHECK_EQ_I(fopen_s(&f, fname, "rb"), 0); if (!f) return;
    n = fread_s(b, 8, 1, 0, f);
    CHECK_EQ_U(n, 0);
    fclose(f);
}
int main(void)
{
    tc_begin(); TC_RUN(bufsize_equals_count_times_size); TC_RUN(bufsize_larger);
    TC_RUN(short_read_at_eof_returns_items_read); TC_RUN(count_zero_returns_0);
    remove(fname); return tc_end();
}
