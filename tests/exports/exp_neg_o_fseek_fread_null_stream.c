/* TC
 * id:    exp_neg_o_fseek_fread_null_stream
 * area:  exports
 * funcs:
 * procs: _o_fseek _o_fread _set_invalid_parameter_handler
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs fseek, _fseeki64, fread (EINVAL / invalid parameter; size or count 0 returns 0)
 * note:  PROBE: _o_ stream aliases validate stream/whence/offset like fseek/fread. NULL stream only (isolated: a crash here must not mask other cases).
 *        The _o_ aliases are undocumented; expectations mirror the documented behaviour of the underlying function.
 */
#include "tc.h"
#include <errno.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>

typedef int (__cdecl *fseek_t)(FILE *, long, int);
typedef size_t (__cdecl *fread_t)(void *, size_t, size_t, FILE *);
static fseek_t p_fseek; static fread_t p_fread;

TC_CASE(fseek_null)
{
    int r;
    if (!p_fseek) TC_SKIP("not exported");
    if (!tc_iph_install()) TC_SKIP("no handler api");
    tc_iph_count = 0; errno = 0; r = p_fseek(NULL, 0, SEEK_SET);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(fread_null_stream)
{
    char b[8]; size_t r;
    if (!p_fread) TC_SKIP("not exported");
    if (!tc_iph_install()) TC_SKIP("no handler api");
    tc_iph_count = 0; errno = 0; r = p_fread(b, 1, 1, NULL);
    CHECK_EQ_U(r, 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
    p_fseek = (fseek_t)tc_crt_proc("_o_fseek"); p_fread = (fread_t)tc_crt_proc("_o_fread");
    TC_RUN(fseek_null); TC_RUN(fread_null_stream);
    return tc_end();
}
