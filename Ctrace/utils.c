#include "ft_nm.h"

int	nm_error(const char *prog, const char *file, const char *msg)
{
	if (prog && file)
	{
		write(2, prog, strlen(prog));
		write(2, ": ", 2);
		write(2, file, strlen(file));
		write(2, ": ", 2);
	}
	if (msg)
	{
		write(2, msg, strlen(msg));
	}
	write(2, "\n", 1);
	return (-1);
}
