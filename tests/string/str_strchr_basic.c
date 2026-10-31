/* TC
 * id:    str_strchr_basic
 * area:  string
 * funcs: strchr
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.24.5.2; MS docs strchr
 */
#include "tc.h"
#include <string.h>

static const char *vs;
static volatile int vc;

TC_CASE(first) { static const char s[] = "hello world"; vs = s; vc = 'o'; CHECK_EQ_P(strchr(vs, vc), s + 4); vc = 'h'; CHECK_EQ_P(strchr(vs, vc), s); }
TC_CASE(nul) { static const char s[] = "abc"; vs = s; vc = 0; CHECK_EQ_P(strchr(vs, vc), s + 3); }
TC_CASE(conv_to_char) { static const char s[] = "xyzabc"; vs = s; vc = 'a' + 256; CHECK_EQ_P(strchr(vs, vc), s + 3); }
TC_CASE(highbit) { static const char s[] = "caf\xE9!"; vs = s; vc = 0xE9; CHECK_EQ_P(strchr(vs, vc), s + 3); vc = -23; CHECK_EQ_P(strchr(vs, vc), s + 3); }
TC_CASE(repeated) { static const char s[] = "aaa"; vs = s; vc = 'a'; CHECK_EQ_P(strchr(vs, vc), s); }
int main(void) { tc_begin(); TC_RUN(first); TC_RUN(nul); TC_RUN(conv_to_char); TC_RUN(highbit); TC_RUN(repeated); return tc_end(); }
