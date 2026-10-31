/* TC
 * id:    hp_neg_realloc_null_huge
 * area:  heap
 * funcs: realloc
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs realloc; C11 7.22.3.5 (realloc(NULL,n) == malloc(n))
 * note:  errno=ENOMEM only asserted on ucrtbase.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>

static volatile size_t v_n;
TC_CASE(null_max) { void *p; errno = 0; v_n = SIZE_MAX; p = realloc(NULL, v_n); CHECK(p == NULL); if (TC_AT_LEAST(140)) CHECK_EQ_I(errno, ENOMEM); if (p) free(p); }
int main(void) { tc_begin(); TC_RUN(null_max); return tc_end(); }
