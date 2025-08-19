#include <signal.h>

void	set_child_signals_default(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void	set_parent_signals_interactive(void)
{
	signal(SIGQUIT, SIG_IGN);
}
