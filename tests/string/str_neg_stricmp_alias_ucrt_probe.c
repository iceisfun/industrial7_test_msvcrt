/* TC
 * id:    str_neg_stricmp_alias_ucrt_probe
 * area:  string
 * funcs:
 * procs: stricmp strnicmp
 * crt:   ucrtbase
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _stricmp
 * note:  PROBE: ucrtbase.dll is believed NOT to export the POSIX-name stricmp/strnicmp (oldnames.lib supplies them statically). Asserts both are absent; unverified.
 */
#include "tc.h"

TC_CASE(not_exported) { CHECK(tc_crt_proc("stricmp") == NULL); CHECK(tc_crt_proc("strnicmp") == NULL); }
int main(void) { tc_begin(); TC_RUN(not_exported); return tc_end(); }
