/* TC
 * id:    gct_strcmpi_lowercase_folding_boundary
 * area:  ctype
 * funcs: 
 * procs: _stricmp
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _stricmp: strings are converted to lowercase before comparing
 * note:  Classic difference from a toupper-based compare for chars between 'Z' (0x5A) and 'a' (0x61). Sign only.
 */
#include "tc.h"
typedef int (__cdecl *cmp_t)(const char *, const char *);
static cmp_t strcmpi_p;
static volatile const char *pa, *pb;
static int c(const char *a, const char *b) { pa = a; pb = b; return strcmpi_p ? strcmpi_p((const char *)pa, (const char *)pb) : 0; }
TC_CASE(underscore_vs_letters) { CHECK(c("_", "a") < 0); CHECK(c("_", "A") < 0); CHECK(c("a", "_") > 0); }
TC_CASE(bracket_caret_backtick) { CHECK(c("[", "a") < 0); CHECK(c("^", "A") < 0); CHECK(c("`", "A") < 0); CHECK(c("\\", "Z") < 0); }
TC_CASE(brace_above_z) { CHECK(c("{", "Z") > 0); CHECK(c("{", "z") > 0); }
TC_CASE(below_letters) { CHECK(c("@", "A") < 0); CHECK(c("A", "@") > 0); CHECK(c("0", "a") < 0); }
int main(void) { tc_begin(); strcmpi_p = (cmp_t)tc_crt_proc("_stricmp"); if (!strcmpi_p) { tc_write("TC FAIL " TC_TEST_ID "::loaded _stricmp missing\n"); return 1; }
    TC_RUN(underscore_vs_letters); TC_RUN(bracket_caret_backtick); TC_RUN(brace_above_z); TC_RUN(below_letters); return tc_end(); }
