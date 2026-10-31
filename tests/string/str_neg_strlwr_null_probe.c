/* TC
 * id:    str_neg_strlwr_null_probe
 * area:  string
 * funcs: _strlwr _strupr
 * procs: 
 * crt:   >=100
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _strlwr (no NULL behaviour documented)
 * note:  PROBE: _strlwr(NULL) and _strupr(NULL) return NULL, errno EINVAL, invalid parameter handler called. Believed from UCRT parameter validation; undocumented.
 */
#include "tc.h"
#include <string.h>
#include <errno.h>
static char *(*volatile p_lwr)(char *) = _strlwr;
static char *(*volatile p_upr)(char *) = _strupr;
TC_CASE(lwr_null) { char *r; tc_iph_count = 0; errno = 0; r = p_lwr(NULL); CHECK(r == NULL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(upr_null) { char *r; tc_iph_count = 0; errno = 0; r = p_upr(NULL); CHECK(r == NULL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
int main(void) { tc_begin(); if (!tc_iph_install()) return 3; TC_RUN(lwr_null); TC_RUN(upr_null); return tc_end(); }
