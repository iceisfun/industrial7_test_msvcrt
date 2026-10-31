/* TC
 * id:    loc_neg_setlocale_bad_category
 * area:  locale
 * funcs: setlocale
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs setlocale; _VALIDATE_RETURN convention
 * note:  PROBE: only return==NULL is near certain; errno==EINVAL and invalid-parameter handler count==1 are the guess (separate cases).
 */
#include "tc.h"
#include <locale.h>
#include <errno.h>
static void cp(char *d, const char *s, int n) { int i = 0; if (!s) { d[0] = 0; return; } while (s[i] && i < n - 1) { d[i] = s[i]; i++; } d[i] = 0; }
static int ends(const char *s, const char *suf) { int a = 0, b = 0; if (!s) return 0; while (s[a]) a++; while (suf[b]) b++; if (b > a) return 0; return tc_strequ(s + a - b, suf); }
static const char * volatile v_name;

TC_CASE(category_minus1) { tc_iph_install(); v_name = NULL; CHECK(setlocale(-1, v_name) == NULL); }
TC_CASE(category_max_plus1) { tc_iph_install(); CHECK(setlocale(LC_MAX + 1, NULL) == NULL); CHECK(setlocale(100, "C") == NULL); }
TC_CASE(errno_einval) { tc_iph_install(); errno = 0; setlocale(-1, NULL); CHECK_EQ_I(errno, EINVAL); errno = 0; setlocale(LC_MAX + 1, NULL); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(iph_called)
{
    if (!tc_iph_install()) TC_SKIP("no iph api");
    tc_iph_count = 0; setlocale(-1, NULL); CHECK_EQ_I(tc_iph_count, 1);
    setlocale(LC_MAX + 1, NULL); CHECK_EQ_I(tc_iph_count, 2);
}
TC_CASE(state_unchanged) { CHECK_STR(setlocale(LC_ALL, NULL), "C"); }
int main(void) { tc_begin(); TC_RUN(category_minus1); TC_RUN(category_max_plus1); TC_RUN(errno_einval); TC_RUN(iph_called); TC_RUN(state_unchanged); return tc_end(); }
