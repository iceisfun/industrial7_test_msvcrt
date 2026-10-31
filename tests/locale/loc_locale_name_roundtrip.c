/* TC
 * id:    loc_locale_name_roundtrip
 * area:  locale
 * funcs: setlocale
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs setlocale (returned string can be passed back)
 * note:  Returned pointer is copied before the next call.
 */
#include "tc.h"
#include <locale.h>
#include <errno.h>
static void cp(char *d, const char *s, int n) { int i = 0; if (!s) { d[0] = 0; return; } while (s[i] && i < n - 1) { d[i] = s[i]; i++; } d[i] = 0; }
static int ends(const char *s, const char *suf) { int a = 0, b = 0; if (!s) return 0; while (s[a]) a++; while (suf[b]) b++; if (b > a) return 0; return tc_strequ(s + a - b, suf); }
static const char * volatile v_name;

static void rt(const char *name)
{
    char a[256], b[256];
    v_name = name;
    cp(a, setlocale(LC_ALL, v_name), sizeof a);
    CHECK_STR(a, name);
    v_name = a;
    cp(b, setlocale(LC_ALL, v_name), sizeof b);
    CHECK_STR(b, a);
    setlocale(LC_ALL, "C");
}
TC_CASE(rt_english) { rt("English_United States.1252"); }
TC_CASE(rt_german) { rt("German_Germany.1252"); }
TC_CASE(rt_french) { rt("French_France.1252"); }
TC_CASE(rt_C) { rt("C"); }
int main(void) { tc_begin(); TC_RUN(rt_english); TC_RUN(rt_german); TC_RUN(rt_french); TC_RUN(rt_C); return tc_end(); }
