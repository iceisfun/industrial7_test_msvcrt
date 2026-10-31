/* TC
 * id:    str_neg_strtok_notoken_then_null
 * area:  string
 * funcs: strtok
 * crt:   all
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.24.5.8 (saved state after a no-token first call is unspecified)
 * note:  PROBE: after strtok(s, d) returns NULL because s holds only delimiters, a following
 *        strtok(NULL, d) also returns NULL (saved pointer left at end of string). Not guaranteed by ISO C.
 */
#include "tc.h"
#include <string.h>

TC_CASE(only_delims_then_null) { char b[] = ",,,;;"; CHECK(strtok(b, ",;") == NULL); CHECK(strtok(NULL, ",;") == NULL); }

int main(void) { tc_begin(); TC_RUN(only_delims_then_null); return tc_end(); }
