#include "ft_nm.h"

/*
** GNU nm sorts symbols with a simple strcmp (case-sensitive, byte order).
** '_' (ASCII 95) sorts between uppercase (65-90) and lowercase (97-122),
** which naturally produces the correct output.
*/
static int	sym_cmp(const void *a, const void *b)
{
	const t_sym	*sa = a;
	const t_sym	*sb = b;

	return (strcmp(sa->name, sb->name));
}

void	nm_sort(t_sym *syms, size_t count)
{
	qsort(syms, count, sizeof(t_sym), sym_cmp);
}
