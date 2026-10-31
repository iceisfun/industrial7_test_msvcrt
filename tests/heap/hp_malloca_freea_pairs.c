/* TC
 * id:    hp_malloca_freea_pairs
 * area:  heap
 * funcs: malloc free
 * crt:   >=90
 * tier:  p2
 * conf:  documented
 * ref:   MS docs _malloca, _freea
 * note:  _malloca/_freea are inline header code (stack below _ALLOCA_S_THRESHOLD=1024, heap via malloc/free above), so only
 *        malloc/free are CRT imports. The test therefore mostly checks the header logic over the target CRT malloc/free.
 */
#include "tc.h"
#include <malloc.h>
#include <string.h>

TC_CASE(small)
{
    volatile size_t n = 16; unsigned char *p = (unsigned char *)_malloca(n); size_t i;
    CHECK(p != NULL);
    if (!p) return;
    for (i = 0; i < n; i++) p[i] = (unsigned char)i;
    CHECK_EQ_I(p[15], 15);
    _freea(p);
}
TC_CASE(large)
{
    volatile size_t n = 2000; unsigned char *p = (unsigned char *)_malloca(n); size_t i;
    CHECK(p != NULL);
    if (!p) return;
    for (i = 0; i < n; i++) p[i] = (unsigned char)(i * 3);
    CHECK_EQ_I(p[1999], (unsigned char)(1999 * 3));
    _freea(p);
}
TC_CASE(null) { _freea(NULL); }

int main(void) { tc_begin(); TC_RUN(small); TC_RUN(large); TC_RUN(null); return tc_end(); }
