/* TC
 * id:    fio_neg_probe_fopen_mode_letter_conflicts
 * area:  stdio_file
 * kind:  negative
 * funcs: fopen fclose
 * procs: 
 * crt:   >=80
 * tier:  p1
 * conf:  probe
 * ref:   MS docs fopen (mode string grammar: one of r/w/a, optional single +, one of t/b)
 * note:  PROBE: does the CRT mode parser reject (NULL, EINVAL, handler once) or silently ignore/stop at a conflicting or
 *        repeated suffix/access letter? Cases: rbt rtb r+b+ rb+b (suffix conflicts), rr ww r+a w+r aw rw (second access
 *        letter), +r (leading '+'). Guess: rejected. Old CRTs may merely stop parsing at the conflict and open the file
 *        (in which case the guessed assertion is wrong - which is what real Windows will tell us). A file created by an
 *        accepted open is cleaned up.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#define TC_S2(x) #x
#define TC_S1(x) TC_S2(x)
#define V TC_S1(TC_CRT_VER)
static int exists(const char *n) { return GetFileAttributesA(n) != INVALID_FILE_ATTRIBUTES; }
#define F "tc_fio_neg_probe_modeletter_" V ".tmp"
static const char * volatile fname = F;
static void bad(const char *mode)
{
    FILE *f; int n0 = tc_iph_count; errno = 0;
    f = fopen(fname, mode);
    CHECK(f == NULL);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count - n0, 1);
    if (f) fclose(f);
    DeleteFileA(fname);
}
static const char * volatile m_rbt = "rbt", * volatile m_rtb = "rtb", * volatile m_dup = "r+b+", * volatile m_bb = "rb+b";
static const char * volatile m_rr = "rr", * volatile m_ww = "ww", * volatile m_rpa = "r+a", * volatile m_wpr = "w+r", * volatile m_aw = "aw", * volatile m_rw = "rw";
static const char * volatile m_plus = "+r", * volatile m_wbt = "wbt";
TC_CASE(bin_and_text) { bad(m_rbt); bad(m_rtb); }
TC_CASE(dup_chars)    { bad(m_dup); bad(m_bb); }
TC_CASE(dup_access)   { bad(m_rr); bad(m_ww); }
TC_CASE(mixed_access) { bad(m_rpa); bad(m_wpr); bad(m_aw); bad(m_rw); }
TC_CASE(leading_plus) { bad(m_plus); }
TC_CASE(no_file_left_after_wbt)
{
    FILE *f; DeleteFileA(fname);
    f = fopen(fname, m_wbt);
    if (f) fclose(f);
    CHECK(f == NULL);
    CHECK(!exists(fname));
    DeleteFileA(fname);
}
int main(void)
{
    tc_begin(); DeleteFileA(fname);
    CHECK(tc_iph_install());
    TC_RUN(bin_and_text); TC_RUN(dup_chars); TC_RUN(dup_access); TC_RUN(mixed_access); TC_RUN(leading_plus);
    TC_RUN(no_file_left_after_wbt);
    DeleteFileA(fname);
    return tc_end();
}
