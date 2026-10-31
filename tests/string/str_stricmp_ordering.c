/* TC
 * id:    str_stricmp_ordering
 * area:  string
 * funcs: _stricmp _strnicmp
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _stricmp, _strnicmp (compare after converting to lowercase)
 * note:  Characters between 'Z' and 'a' ([ \ ] ^ _ `) sort BELOW letters because comparison folds to lowercase, not uppercase.
 */
#include "tc.h"
#include <string.h>

#define SGN(x) ((x) < 0 ? -1 : (x) > 0 ? 1 : 0)
static volatile const char *va, *vb;
static int icmp(const char *a, const char *b) { va = a; vb = b; return SGN(_stricmp((const char *)va, (const char *)vb)); }
static int nicmp(const char *a, const char *b, size_t n) { va = a; vb = b; return SGN(_strnicmp((const char *)va, (const char *)vb, n)); }
static int lowcmp(const char *a, const char *b)  /* reference: ASCII tolower then unsigned compare */
{
    for (;; a++, b++) {
        unsigned char x = (unsigned char)*a, y = (unsigned char)*b;
        if (x >= 'A' && x <= 'Z') x += 32;
        if (y >= 'A' && y <= 'Z') y += 32;
        if (x != y) return x < y ? -1 : 1;
        if (!x) return 0;
    }
}

TC_CASE(bracket_lt_a) { CHECK_EQ_I(icmp("[", "a"), -1); CHECK_EQ_I(icmp("a", "["), 1); CHECK_EQ_I(icmp("[", "A"), -1); }
TC_CASE(underscore_lt_A) { CHECK_EQ_I(icmp("_", "A"), -1); CHECK_EQ_I(icmp("A", "_"), 1); CHECK_EQ_I(icmp("`", "Z"), -1); }
TC_CASE(Z_gt_underscore) { CHECK_EQ_I(icmp("Z", "_"), 1); CHECK_EQ_I(icmp("_", "Z"), -1); CHECK_EQ_I(icmp("z", "^"), 1); }
TC_CASE(n_variant) { CHECK_EQ_I(nicmp("_x", "AX", 1), -1); CHECK_EQ_I(nicmp("Z", "_", 1), 1); CHECK_EQ_I(nicmp("a_", "A[", 1), 0); }
TC_CASE(matches_tolower_strcmp)
{
    static const char *v[] = { "[", "\\", "]", "^", "_", "`", "A", "Z", "a", "z", "@", "{", "0", "9", "ab_c", "AB[C", "Ab", "aB_" };
    int i, j, n = (int)(sizeof v / sizeof v[0]);
    for (i = 0; i < n; i++) for (j = 0; j < n; j++) {
        if (icmp(v[i], v[j]) != lowcmp(v[i], v[j])) { CHECK(0 && "_stricmp disagrees with tolower+strcmp"); return; }
    }
}

int main(void) { tc_begin(); TC_RUN(bracket_lt_a); TC_RUN(underscore_lt_A); TC_RUN(Z_gt_underscore); TC_RUN(n_variant); TC_RUN(matches_tolower_strcmp); return tc_end(); }
