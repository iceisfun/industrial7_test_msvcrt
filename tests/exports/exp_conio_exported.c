/* TC
 * id:    exp_conio_exported
 * area:  exports
 * funcs: 
 * procs: _kbhit _getch _getche _putch _cputs _ungetch _getwch _putwch _cputws
 * crt:   all
 * tier:  p2
 * conf:  documented
 * ref:   MS docs console and port I/O
 * note:  Never invoke: no console I/O in the corpus.
 */
#include "tc.h"

static void need_all(const char *const *names, int n)
{
    int i;
    for (i = 0; i < n; i++)
        if (!tc_crt_proc(names[i])) tc_fail_expr(__FILE__, __LINE__, names[i]);
}
#define NEEDS(arr) need_all(arr, (int)(sizeof(arr) / sizeof((arr)[0])))

TC_CASE(resolves_narrow)
{
    static const char *const names[] = { "_kbhit", "_getch", "_getche", "_putch", "_cputs", "_ungetch" };
    NEEDS(names);
}

TC_CASE(resolves_wide)
{
    static const char *const names[] = { "_getwch", "_putwch", "_cputws" };
    NEEDS(names);
}

int main(void)
{
    tc_begin();
    TC_RUN(resolves_narrow);
    TC_RUN(resolves_wide);
    return tc_end();
}
