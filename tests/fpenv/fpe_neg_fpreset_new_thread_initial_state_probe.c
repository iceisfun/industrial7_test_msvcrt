/* TC
 * id:    fpe_neg_fpreset_new_thread_initial_state_probe
 * area:  fpenv
 * kind:  negative
 * funcs: _controlfp
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcrt
 * tier:  p2
 * conf:  probe
 * ref:   Win32 CreateThread; MS docs _controlfp
 * note:  PROBE: whether a new thread inherits the creator's FP control state. Asserts only that the RC field is a valid value that is either default (nearest) or the creator's (up); logs which. Creator unmasks underflow while creating (no FP arithmetic in that window).
 */
#include "tc.h"
#include <float.h>
#define ALLRC (_MCW_EM | _MCW_RC | _MCW_DN)
static void logx(const char *what, unsigned long long v) { char b[32]; tc_hex(v, b); tc_write("TC INFO " TC_TEST_ID " "); tc_write(what); tc_write("="); tc_write(b); tc_write("\n"); }

static volatile unsigned t_word;
static DWORD WINAPI th(LPVOID p) { (void)p; t_word = _controlfp(0, 0); return 0; }
TC_CASE(initial_state)
{
    HANDLE h; unsigned orig = _controlfp(0, 0);
    _controlfp(_RC_UP, _MCW_RC);
    _controlfp(0, _EM_UNDERFLOW);
    h = CreateThread(NULL, 0, th, NULL, 0, NULL);
    CHECK(h != NULL);
    if (h) { WaitForSingleObject(h, 10000); CloseHandle(h); }
    _controlfp(orig, _MCW_EM | _MCW_RC);
    _clearfp();
    logx("thread_word", t_word);
    logx("thread_inherited_rc", (t_word & _MCW_RC) == _RC_UP);
    CHECK((t_word & _MCW_RC) == _RC_NEAR || (t_word & _MCW_RC) == _RC_UP);
    CHECK_EQ_U(t_word & _EM_INEXACT, _EM_INEXACT);   /* inexact stays masked in either model */
}
int main(void) { tc_begin(); TC_RUN(initial_state); return tc_end(); }
