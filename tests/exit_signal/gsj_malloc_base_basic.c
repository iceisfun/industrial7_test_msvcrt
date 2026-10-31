/* TC
 * id:    gsj_malloc_base_basic
 * area:  exit_signal
 * funcs:
 * procs: _malloc_base _free_base _msize
 * crt:   ucrtbase
 * tier:  p0
 * conf:  inferred
 * ref:   MS docs malloc (size 0 yields a valid pointer; storage suitably aligned); _malloc_base/_free_base are undocumented ucrtbase exports
 * note:  _malloc_base is undocumented; expectations carry over from the documented malloc/free/_msize contract.
 *        Alignment: 16 on 64-bit, 8 on 32-bit.
 */
#include "tc.h"

typedef void *(__cdecl *malloc_t)(size_t);
typedef void (__cdecl *free_t)(void *);
typedef size_t (__cdecl *msize_t)(void *);
static malloc_t pm; static free_t pf; static msize_t ps;

TC_CASE(one_byte) { unsigned char *p; CHECK(pm && pf); if (!pm || !pf) return; p = pm(1); CHECK(p != NULL); if (p) { p[0] = 0xAB; CHECK_EQ_U(p[0], 0xAB); pf(p); } }
TC_CASE(zero_size) { void *p; if (!pm || !pf) return; p = pm(0); CHECK(p != NULL); pf(p); }
TC_CASE(page) { unsigned char *p; size_t i; if (!pm || !pf || !ps) return; p = pm(4096); CHECK(p != NULL); if (!p) return;
    for (i = 0; i < 4096; i++) p[i] = (unsigned char)i;
    CHECK(ps(p) >= 4096); for (i = 0; i < 4096; i++) if (p[i] != (unsigned char)i) { CHECK(0); break; }
    pf(p); }
TC_CASE(alignment) { size_t n; if (!pm || !pf) return; for (n = 1; n <= 64; n++) { void *p = pm(n); CHECK(p != NULL); CHECK_EQ_U((ULONG_PTR)p & (sizeof(void *) == 8 ? 15 : 7), 0); pf(p); } }
TC_CASE(distinct) { unsigned char *a, *b; if (!pm || !pf) return; a = pm(100); b = pm(100); CHECK(a && b && a != b);
    if (a && b) { size_t i; for (i = 0; i < 100; i++) { a[i] = 1; b[i] = 2; } for (i = 0; i < 100; i++) if (a[i] != 1 || b[i] != 2) { CHECK(0); break; } }
    pf(a); pf(b); }
int main(void)
{
    tc_begin();
    pm = (malloc_t)tc_crt_proc("_malloc_base"); pf = (free_t)tc_crt_proc("_free_base"); ps = (msize_t)tc_crt_proc("_msize");
    TC_RUN(one_byte); TC_RUN(zero_size); TC_RUN(page); TC_RUN(alignment); TC_RUN(distinct);
    return tc_end();
}
