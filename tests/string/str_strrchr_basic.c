/* TC
 * id:    str_strrchr_basic
 * area:  string
 * funcs: strrchr
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.24.5.5; MS docs strrchr
 */
#include "tc.h"
#include <string.h>

static const char *vs;
static volatile int vc;

TC_CASE(last) { static const char s[] = "a.b.c"; vs = s; vc = '.'; CHECK_EQ_P(strrchr(vs, vc), s + 3); }
TC_CASE(single) { static const char s[] = "abc"; vs = s; vc = 'b'; CHECK_EQ_P(strrchr(vs, vc), s + 1); }
TC_CASE(nul) { static const char s[] = "abc"; vs = s; vc = 0; CHECK_EQ_P(strrchr(vs, vc), s + 3); }
TC_CASE(path) { static const char s[] = "C:\\dir\\file.txt"; vs = s; vc = '\\'; CHECK_STR(strrchr(vs, vc) + 1, "file.txt"); }
TC_CASE(highbit) { static const char s[] = "\xE9x\xE9y"; vs = s; vc = 0xE9; CHECK_EQ_P(strrchr(vs, vc), s + 2); }
int main(void) { tc_begin(); TC_RUN(last); TC_RUN(single); TC_RUN(nul); TC_RUN(path); TC_RUN(highbit); return tc_end(); }
