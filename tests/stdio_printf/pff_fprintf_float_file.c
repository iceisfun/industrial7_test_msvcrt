/* TC
 * id:    pff_fprintf_float_file
 * area:  stdio_printf
 * funcs: fprintf
 * crt:   all
 * tier:  p1
 * conf:  standard
 * ref:   C11 7.21.6.1, 7.21.2 (text mode); MS docs fprintf, fopen
 * note:  temp file tc_pff_fprintf_float_file_*.tmp in cwd, removed at the end
 */
#include "tc.h"
#include <stdio.h>

#define STR_(x) #x
#define STR(x) STR_(x)
#define F1 "tc_pff_fprintf_float_file_" STR(TC_CRT_VER) "_1.tmp"
#define F2 "tc_pff_fprintf_float_file_" STR(TC_CRT_VER) "_2.tmp"
TC_CASE(binary)
{
    FILE *fp; char b[32] = {0}; size_t got; volatile double a = 1.5, c = 2.5; int n;
    fp = fopen(F1, "wb"); CHECK(fp != NULL); if (!fp) return;
    n = fprintf(fp, "%.2f|%g\n", a, c); CHECK_EQ_I(n, 9); fclose(fp);
    fp = fopen(F1, "rb"); CHECK(fp != NULL); if (!fp) return;
    got = fread(b, 1, sizeof b - 1, fp); fclose(fp);
    CHECK_EQ_U(got, 9); CHECK_STR(b, "1.50|2.5\n");
}
TC_CASE(text_mode_crlf)
{
    FILE *fp; char b[32] = {0}; size_t got; volatile double a = 1.5, c = 2.5; int n;
    fp = fopen(F2, "w"); CHECK(fp != NULL); if (!fp) return;
    n = fprintf(fp, "%.2f|%g\n", a, c); CHECK_EQ_I(n, 9); fclose(fp);
    fp = fopen(F2, "rb"); CHECK(fp != NULL); if (!fp) return;
    got = fread(b, 1, sizeof b - 1, fp); fclose(fp);
    CHECK_EQ_U(got, 10); CHECK_STR(b, "1.50|2.5\r\n");
}
int main(void) { tc_begin(); TC_RUN(binary); TC_RUN(text_mode_crlf); remove(F1); remove(F2); return tc_end(); }
