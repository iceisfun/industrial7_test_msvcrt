/* TC
 * id:    str_neg_strtok_s_null_null
 * area:  string
 * funcs:
 * procs: strtok_s _set_invalid_parameter_handler
 * crt:   >=100
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs strtok_s
 * note:  PROBE: strtok_s(NULL, delim, &ctx) with ctx == NULL (no prior string). Believed to be treated as
 *        invalid (returns NULL, errno EINVAL, invalid-parameter handler called once) per the CRT's
 *        validation "string != NULL || *context != NULL". Not certain the docs state it.
 */
#include "tc.h"
#include <errno.h>

typedef char *(__cdecl *strtok_s_t)(char *, const char *, char **);

TC_CASE(null_null)
{
    strtok_s_t p = (strtok_s_t)tc_crt_proc("strtok_s");
    char *ctx = NULL; int before;
    if (!p) TC_SKIP("strtok_s not exported");
    errno = 0; before = tc_iph_count;
    CHECK(p(NULL, " ", &ctx) == NULL);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count - before, 1);
}

int main(void) { tc_begin(); CHECK(tc_iph_install()); TC_RUN(null_null); return tc_end(); }
