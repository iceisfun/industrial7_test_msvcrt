/* TC
 * id:    dir_probe_findclose_double
 * area:  dir
 * funcs: _findfirst32 _findclose
 * crt:   >=90
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _findclose (returns -1 on failure)
 * note:  Also probes _findclose(0) == -1 (FindClose(NULL) fails). Probes that closing the same search handle twice fails the second time (the CRT hands back the OS find handle,
 *        so FindClose on a closed handle fails). Not documented explicitly.
 */
#include "tc.h"
#include <io.h>

static const char *volatile v_spec;
TC_CASE(double_close)
{
    struct _finddata32_t fd; intptr_t h;
    v_spec = "*"; h = _findfirst32(v_spec, &fd);
    CHECK(h != -1);
    if (h == -1) return;
    CHECK_EQ_I(_findclose(h), 0);
    CHECK_EQ_I(_findclose(h), -1);
}
TC_CASE(zero_handle) { CHECK_EQ_I(_findclose((intptr_t)0), -1); }  /* FindClose(NULL) expected to fail */
int main(void) { tc_begin(); TC_RUN(double_close); TC_RUN(zero_handle); return tc_end(); }
