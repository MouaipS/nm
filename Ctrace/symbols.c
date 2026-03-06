#include "ft_nm.h"

/*
** Determine the nm letter for a symbol given its section header flags.
** Returns uppercase; caller lowercases for STB_LOCAL.
*/
static char	letter_from_section64(t_nm_ctx *ctx, uint16_t shndx)
{
	Elf64_Shdr	*shdrs = ctx->shdrs;
	Elf64_Shdr	*sh;
	uint64_t	flags;
	uint32_t	type;

	if (shndx == SHN_UNDEF)
		return ('U');
	if (shndx == SHN_ABS)
		return ('A');
	if (shndx == SHN_COMMON)
		return ('C');
	if (shndx >= ctx->shnum)
		return ('?');
	sh = &shdrs[shndx];
	flags = sh->sh_flags;
	type = sh->sh_type;
	if (type == SHT_NOBITS && (flags & SHF_ALLOC))
		return ('B');
	if ((flags & SHF_EXECINSTR) && (flags & SHF_ALLOC))
		return ('T');
	if ((flags & SHF_WRITE) && (flags & SHF_ALLOC))
		return ('D');
	if (flags & SHF_ALLOC)
		return ('R');
	return ('N');
}

static char	letter_from_section32(t_nm_ctx *ctx, uint16_t shndx)
{
	Elf32_Shdr	*shdrs = ctx->shdrs;
	Elf32_Shdr	*sh;
	uint32_t	flags;
	uint32_t	type;

	if (shndx == SHN_UNDEF)
		return ('U');
	if (shndx == SHN_ABS)
		return ('A');
	if (shndx == SHN_COMMON)
		return ('C');
	if (shndx >= ctx->shnum)
		return ('?');
	sh = &shdrs[shndx];
	flags = sh->sh_flags;
	type = sh->sh_type;
	if (type == SHT_NOBITS && (flags & SHF_ALLOC))
		return ('B');
	if ((flags & SHF_EXECINSTR) && (flags & SHF_ALLOC))
		return ('T');
	if ((flags & SHF_WRITE) && (flags & SHF_ALLOC))
		return ('D');
	if (flags & SHF_ALLOC)
		return ('R');
	return ('N');
}

static char	resolve_letter(char base, int binding, uint16_t shndx)
{
	char	c;

	c = base;
	if (binding == STB_WEAK)
	{
		if (shndx == SHN_UNDEF)
			c = 'w';
		else
			c = 'W';
		return (c);
	}
	if (binding == STB_LOCAL)
		c = (char)tolower((unsigned char)c);
	return (c);
}

/* ---------- 64-bit ---------- */

static int	collect64(t_nm_ctx *ctx, t_sym **out, size_t *count)
{
	size_t		n;
	size_t		i;
	Elf64_Sym	*sym;
	t_sym		*arr;
	char		base;
	int			binding;
	const char	*name;

	if (!ctx->symtab || !ctx->strtab || ctx->sym_entsize == 0)
		return (0);
	n = ctx->symtab_size / ctx->sym_entsize;
	arr = malloc(n * sizeof(t_sym));
	if (!arr)
		return (-1);
	*count = 0;
	i = 0;
	while (i < n)
	{
		sym = (Elf64_Sym *)((uint8_t *)ctx->symtab + i * ctx->sym_entsize);
		i++;
		/* skip the null symbol (index 0) and file/section symbols */
		int stype = ELF64_ST_TYPE(sym->st_info);
		if (stype == STT_FILE || stype == STT_SECTION)
			continue ;
		if (sym->st_name >= ctx->strtab_size)
			continue ;
		name = ctx->strtab + sym->st_name;
		if (name[0] == '\0')
			continue ;
		binding = ELF64_ST_BIND(sym->st_info);
		base = letter_from_section64(ctx, sym->st_shndx);
		arr[*count].letter = resolve_letter(base, binding, sym->st_shndx);
		arr[*count].name = name;
		arr[*count].value = sym->st_value;
		arr[*count].has_value = (sym->st_shndx != SHN_UNDEF
			&& binding != STB_WEAK) || sym->st_value != 0;
		(*count)++;
	}
	*out = arr;
	return (0);
}

/* ---------- 32-bit ---------- */

static int	collect32(t_nm_ctx *ctx, t_sym **out, size_t *count)
{
	size_t		n;
	size_t		i;
	Elf32_Sym	*sym;
	t_sym		*arr;
	char		base;
	int			binding;
	const char	*name;

	if (!ctx->symtab || !ctx->strtab || ctx->sym_entsize == 0)
		return (0);
	n = ctx->symtab_size / ctx->sym_entsize;
	arr = malloc(n * sizeof(t_sym));
	if (!arr)
		return (-1);
	*count = 0;
	i = 0;
	while (i < n)
	{
		sym = (Elf32_Sym *)((uint8_t *)ctx->symtab + i * ctx->sym_entsize);
		i++;
		int stype = ELF32_ST_TYPE(sym->st_info);
		if (stype == STT_FILE || stype == STT_SECTION)
			continue ;
		if (sym->st_name >= ctx->strtab_size)
			continue ;
		name = ctx->strtab + sym->st_name;
		if (name[0] == '\0')
			continue ;
		binding = ELF32_ST_BIND(sym->st_info);
		base = letter_from_section32(ctx, sym->st_shndx);
		arr[*count].letter = resolve_letter(base, binding, sym->st_shndx);
		arr[*count].name = name;
		arr[*count].value = (uint64_t)sym->st_value;
		arr[*count].has_value = (sym->st_shndx != SHN_UNDEF
			&& binding != STB_WEAK) || sym->st_value != 0;
		(*count)++;
	}
	*out = arr;
	return (0);
}

int	nm_collect_symbols(t_nm_ctx *ctx, t_sym **out, size_t *count)
{
	*out = NULL;
	*count = 0;
	if (ctx->is_64)
		return (collect64(ctx, out, count));
	return (collect32(ctx, out, count));
}
