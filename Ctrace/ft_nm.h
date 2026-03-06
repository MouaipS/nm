#ifndef FT_NM_H
# define FT_NM_H

# include <elf.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/mman.h>
# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <stdio.h>
# include <stdint.h>
# include <ctype.h>
# include <errno.h>

/*
** Generic ELF context — works for both 32 and 64 bit
*/
typedef struct s_nm_ctx
{
	void		*map;
	size_t		map_size;
	int			is_64;

	/* pointers into the map — set after parsing the header */
	void		*shdrs;			/* section header table */
	uint16_t	shnum;			/* number of section headers */
	uint16_t	shstrndx;		/* index of .shstrtab */
	char		*shstrtab;		/* section name strings */

	void		*symtab;		/* raw symtab section data */
	size_t		symtab_size;	/* bytes */
	size_t		sym_entsize;	/* sizeof one entry */
	char		*strtab;		/* symbol name strings */
	size_t		strtab_size;
}	t_nm_ctx;

/*
** One parsed symbol ready to print
*/
typedef struct s_sym
{
	const char	*name;
	uint64_t	value;
	char		letter;
	int			has_value;		/* 0 for 'U' / 'w' */
}	t_sym;

/* parse.c */
int		nm_parse(t_nm_ctx *ctx, const char *filename);

/* symbols.c */
int		nm_collect_symbols(t_nm_ctx *ctx, t_sym **out, size_t *count);

/* sort.c */
void	nm_sort(t_sym *syms, size_t count);

/* print.c */
void	nm_print(const t_sym *syms, size_t count, int is_64);

/* utils.c */
int		nm_error(const char *prog, const char *file, const char *msg);
int		nm_bounds_check(t_nm_ctx *ctx, const void *ptr, size_t size);

#endif
