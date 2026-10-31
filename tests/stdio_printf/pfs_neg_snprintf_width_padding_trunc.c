/* TC
 * id:    pfs_neg_snprintf_width_padding_trunc
 * area:  stdio_printf
 * funcs: _snprintf _snwprintf
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _snprintf/_snwprintf: if len > count, count characters are written and -1 returned; if len == count, no terminating NUL is appended
 * note:  Width padding drives the output past count; nothing may be written beyond count (sentinels). Content of the first count chars is the space padding.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static void fill(unsigned char *b) { int i; for (i = 0; i < 16; i++) b[i] = 0x55; }
static void wfill(wchar_t *b) { int i; for (i = 0; i < 16; i++) b[i] = 0x5555; }
static const char *volatile f_10s = "%10s";
static const char *volatile f_m10s = "%-10s|";
static const char *volatile f_6s = "%6s";
static const wchar_t *volatile wf_10s = L"%10s";
static const wchar_t *volatile wf_m10s = L"%-10s|";
static const wchar_t *volatile wf_6s = L"%6s";

TC_CASE(right_pad_overflow)
{
    unsigned char b[16]; int n; fill(b);
    n = _snprintf((char *)b, 6, f_10s, "ab");
    CHECK_EQ_I(n, -1);
    CHECK_MEM(b, "      ", 6);
    CHECK(b[6] == 0x55 && b[7] == 0x55);
}
TC_CASE(left_pad_overflow)
{
    unsigned char b[16]; int n; fill(b);
    n = _snprintf((char *)b, 6, f_m10s, "ab");
    CHECK_EQ_I(n, -1);
    CHECK_MEM(b, "ab    ", 6);
    CHECK(b[6] == 0x55 && b[7] == 0x55);
}
TC_CASE(exact_fit_and_one_over)
{
    unsigned char b[16]; int n; fill(b);
    n = _snprintf((char *)b, 6, f_6s, "abcdef");
    CHECK_EQ_I(n, 6); CHECK_MEM(b, "abcdef", 6); CHECK(b[6] == 0x55);
    fill(b);
    n = _snprintf((char *)b, 6, f_6s, "abcdefg");
    CHECK_EQ_I(n, -1); CHECK_MEM(b, "abcdef", 6); CHECK(b[6] == 0x55);
}
TC_CASE(wide_right_pad)
{
    wchar_t w[16]; int n, i; wfill(w);
    n = _snwprintf(w, 6, wf_10s, L"ab");
    CHECK_EQ_I(n, -1);
    for (i = 0; i < 6; i++) CHECK(w[i] == L' ');
    CHECK(w[6] == 0x5555 && w[7] == 0x5555);
}
TC_CASE(wide_left_pad)
{
    wchar_t w[16]; int n; wfill(w);
    n = _snwprintf(w, 6, wf_m10s, L"ab");
    CHECK_EQ_I(n, -1);
    CHECK(w[0] == L'a' && w[1] == L'b' && w[2] == L' ' && w[5] == L' ');
    CHECK(w[6] == 0x5555 && w[7] == 0x5555);
}
TC_CASE(wide_exact_fit)
{
    wchar_t w[16]; int n; wfill(w);
    n = _snwprintf(w, 6, wf_6s, L"abcdef");
    CHECK_EQ_I(n, 6); CHECK(w[0] == L'a' && w[5] == L'f'); CHECK(w[6] == 0x5555);
    wfill(w);
    n = _snwprintf(w, 6, wf_6s, L"abcdefg");
    CHECK_EQ_I(n, -1); CHECK(w[5] == L'f'); CHECK(w[6] == 0x5555);
}
int main(void)
{
    tc_begin();
    TC_RUN(right_pad_overflow); TC_RUN(left_pad_overflow); TC_RUN(exact_fit_and_one_over);
    TC_RUN(wide_right_pad); TC_RUN(wide_left_pad); TC_RUN(wide_exact_fit);
    return tc_end();
}
