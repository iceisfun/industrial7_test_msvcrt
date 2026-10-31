/* TC
 * id:    sec_strncpy_s_nopad_probe
 * area:  secure_crt
 * funcs: strncpy_s strcpy_s strcat_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * conf:  probe
 * ref:   MS docs strncpy_s (debug CRT fills buffer with 0xFD; _CrtSetDebugFillThreshold)
 * note:  PROBE: retail CRT is believed to write only up to and including the terminator, leaving the tail bytes unchanged.
 */
#include "tc.h"
#include <string.h>

static void fill(char *b) { int i; for (i = 0; i < 16; i++) b[i] = 'Z'; }
static void tail_is_z(const char *b, int from) { int i; for (i = from; i < 16; i++) CHECK_EQ_I((unsigned char)b[i], 'Z'); }

TC_CASE(strncpy_s_tail_bytes_unchanged)
{ char b[16]; fill(b); CHECK_EQ_I(strncpy_s(b, 16, "ab", 5), 0); CHECK_STR(b, "ab"); tail_is_z(b, 3); }
TC_CASE(strcpy_s_tail_bytes_unchanged)
{ char b[16]; fill(b); CHECK_EQ_I(strcpy_s(b, 16, "ab"), 0); CHECK_STR(b, "ab"); tail_is_z(b, 3); }
TC_CASE(strcat_s_tail_bytes_unchanged)
{ char b[16]; fill(b); b[0] = 'a'; b[1] = 0; CHECK_EQ_I(strcat_s(b, 16, "bc"), 0); CHECK_STR(b, "abc"); tail_is_z(b, 4); }
int main(void) { tc_begin(); TC_RUN(strncpy_s_tail_bytes_unchanged); TC_RUN(strcpy_s_tail_bytes_unchanged); TC_RUN(strcat_s_tail_bytes_unchanged); return tc_end(); }
