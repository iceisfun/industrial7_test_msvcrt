/* TC
 * id:    hp_heapmin_ret
 * area:  heap
 * funcs: _heapmin malloc free
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _heapmin ("returns 0 if successful or -1 in the case of failure")
 */
#include "tc.h"
#include <malloc.h>

static volatile size_t v_n = 200;

TC_CASE(fresh) { CHECK_EQ_I(_heapmin(), 0); }
TC_CASE(after_free_many)
{
    void *p[200]; int i;
    for (i = 0; i < 200; i++) p[i] = malloc(v_n + i);
    for (i = 0; i < 200; i++) free(p[i]);
    CHECK_EQ_I(_heapmin(), 0);
}
TC_CASE(still_usable)
{
    char *p = (char *)malloc(1000); CHECK(p != NULL); if (!p) return;
    p[0] = 1; p[999] = 2;
    CHECK_EQ_I(_heapmin(), 0);
    CHECK(p[0] == 1 && p[999] == 2);
    free(p);
    p = (char *)malloc(5000); CHECK(p != NULL); if (p) { p[4999] = 3; free(p); }
}
int main(void) { tc_begin(); TC_RUN(fresh); TC_RUN(after_free_many); TC_RUN(still_usable); return tc_end(); }
