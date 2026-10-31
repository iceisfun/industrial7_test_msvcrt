/* TC
 * id:    dir_neg_findclose_invalid
 * area:  dir
 * kind:  negative
 * funcs: _findclose
 * crt:   >=80
 * tier:  p1
 * conf:  inferred
 * ref:   MS docs _findclose: returns 0 on success, -1 on failure (that an invalid handle fails is inferred from FindClose)
 * note:  errno after the failure is left to dir_neg_probe_findclose_errno; double close is in dir_probe_findclose_double.
 */
#include "tc.h"
#include <io.h>
#include <errno.h>

TC_CASE(minus_one) { CHECK_EQ_I(_findclose((intptr_t)-1), -1); }
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(minus_one); return tc_end(); }
