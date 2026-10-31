/* TC
 * id:    str_neg_strchr_notfound
 * area:  string
 * funcs: strchr strrchr
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.24.5.2, 7.24.5.5
 */
#include "tc.h"
#include <string.h>

static const char *vs;
static volatile int vc;

TC_CASE(strchr_absent) { static const char s[] = "hello"; vs = s; vc = 'z'; CHECK(strchr(vs, vc) == NULL); }
TC_CASE(strrchr_absent) { static const char s[] = "hello"; vs = s; vc = 'z'; CHECK(strrchr(vs, vc) == NULL); }
TC_CASE(empty_string) { static const char s[] = ""; vs = s; vc = 'a'; CHECK(strchr(vs, vc) == NULL); CHECK(strrchr(vs, vc) == NULL); }
TC_CASE(empty_nul) { static const char s[] = ""; vs = s; vc = 0; CHECK_EQ_P(strchr(vs, vc), s); CHECK_EQ_P(strrchr(vs, vc), s); }
TC_CASE(case_sensitive) { static const char s[] = "hello"; vs = s; vc = 'H'; CHECK(strchr(vs, vc) == NULL); CHECK(strrchr(vs, vc) == NULL); }
int main(void) { tc_begin(); TC_RUN(strchr_absent); TC_RUN(strrchr_absent); TC_RUN(empty_string); TC_RUN(empty_nul); TC_RUN(case_sensitive); return tc_end(); }
