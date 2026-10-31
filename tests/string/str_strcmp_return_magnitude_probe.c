/* TC
 * id:    str_strcmp_return_magnitude_probe
 * area:  string
 * funcs: strcmp strncmp
 * crt:   all
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs strcmp (only says <0, 0, >0)
 * note:  PROBE: asserts the exact return value is -1/1 (ucrt normalises; older msvcrt asm may return a byte difference). Kept separate from the sign-only tests.
 */
#include "tc.h"
#include <string.h>

static volatile const char *va, *vb;

TC_CASE(strcmp_exact) { va = "a"; vb = "c"; CHECK_EQ_I(strcmp((const char *)va, (const char *)vb), -1); va = "c"; vb = "a"; CHECK_EQ_I(strcmp((const char *)va, (const char *)vb), 1); va = "a"; vb = "a"; CHECK_EQ_I(strcmp((const char *)va, (const char *)vb), 0); }
TC_CASE(strncmp_exact) { va = "a"; vb = "c"; CHECK_EQ_I(strncmp((const char *)va, (const char *)vb, 1), -1); va = "c"; vb = "a"; CHECK_EQ_I(strncmp((const char *)va, (const char *)vb, 1), 1); }

int main(void) { tc_begin(); TC_RUN(strcmp_exact); TC_RUN(strncmp_exact); return tc_end(); }
