/* TC
 * id:    pfq_printf_p_redirected
 * area:  stdio_printf
 * funcs: _printf_p _vprintf_p
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs printf_p, _printf_p_l, _vprintf_p (positional parameters); freopen
 * note:  stdout is redirected once with freopen to a temp file. freopen also replaces the process STD_OUTPUT_HANDLE,
 *        which the kernel32-based harness uses, so a duplicate of the original handle is saved and restored with SetStdHandle.
 *        The file is read back while stdout is still open (a new fd 1 would otherwise rebind the std handle).
 *        _wprintf_p is not tested (stream orientation).
 */
#include "tc.h"
#include "pfq_shim.h"

#define XS_(x) #x
#define XS(x) XS_(x)
#define FN "tc_pfq_printf_p_" XS(TC_CRT_VER) ".tmp"

static int vp(const char *fmt, ...)
{ va_list ap; int r; va_start(ap, fmt); r = pq_vprintf_p(fmt, ap); va_end(ap); return r; }

static HANDLE g_dup;
static int g_ok, r_printf, r_vprintf;

TC_CASE(redirect)
{
    HANDLE orig = GetStdHandle(STD_OUTPUT_HANDLE);
    DuplicateHandle(GetCurrentProcess(), orig, GetCurrentProcess(), &g_dup, 0, FALSE, DUPLICATE_SAME_ACCESS);
    g_ok = freopen(FN, "wb", stdout) != NULL;
    if (g_dup) SetStdHandle(STD_OUTPUT_HANDLE, g_dup);
    CHECK(g_ok);
}
TC_CASE(printf_p)
{
    if (!g_ok) TC_SKIP("no redirect");
    r_printf = pq_printf_p("%2$d %1$d;", 1, 2);
    fflush(stdout);
}
TC_CASE(vprintf_p)
{
    if (!g_ok) TC_SKIP("no redirect");
    r_vprintf = vp("%3$s:%1$d:%2$d", 10, 20, "go");
    fflush(stdout);
}
TC_CASE(verify)
{
    char b[64]; size_t n = 0; FILE *f;
    if (!g_ok) TC_SKIP("no redirect");
    f = fopen(FN, "rb");
    CHECK(f != NULL);
    if (f) { n = fread(b, 1, sizeof b - 1, f); fclose(f); }
    b[n] = 0;
    CHECK_EQ_I(r_printf, 4);
    CHECK_EQ_I(r_vprintf, 8);
    CHECK_EQ_U(n, 12);
    CHECK_STR(b, "2 1;go:10:20");
    fclose(stdout);
    if (g_dup) SetStdHandle(STD_OUTPUT_HANDLE, g_dup); /* closing fd 1 resets the std handle */
    CHECK_EQ_I(remove(FN), 0);
}
int main(void)
{
    tc_begin();
    TC_RUN(redirect); TC_RUN(printf_p); TC_RUN(vprintf_p); TC_RUN(verify);
    return tc_end();
}
