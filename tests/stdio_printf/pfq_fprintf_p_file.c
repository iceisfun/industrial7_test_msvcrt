/* TC
 * id:    pfq_fprintf_p_file
 * area:  stdio_printf
 * funcs: _fprintf_p _fwprintf_p
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs printf_p, _printf_p_l, _fprintf_p, _fwprintf_p (positional parameters)
 * note:  ucrtbase: _fprintf_p/_fwprintf_p are SDK inlines over __stdio_common_vfprintf_p / vfwprintf_p. Wrappers in include/pfq_shim.h.
 */
#include "tc.h"
#include "pfq_shim.h"

#define XS_(x) #x
#define XS(x) XS_(x)
#define FN "tc_pfq_fprintf_p_file_" XS(TC_CRT_VER) ".tmp"

static size_t slurp(char *buf, size_t cap)
{
    FILE *f = fopen(FN, "rb"); size_t n = 0;
    if (f) { n = fread(buf, 1, cap - 1, f); fclose(f); }
    buf[n] = 0; return n;
}

TC_CASE(reorder)
{
    char b[64]; FILE *f = fopen(FN, "wb"); int r;
    CHECK(f != NULL); if (!f) return;
    r = pq_fprintf_p(f, "%2$s %1$d", 5, "x");
    fclose(f);
    CHECK_EQ_I(r, 3);
    CHECK_EQ_U(slurp(b, sizeof b), 3);
    CHECK_STR(b, "x 5");
}
TC_CASE(retval_matches_bytes)
{
    char b[64]; FILE *f = fopen(FN, "wb"); int r;
    CHECK(f != NULL); if (!f) return;
    r = pq_fprintf_p(f, "[%3$s|%1$05d|%2$c]", 42, 'z', "str");
    fclose(f);
    CHECK_EQ_I(r, 13);
    CHECK_EQ_U(slurp(b, sizeof b), 13);
    CHECK_STR(b, "[str|00042|z]");
}
TC_CASE(reuse)
{
    char b[64]; FILE *f = fopen(FN, "wb"); int r;
    CHECK(f != NULL); if (!f) return;
    r = pq_fprintf_p(f, "%1$s%1$s", "ab");
    fclose(f);
    CHECK_EQ_I(r, 4);
    slurp(b, sizeof b);
    CHECK_STR(b, "abab");
}
TC_CASE(wide_to_ansi_stream)
{
    char b[64]; FILE *f = fopen(FN, "w"); /* text mode: converted to narrow */ int r;
    CHECK(f != NULL); if (!f) return;
    r = pq_fwprintf_p(f, L"%2$ls-%1$d", 9, L"wide");
    fclose(f);
    CHECK_EQ_I(r, 6);
    slurp(b, sizeof b);
    CHECK_STR(b, "wide-9");
}
TC_CASE(cleanup)
{
    CHECK_EQ_I(remove(FN), 0);
    CHECK(fopen(FN, "rb") == NULL);
}
int main(void)
{
    tc_begin();
    TC_RUN(reorder); TC_RUN(retval_matches_bytes); TC_RUN(reuse); TC_RUN(wide_to_ansi_stream); TC_RUN(cleanup);
    return tc_end();
}
