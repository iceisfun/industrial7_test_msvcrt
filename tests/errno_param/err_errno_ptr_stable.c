/* TC
 * id:    err_errno_ptr_stable
 * area:  errno_param
 * funcs: _errno strlen
 * crt:   all
 * tier:  p0
 * conf:  observed
 * ref:   none (MS docs do not state pointer stability or alignment)
 * note:  Within one thread _errno() returns the same address; int-aligned.
 */
#include "tc.h"
#include <errno.h>
#include <string.h>

TC_CASE(same) { int *a = _errno(), *b = _errno(); CHECK(a != NULL); CHECK_EQ_P(a, b); }
TC_CASE(after_calls) { int *a = _errno(); volatile const char *s = "abc"; CHECK_EQ_U(strlen((const char *)s), 3); CHECK_EQ_P(a, _errno()); }
TC_CASE(aligned) { CHECK_EQ_U(((ULONG_PTR)_errno()) & 3, 0); }
int main(void) { tc_begin(); TC_RUN(same); TC_RUN(after_calls); TC_RUN(aligned); return tc_end(); }
