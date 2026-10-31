/* TC
 * id:    pfx_fprintf_wplus_readback
 * area:  stdio_printf
 * funcs: fprintf fopen fclose rewind fread fseek
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.5.3p7 (update streams need fflush/seek between write and read); MS docs fopen "w+"
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

#define TCS2(x) #x
#define TCS(x) TCS2(x)
#define FN "tc_pfx_fprintf_wplus_readback_" TCS(TC_CRT_VER) ".tmp"
TC_CASE(rewind_read)
{
    char b[32] = {0}; size_t n; FILE *f = fopen(FN, "w+"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(fprintf(f, "%s-%d", "abc", 7), 5);
    rewind(f);
    n = fread(b, 1, sizeof b - 1, f);
    CHECK_EQ_U(n, 5); CHECK_STR(b, "abc-7");
    fclose(f); remove(FN);
}
TC_CASE(fseek_read)
{
    char b[32] = {0}; size_t n; FILE *f = fopen(FN, "w+"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(fprintf(f, "%d%d%d", 1, 2, 3), 3);
    CHECK_EQ_I(fseek(f, 0, SEEK_SET), 0);
    n = fread(b, 1, sizeof b - 1, f);
    CHECK_EQ_U(n, 3); CHECK_STR(b, "123");
    fclose(f); remove(FN);
}
TC_CASE(binary_plus)
{
    char b[32] = {0}; size_t n; FILE *f = fopen(FN, "wb+"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(fprintf(f, "a\nb\r\n"), 5);
    rewind(f);
    n = fread(b, 1, sizeof b - 1, f);
    CHECK_EQ_U(n, 5); CHECK_MEM(b, "a\nb\r\n", 5);
    fclose(f); remove(FN);
}
TC_CASE(text_plus_newline_roundtrip)
{
    char b[32] = {0}; size_t n; FILE *f = fopen(FN, "w+"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(fprintf(f, "a\nb\n"), 4);
    rewind(f);
    n = fread(b, 1, sizeof b - 1, f);   /* CR-LF is translated back to LF on read in text mode */
    CHECK_EQ_U(n, 4); CHECK_MEM(b, "a\nb\n", 4);
    fclose(f); remove(FN);
}

int main(void)
{
    tc_begin();
    TC_RUN(rewind_read);
    TC_RUN(fseek_read);
    TC_RUN(binary_plus);
    TC_RUN(text_plus_newline_roundtrip);
    return tc_end();
}
