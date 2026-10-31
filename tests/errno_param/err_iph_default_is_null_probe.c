/* TC
 * id:    err_iph_default_is_null_probe
 * area:  errno_param
 * funcs: _get_invalid_parameter_handler
 * crt:   >=100
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _get_invalid_parameter_handler
 * note:  PROBE: in a freshly started process (no user handler) the getter returns NULL.
 */
#include "tc.h"
#include <stdlib.h>
TC_CASE(initial) { CHECK(_get_invalid_parameter_handler() == NULL); }
int main(void) { tc_begin(); TC_RUN(initial); return tc_end(); }
