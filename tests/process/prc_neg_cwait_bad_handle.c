/* TC
 * id:    prc_neg_cwait_bad_handle
 * area:  process
 * funcs: _cwait
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _cwait (returns -1 on error)
 */
#include "tc.h"
#include <process.h>

TC_CASE(invalid_handle) { int st = 12345; CHECK_EQ_I(_cwait(&st, (intptr_t)0x7ffffff4, 0), -1); }
TC_CASE(null_handle)    { int st = 0; CHECK_EQ_I(_cwait(&st, 0, 0), -1); }
int main(void) { tc_begin(); TC_RUN(invalid_handle); TC_RUN(null_handle); return tc_end(); }
