/* TC
 * id:    err_neg_malloc_huge
 * area:  errno_param
 * kind:  negative
 * funcs: malloc calloc
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs malloc, calloc: ENOMEM on insufficient memory
 * note:  crt narrowed to >=90: msvcrt.dll/msvcr80 errno-on-failure behaviour not certain.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

static volatile size_t v_n, v_m;

TC_CASE(malloc_max) { void *p; v_n = (size_t)-1; errno = 0; p = malloc(v_n); CHECK(p == NULL); CHECK_EQ_I(errno, ENOMEM); free(p); }
TC_CASE(calloc_overflow) { void *p; v_n = (size_t)-1; v_m = 2; errno = 0; p = calloc(v_n, v_m); CHECK(p == NULL); CHECK_EQ_I(errno, ENOMEM); free(p); }
TC_CASE(calloc_overflow2) { void *p; v_n = 2; v_m = (size_t)-1; errno = 0; p = calloc(v_n, v_m); CHECK(p == NULL); CHECK_EQ_I(errno, ENOMEM); free(p); }

int main(void) { tc_begin(); TC_RUN(malloc_max); TC_RUN(calloc_overflow); TC_RUN(calloc_overflow2); return tc_end(); }
