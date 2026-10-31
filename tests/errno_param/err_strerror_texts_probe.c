/* TC
 * id:    err_strerror_texts_probe
 * area:  errno_param
 * funcs: strerror
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs strerror
 * note:  PROBE: exact wording for rarely used codes; each case asserts non-empty plus the single most likely string. EDOM differs by CRT generation: UCRT "Domain error", older CRTs "Math argument".
 */
#include "tc.h"
#include <string.h>
#include <errno.h>

static volatile int v_e;
static const char *msg(int e) { v_e = e; return strerror(v_e); }

TC_CASE(erange)  { const char *s = msg(ERANGE); CHECK(s && s[0]); CHECK_STR(s, "Result too large"); }
TC_CASE(edom)    { const char *s = msg(EDOM); CHECK(s && s[0]);
#if TC_CRT_VER >= 140
    CHECK_STR(s, "Domain error");
#else
    CHECK_STR(s, "Math argument");
#endif
}
TC_CASE(eilseq)  { const char *s = msg(EILSEQ); CHECK(s && s[0]);
#if TC_CRT_VER >= 80
    CHECK_STR(s, "Illegal byte sequence");
#endif
}
TC_CASE(enotempty) { const char *s = msg(ENOTEMPTY); CHECK(s && s[0]); CHECK_STR(s, "Directory not empty"); }
TC_CASE(enametoolong) { const char *s = msg(ENAMETOOLONG); CHECK(s && s[0]); CHECK_STR(s, "Filename too long"); }

int main(void)
{
    tc_begin();
    TC_RUN(erange); TC_RUN(edom); TC_RUN(eilseq); TC_RUN(enotempty); TC_RUN(enametoolong);
    return tc_end();
}
