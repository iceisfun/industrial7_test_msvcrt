/* TC
 * id:    exp_neg_fullpath_small_buffer
 * area:  exports
 * kind:  negative
 * funcs: 
 * procs: _fullpath
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _fullpath: returns NULL if the buffer is too small (maxlen)
 * note:  Uses a caller-supplied buffer of 4 bytes against a long path; the result must be NULL and nothing may be written beyond 4 bytes (canary check).
 */
#include "tc.h"

typedef char *(__cdecl *full_t)(char *, const char *, size_t);

TC_CASE(too_small)
{
    full_t fp = (full_t)tc_crt_proc("_fullpath");
    char buf[16]; int i;
    CHECK(fp != NULL);
    if (!fp) return;
    for (i = 0; i < 16; i++) buf[i] = 'Z';
    CHECK(fp(buf, "C:\\some\\long\\directory\\name.txt", 4) == NULL);
    for (i = 4; i < 16; i++) CHECK_EQ_I(buf[i], 'Z');   /* no overrun past maxlen */
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(too_small); return tc_end(); }
