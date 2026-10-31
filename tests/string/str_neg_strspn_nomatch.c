/* TC
 * id:    str_neg_strspn_nomatch
 * area:  string
 * funcs: strspn strcspn
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.24.5.6, 7.24.5.3
 */
#include "tc.h"
#include <string.h>

static const char *volatile v_s, *volatile v_set;
static size_t sp(const char *s, const char *set) { v_s = s; v_set = set; return strspn((const char *)v_s, (const char *)v_set); }
static size_t cs(const char *s, const char *set) { v_s = s; v_set = set; return strcspn((const char *)v_s, (const char *)v_set); }

TC_CASE(spn_disjoint) { CHECK_EQ_U(sp("abc", "xyz"), 0); }
TC_CASE(cspn_first_in_set) { CHECK_EQ_U(cs("abc", "a"), 0); }
TC_CASE(spn_empty) { CHECK_EQ_U(sp("", "abc"), 0); }
TC_CASE(cspn_empty) { CHECK_EQ_U(cs("", "abc"), 0); }

int main(void) { tc_begin(); TC_RUN(spn_disjoint); TC_RUN(cspn_first_in_set); TC_RUN(spn_empty); TC_RUN(cspn_empty); return tc_end(); }
