/* TC
 * id:    hp_stress_realloc_chain
 * area:  heap
 * funcs: realloc free
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.22.3.5; MS docs realloc
 */
#include "tc.h"
#include <stdlib.h>

static volatile size_t v_size;
static unsigned char pat(size_t i) { return (unsigned char)(i * 31u + 7u); }

TC_CASE(chain)
{
    unsigned char *p = NULL; size_t cur = 0, i; unsigned long long s = 12345;
    int step, bad = 0, nullret = 0;
    for (step = 0; step < 200; step++) {
        size_t n, keep; unsigned char *q;
        s = s * 6364136223846793005ULL + 1442695040888963407ULL;
        n = (size_t)((s >> 33) % 70000u) + 1;
        v_size = n;
        q = (unsigned char *)realloc(p, v_size);
        if (!q) { nullret++; break; }
        p = q;
        keep = cur < n ? cur : n;
        for (i = 0; i < keep; i++) if (p[i] != pat(i)) { bad++; break; }
        for (i = 0; i < n; i++) p[i] = pat(i);
        cur = n;
    }
    CHECK_EQ_I(nullret, 0);
    CHECK_EQ_I(bad, 0);
    free(p);
}
TC_CASE(shrink_to_one_and_back)
{
    unsigned char *p; v_size = 70000;
    p = (unsigned char *)realloc(NULL, v_size);
    CHECK(p != NULL);
    if (!p) return;
    p[0] = 0x5a; v_size = 1; p = (unsigned char *)realloc(p, v_size);
    CHECK(p != NULL);
    if (p) { CHECK_EQ_U(p[0], 0x5a); v_size = 65000; p = (unsigned char *)realloc(p, v_size); CHECK(p != NULL); if (p) CHECK_EQ_U(p[0], 0x5a); }
    free(p);
}
int main(void) { tc_begin(); TC_RUN(chain); TC_RUN(shrink_to_one_and_back); return tc_end(); }
