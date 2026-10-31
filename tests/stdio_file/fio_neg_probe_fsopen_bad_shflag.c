/* TC
 * id:    fio_neg_probe_fsopen_bad_shflag
 * area:  stdio_file
 * funcs: _fsopen fclose
 * crt:   >=80
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _fsopen; _sopen_s documents shflag validation
 * note:  PROBE: whether _fsopen rejects an undefined shflag (0x77) with NULL/EINVAL and calls the
 *        invalid-parameter handler. Documented for _sopen_s/_wsopen_s; unsure for _fsopen.
 */
#include "tc.h"
#define TC_S2(x) #x
#define TC_S1(x) TC_S2(x)
#define TC_VS TC_S1(TC_CRT_VER)  /* per-CRT file names: runs may be concurrent */
#define TC_W2(x) L##x
#define TC_WID(x) TC_W2(x)
#include <stdio.h>
#include <errno.h>

static const char NAME[] = "tc_fio_neg_probe_fsopen_bad_shflag_" TC_VS ".tmp";
TC_CASE(bad_shflag)
{
    FILE *f; volatile int sh = 0x77;
    if (!tc_iph_install()) TC_SKIP("no iph");
    tc_iph_count = 0; errno = 0;
    f = _fsopen(NAME, "wb", sh);
    CHECK(f == NULL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    if (f) fclose(f);
    DeleteFileA(NAME);
}
int main(void) { tc_begin(); DeleteFileA(NAME); TC_RUN(bad_shflag); return tc_end(); }
