/* TC
 * id:    ct_isascii_exported_probe
 * area:  ctype
 * funcs:
 * procs: __isascii isascii
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   MS docs isascii, __isascii
 * note:  PROBE: that the undecorated name `isascii` is an exported symbol of every CRT DLL (mingw import
 *        libs list it for all targets; not confirmed by documentation).
 */
#include "tc.h"
#include <ctype.h>

TC_CASE(isascii_exported) { CHECK(tc_crt_proc("isascii") != NULL); }
TC_CASE(__isascii_exported) { CHECK(tc_crt_proc("__isascii") != NULL); }

int main(void) { tc_begin(); TC_RUN(isascii_exported); TC_RUN(__isascii_exported); return tc_end(); }
