/* TC
 * id:    prc_popen_write
 * area:  process
 * funcs: _popen _pclose
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  observed
 * ref:   MS docs _popen; findstr exit code 0 on match, 1 on no match
 */
#include "tc.h"
#include <stdio.h>
static int feed(const char *s)
{
    FILE *volatile f = _popen("findstr x >nul", "w");
    if (!f) return -1000;
    fputs(s, f);
    return _pclose(f);
}
TC_CASE(match)   { CHECK_EQ_I(feed("xyz\n"), 0); }
TC_CASE(nomatch) { CHECK_EQ_I(feed("abc\n"), 1); }
int main(void) { tc_begin(); TC_RUN(match); TC_RUN(nomatch); return tc_end(); }
