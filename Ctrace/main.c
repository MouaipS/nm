#include "ft_nm.h"

static int	process_file(const char *filename, int print_filename)
{
	t_nm_ctx	ctx;
	t_sym		*syms;
	size_t		count;
	int			ret;

	memset(&ctx, 0, sizeof(ctx));
	ret = nm_parse(&ctx, filename);
	if (ret < 0)
		return (1);
	if (nm_collect_symbols(&ctx, &syms, &count) < 0)
	{
		munmap(ctx.map, ctx.map_size);
		return (nm_error("ft_nm", filename, "memory allocation failed"), 1);
	}
	nm_sort(syms, count);
	if (print_filename)
	{
		write(1, "\n", 1);
		write(1, filename, strlen(filename));
		write(1, ":\n", 2);
	}
	nm_print(syms, count, ctx.is_64);
	free(syms);
	munmap(ctx.map, ctx.map_size);
	return (0);
}

int	main(int argc, char **argv)
{
	int	i;
	int	ret;
	int	multi;

	if (argc < 2)
	{
		/* default: try a.out like real nm */
		return (process_file("a.out", 0));
	}
	multi = (argc > 2);
	ret = 0;
	i = 1;
	while (i < argc)
	{
		if (process_file(argv[i], multi) != 0)
			ret = 1;
		i++;
	}
	return (ret);
}
