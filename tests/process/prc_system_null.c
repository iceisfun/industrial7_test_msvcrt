/* TC
 * id:    prc_system_null
 * area:  process
 * funcs: system
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.22.4.8 (system(NULL) nonzero iff a command processor exists); MS docs system
 */
#include "tc.h"
#include <stdlib.h>

TC_CASE(null_command) { const char *volatile c = NULL; CHECK(system(c) != 0); }

int main(void) { tc_begin(); TC_RUN(null_command); return tc_end(); }
