/* TC
 * id:    pfu_fprintf_s_basic
 * area:  stdio_printf
 * funcs: fprintf_s fwprintf_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcrt
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fprintf_s, _fprintf_s_l, fwprintf_s, _fwprintf_s_l (return number of characters written)
 * note:  temp files tc_pfu_fprintf_s_basic_*.tmp, removed.
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

#define F1 "tc_pfu_fprintf_s_basic_a_" TC_CRT_DLL ".tmp"
#define F2 "tc_pfu_fprintf_s_basic_w_" TC_CRT_DLL ".tmp"

TC_CASE(narrow_count_and_content)
{
    FILE *fp = fopen(F1, "wb"); char buf[64]; size_t n; int r;
    CHECK(fp != NULL); if (!fp) return;
    r = fprintf_s(fp, "%d %s", 42, "abc");
    CHECK_EQ_I(r, 6);
    fclose(fp);
    fp = fopen(F1, "rb"); CHECK(fp != NULL); if (!fp) return;
    memset(buf, 0, sizeof buf); n = fread(buf, 1, sizeof buf - 1, fp); fclose(fp);
    CHECK_EQ_U(n, 6); CHECK_STR(buf, "42 abc");
}
TC_CASE(wide_count_and_content)
{
    FILE *fp = fopen(F2, "wb"); char buf[64]; size_t n; int r;
    CHECK(fp != NULL); if (!fp) return;
    r = fwprintf_s(fp, L"%d %ls", 12, L"ab");
    CHECK_EQ_I(r, 5);
    fclose(fp);
    fp = fopen(F2, "rb"); CHECK(fp != NULL); if (!fp) return;
    memset(buf, 0, sizeof buf); n = fread(buf, 1, sizeof buf - 1, fp); fclose(fp);
    CHECK(n >= 5);   /* narrow-converted (5 bytes) in default text/ANSI mode; do not assert the encoding */
}
TC_CASE(empty_format_returns_zero)
{
    FILE *fp = fopen(F1, "wb"); volatile const char *f = ""; volatile const wchar_t *wf = L""; int r;
    CHECK(fp != NULL); if (!fp) return;
    r = fprintf_s(fp, (const char *)f); CHECK_EQ_I(r, 0);
    r = fwprintf_s(fp, (const wchar_t *)wf); CHECK_EQ_I(r, 0);
    fclose(fp);
}
TC_CASE(zero_precision_string)
{
    FILE *fp = fopen(F1, "wb"); int r;
    CHECK(fp != NULL); if (!fp) return;
    r = fprintf_s(fp, "%.0s", "abc"); CHECK_EQ_I(r, 0);
    r = fwprintf_s(fp, L"%.0ls", L"abc"); CHECK_EQ_I(r, 0);
    fclose(fp);
}
int main(void)
{
    tc_begin();
    TC_RUN(narrow_count_and_content); TC_RUN(wide_count_and_content);
    TC_RUN(empty_format_returns_zero); TC_RUN(zero_precision_string);
    DeleteFileA(F1); DeleteFileA(F2);
    return tc_end();
}
