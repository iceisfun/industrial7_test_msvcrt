/* TC
 * id:    hp_neg_calloc_overflow
 * area:  heap
 * funcs: calloc
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs calloc (ENOMEM); C11 7.22.3.2
 * note:  msvcr80+ only: the VC6-era msvcrt.dll may lack the multiplication-overflow check.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>

static volatile size_t v_a, v_b;

static void try_calloc(size_t a, size_t b, int line)
{
    void *p; errno = 0; v_a = a; v_b = b; p = calloc(v_a, v_b);
    if (p != NULL || errno != ENOMEM) tc_fail_ints(__FILE__, line, "calloc(a,b)!=NULL || errno", "ENOMEM", p != NULL ? 1 : errno, ENOMEM);
    if (p) free(p);
}
TC_CASE(max_2) { try_calloc(SIZE_MAX, 2, __LINE__); }
TC_CASE(half_plus_1) { try_calloc(SIZE_MAX / 2 + 1, 2, __LINE__); try_calloc(2, SIZE_MAX / 2 + 1, __LINE__); }
TC_CASE(max_max) { try_calloc(SIZE_MAX, SIZE_MAX, __LINE__); }
int main(void) { tc_begin(); TC_RUN(max_2); TC_RUN(half_plus_1); TC_RUN(max_max); return tc_end(); }
