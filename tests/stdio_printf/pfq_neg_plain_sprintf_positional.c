/* TC
 * id:    pfq_neg_plain_sprintf_positional
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf fprintf _snwprintf
 * crt:   >=90
 * tier:  p0
 * conf:  probe
 * ref:   MS docs printf_p positional parameters: positional arguments are provided by the _p variants only
 * note:  PROBES that the plain (non-_p) family does not honour "%2$d %1$d" as a reordering. Only "output is not '2 1'" is asserted;
 *        the exact behaviour (invalid parameter / -1 / literal text) is unspecified. An invalid-parameter handler is installed so
 *        that a rejection cannot terminate the process.
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

static int neq(const char *a, const char *b) { return !tc_strequ(a, b); }

TC_CASE(sprintf_not_reordered)
{
    char b[64]; const char * volatile f = "%2$d %1$d"; int r;
    b[0] = 0;
    r = sprintf(b, f, 1, 2);
    CHECK(neq(b, "2 1"));
    (void)r;
}
TC_CASE(snwprintf_not_reordered)
{
    wchar_t b[64]; const wchar_t * volatile f = L"%2$d %1$d"; int r;
    b[0] = 0;
    r = _snwprintf(b, 63, f, 1, 2);
    CHECK(!tc_wcsequ(b, L"2 1"));
    (void)r;
}
TC_CASE(fprintf_not_reordered)
{
    FILE *fp; char buf[64]; size_t n = 0; const char * volatile f = "%2$d %1$d";
    char name[64] = "tc_pfq_neg_plain_sprintf_positional_"; char num[24]; size_t k = 0;
    tc_i64(TC_CRT_VER, num); while (name[k]) k++; { size_t j = 0; while (num[j]) name[k++] = num[j++]; } name[k++] = '.'; name[k++] = 't'; name[k++] = 'm'; name[k++] = 'p'; name[k] = 0;
    fp = fopen(name, "wb");
    CHECK(fp != NULL);
    if (!fp) return;
    fprintf(fp, f, 1, 2);
    fclose(fp);
    fp = fopen(name, "rb");
    if (fp) { n = fread(buf, 1, sizeof buf - 1, fp); fclose(fp); }
    buf[n] = 0;
    remove(name);
    CHECK(neq(buf, "2 1"));
}
TC_CASE(single_positional_no_crash)
{
    char b[64]; const char * volatile f = "%1$d"; volatile int r;
    b[0] = 0;
    r = sprintf(b, f, 5);
    (void)r;
    CHECK(1);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(sprintf_not_reordered); TC_RUN(snwprintf_not_reordered); TC_RUN(fprintf_not_reordered); TC_RUN(single_positional_no_crash);
    return tc_end();
}
