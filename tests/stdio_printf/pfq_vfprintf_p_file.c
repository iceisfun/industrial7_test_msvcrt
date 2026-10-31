/* TC
 * id:    pfq_vfprintf_p_file
 * area:  stdio_printf
 * funcs: _vfprintf_p _vfwprintf_p
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs vprintf_p functions (_vfprintf_p, _vfwprintf_p), printf_p positional parameters
 */
#include "tc.h"
#include "pfq_shim.h"

#define XS_(x) #x
#define XS(x) XS_(x)
#define FN "tc_pfq_vfprintf_p_file_" XS(TC_CRT_VER) ".tmp"

static size_t slurp(char *buf, size_t cap)
{
    FILE *f = fopen(FN, "rb"); size_t n = 0;
    if (f) { n = fread(buf, 1, cap - 1, f); fclose(f); }
    buf[n] = 0; return n;
}
static int vf(FILE *f, const char *fmt, ...)
{ va_list ap; int r; va_start(ap, fmt); r = pq_vfprintf_p(f, fmt, ap); va_end(ap); return r; }
static int vfw(FILE *f, const wchar_t *fmt, ...)
{ va_list ap; int r; va_start(ap, fmt); r = pq_vfwprintf_p(f, fmt, ap); va_end(ap); return r; }

TC_CASE(narrow)
{
    char b[64]; FILE *f = fopen(FN, "wb"); int r;
    CHECK(f != NULL); if (!f) return;
    r = vf(f, "%2$s=%1$d", 12, "key");
    fclose(f);
    CHECK_EQ_I(r, 6);
    CHECK_EQ_U(slurp(b, sizeof b), 6);
    CHECK_STR(b, "key=12");
}
TC_CASE(wide)
{
    char b[64]; FILE *f = fopen(FN, "w"); /* text mode: ANSI-mode wide stream converts to narrow */ int r;
    CHECK(f != NULL); if (!f) return;
    r = vfw(f, L"%2$ls=%1$d", 12, L"key");
    fclose(f);
    CHECK_EQ_I(r, 6);
    slurp(b, sizeof b);
    CHECK_STR(b, "key=12");
}
TC_CASE(star_positions)
{
    char b[64]; FILE *f = fopen(FN, "wb"); int r;
    CHECK(f != NULL); if (!f) return;
    r = vf(f, "[%1$*2$d]", 7, 5);
    fclose(f);
    CHECK_EQ_I(r, 7);
    slurp(b, sizeof b);
    CHECK_STR(b, "[    7]");
}
TC_CASE(cleanup)
{
    CHECK_EQ_I(remove(FN), 0);
}
int main(void)
{
    tc_begin();
    TC_RUN(narrow); TC_RUN(wide); TC_RUN(star_positions); TC_RUN(cleanup);
    return tc_end();
}
