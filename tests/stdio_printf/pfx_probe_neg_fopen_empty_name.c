/* TC
 * id:    pfx_probe_neg_fopen_empty_name
 * area:  stdio_printf
 * kind:  negative
 * funcs: fopen fclose
 * crt:   ucrtbase
 * tier:  p2
 * conf:  observed
 * ref:   MS docs fopen
 * note:  fopen("", "r") returns NULL with errno EINVAL (observed Win10 22H2 ucrtbase: the empty name is rejected before CreateFile; old guess ENOENT).
 *        Raw evidence (baseline): TC FAIL ...::empty_name errno == ENOENT got=22 want=2. Only verified on ucrtbase.
 *        crt narrowed to ucrtbase: the observed behaviour was measured on Win10 22H2 ucrtbase only (older CRTs [unverified]).
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static const char *volatile empty = "";

TC_CASE(empty_name)
{
    FILE *f; errno = 0; f = fopen(empty, "r");
    CHECK(f == NULL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 0);   /* observed: handler not called */ if (f) fclose(f);
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(empty_name); return tc_end(); }
