/* TC
 * id:    pfw_neg_snwprintf_s_count_gt_size
 * area:  stdio_printf
 * funcs:
 * procs: vswprintf_s _vsnwprintf_s _vswprintf_s_l
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _snprintf_s, _snprintf_s_l, _snwprintf_s (count > sizeOfBuffer, _TRUNCATE)
 */
#include "tc.h"
#include <wchar.h>
#include <errno.h>
#include <stdarg.h>
#if TC_CRT_VER >= 140
#define V_SW(b,n,f,ap)      vswprintf_s(b,n,f,ap)
#define V_SNW(b,n,c,f,ap)   _vsnwprintf_s(b,n,c,f,ap)
#define V_SWL(b,n,f,l,ap)   _vswprintf_s_l(b,n,f,l,ap)
#else
TC_DECL_PROC(int, vswprintf_s, (wchar_t *, size_t, const wchar_t *, va_list));
TC_DECL_PROC(int, _vsnwprintf_s, (wchar_t *, size_t, size_t, const wchar_t *, va_list));
TC_DECL_PROC(int, _vswprintf_s_l, (wchar_t *, size_t, const wchar_t *, void *, va_list));
#define V_SW(b,n,f,ap)      tc_p_vswprintf_s(b,n,f,ap)
#define V_SNW(b,n,c,f,ap)   tc_p__vsnwprintf_s(b,n,c,f,ap)
#define V_SWL(b,n,f,l,ap)   tc_p__vswprintf_s_l(b,n,f,l,ap)
#endif
static int w_swprintf_s(wchar_t *b, size_t n, const wchar_t *f, ...) { va_list ap; int r; va_start(ap, f); r = V_SW(b, n, f, ap); va_end(ap); return r; }
static int w_snwprintf_s(wchar_t *b, size_t n, size_t c, const wchar_t *f, ...) { va_list ap; int r; va_start(ap, f); r = V_SNW(b, n, c, f, ap); va_end(ap); return r; }
static int w_swprintf_s_l(wchar_t *b, size_t n, const wchar_t *f, void *l, ...) { va_list ap; int r; va_start(ap, l); r = V_SWL(b, n, f, l, ap); va_end(ap); return r; }

static const wchar_t *volatile f10 = L"0123456789";
static const wchar_t *volatile f3 = L"abc";

typedef struct { wchar_t g1[4]; wchar_t b[4]; wchar_t g2[4]; } G;
static void ginit(G *g) { int i; for (i = 0; i < 4; i++) { g->g1[i] = 0x5151; g->b[i] = 0x7777; g->g2[i] = 0x5252; } }
static int gok(const G *g) { int i, ok = 1; for (i = 0; i < 4; i++) if (g->g1[i] != 0x5151 || g->g2[i] != 0x5252) ok = 0; return ok; }

TC_CASE(count100_long)
{
    G g; int n; ginit(&g); errno = 0; tc_iph_count = 0;
    n = w_snwprintf_s(g.b, 4, 100, f10);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(g.b[0], 0); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(tc_iph_count, 1); CHECK(gok(&g));
}
TC_CASE(count100_fits)
{
    G g; int n; ginit(&g); errno = 0; tc_iph_count = 0;
    n = w_snwprintf_s(g.b, 4, 100, f3);
    CHECK_EQ_I(n, 3); CHECK_WSTR(g.b, L"abc"); CHECK_EQ_I(tc_iph_count, 0); CHECK(gok(&g));
}
TC_CASE(truncate_long)
{
    G g; int n; ginit(&g); errno = 0; tc_iph_count = 0;
    n = w_snwprintf_s(g.b, 4, (size_t)-1, f10);
    CHECK_EQ_I(n, -1); CHECK_WSTR(g.b, L"012"); CHECK_EQ_I(tc_iph_count, 0); CHECK(gok(&g));
}
int main(void)
{
    tc_begin(); tc_iph_install();
#if TC_CRT_VER < 140
    TC_LOAD_PROC(vswprintf_s); TC_LOAD_PROC(_vsnwprintf_s); TC_LOAD_PROC(_vswprintf_s_l);
    CHECK(tc_p_vswprintf_s && tc_p__vsnwprintf_s && tc_p__vswprintf_s_l);
    if (!tc_p_vswprintf_s || !tc_p__vsnwprintf_s || !tc_p__vswprintf_s_l) return tc_end();
#endif
    TC_RUN(count100_long); TC_RUN(count100_fits); TC_RUN(truncate_long);
    return tc_end();
}
