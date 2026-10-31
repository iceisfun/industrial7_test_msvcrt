/* TC
 * id:    str_strcmp_highbit_unsigned
 * area:  string
 * funcs: strcmp strncmp
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.24.4 (compared as unsigned char); MS docs strcmp
 */
#include "tc.h"
#include <string.h>

#define SGN(x) ((x) < 0 ? -1 : (x) > 0 ? 1 : 0)
static volatile const char *va, *vb;
static int cmp(const char *a, const char *b) { va = a; vb = b; return SGN(strcmp((const char *)va, (const char *)vb)); }
static int ncmp(const char *a, const char *b, size_t n) { va = a; vb = b; return SGN(strncmp((const char *)va, (const char *)vb, n)); }

TC_CASE(x80_gt_x7f) { CHECK_EQ_I(cmp("\x80", "\x7f"), 1); CHECK_EQ_I(cmp("\x7f", "\x80"), -1); }
TC_CASE(xff_gt_a) { CHECK_EQ_I(cmp("\xff", "a"), 1); CHECK_EQ_I(cmp("a", "\xff"), -1); }
TC_CASE(strncmp_same) { CHECK_EQ_I(ncmp("\x80", "\x7f", 1), 1); CHECK_EQ_I(ncmp("\xff", "a", 3), 1); CHECK_EQ_I(ncmp("a", "\xff", 1), -1); }
TC_CASE(xe9_vs_e) { CHECK_EQ_I(cmp("\xe9", "e"), 1); CHECK_EQ_I(ncmp("e", "\xe9", 1), -1); }

int main(void) { tc_begin(); TC_RUN(x80_gt_x7f); TC_RUN(xff_gt_a); TC_RUN(strncmp_same); TC_RUN(xe9_vs_e); return tc_end(); }
