#include "ft_nm.h"

/*
** Bounds check: ensure [ptr, ptr+size) is within the mapped file.
*/
int	nm_bounds_check(t_nm_ctx *ctx, const void *ptr, size_t size)
{
	const uint8_t	*start = ctx->map;
	const uint8_t	*end = start + ctx->map_size;
	const uint8_t	*p = ptr;

	if (p < start || p + size > end || p + size < p)
		return (0);
	return (1);
}

/* ---------- 64-bit helpers ---------- */

static int	parse_shdr64(t_nm_ctx *ctx)
{
	Elf64_Ehdr	*ehdr = ctx->map;
	Elf64_Shdr	*shdrs;
	Elf64_Shdr	*shstr_hdr;
	Elf64_Shdr	*sh;
	uint16_t	i;

	if (!nm_bounds_check(ctx, (uint8_t *)ctx->map + ehdr->e_shoff,
			(size_t)ehdr->e_shnum * ehdr->e_shentsize))
		return (-1);
	shdrs = (Elf64_Shdr *)((uint8_t *)ctx->map + ehdr->e_shoff);
	ctx->shdrs = shdrs;
	ctx->shnum = ehdr->e_shnum;
	ctx->shstrndx = ehdr->e_shstrndx;
	if (ctx->shstrndx >= ctx->shnum)
		return (-1);
	shstr_hdr = &shdrs[ctx->shstrndx];
	if (!nm_bounds_check(ctx,
			(uint8_t *)ctx->map + shstr_hdr->sh_offset, shstr_hdr->sh_size))
		return (-1);
	ctx->shstrtab = (char *)ctx->map + shstr_hdr->sh_offset;
	i = 0;
	while (i < ctx->shnum)
	{
		sh = &shdrs[i];
		if (sh->sh_type == SHT_SYMTAB)
		{
			if (!nm_bounds_check(ctx,
					(uint8_t *)ctx->map + sh->sh_offset, sh->sh_size))
				return (-1);
			ctx->symtab = (uint8_t *)ctx->map + sh->sh_offset;
			ctx->symtab_size = sh->sh_size;
			ctx->sym_entsize = sh->sh_entsize ? sh->sh_entsize : sizeof(Elf64_Sym);
			/* sh_link points to the associated .strtab */
			if (sh->sh_link < ctx->shnum)
			{
				Elf64_Shdr *str_hdr = &shdrs[sh->sh_link];
				if (!nm_bounds_check(ctx,
						(uint8_t *)ctx->map + str_hdr->sh_offset,
						str_hdr->sh_size))
					return (-1);
				ctx->strtab = (char *)ctx->map + str_hdr->sh_offset;
				ctx->strtab_size = str_hdr->sh_size;
			}
		}
		i++;
	}
	return (0);
}

/* ---------- 32-bit helpers ---------- */

static int	parse_shdr32(t_nm_ctx *ctx)
{
	Elf32_Ehdr	*ehdr = ctx->map;
	Elf32_Shdr	*shdrs;
	Elf32_Shdr	*shstr_hdr;
	Elf32_Shdr	*sh;
	uint16_t	i;

	if (!nm_bounds_check(ctx, (uint8_t *)ctx->map + ehdr->e_shoff,
			(size_t)ehdr->e_shnum * ehdr->e_shentsize))
		return (-1);
	shdrs = (Elf32_Shdr *)((uint8_t *)ctx->map + ehdr->e_shoff);
	ctx->shdrs = shdrs;
	ctx->shnum = ehdr->e_shnum;
	ctx->shstrndx = ehdr->e_shstrndx;
	if (ctx->shstrndx >= ctx->shnum)
		return (-1);
	shstr_hdr = &shdrs[ctx->shstrndx];
	if (!nm_bounds_check(ctx,
			(uint8_t *)ctx->map + shstr_hdr->sh_offset, shstr_hdr->sh_size))
		return (-1);
	ctx->shstrtab = (char *)ctx->map + shstr_hdr->sh_offset;
	i = 0;
	while (i < ctx->shnum)
	{
		sh = &shdrs[i];
		if (sh->sh_type == SHT_SYMTAB)
		{
			if (!nm_bounds_check(ctx,
					(uint8_t *)ctx->map + sh->sh_offset, sh->sh_size))
				return (-1);
			ctx->symtab = (uint8_t *)ctx->map + sh->sh_offset;
			ctx->symtab_size = sh->sh_size;
			ctx->sym_entsize = sh->sh_entsize ? sh->sh_entsize : sizeof(Elf32_Sym);
			if (sh->sh_link < ctx->shnum)
			{
				Elf32_Shdr *str_hdr = &shdrs[sh->sh_link];
				if (!nm_bounds_check(ctx,
						(uint8_t *)ctx->map + str_hdr->sh_offset,
						str_hdr->sh_size))
					return (-1);
				ctx->strtab = (char *)ctx->map + str_hdr->sh_offset;
				ctx->strtab_size = str_hdr->sh_size;
			}
		}
		i++;
	}
	return (0);
}

/* ---------- public entry point ---------- */

int	nm_parse(t_nm_ctx *ctx, const char *filename)
{
	int			fd;
	struct stat	st;
	uint8_t		*ident;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (nm_error("ft_nm", filename, strerror(errno)));
	if (fstat(fd, &st) < 0)
	{
		close(fd);
		return (nm_error("ft_nm", filename, strerror(errno)));
	}
	if (st.st_size < (off_t)EI_NIDENT)
	{
		close(fd);
		return (nm_error("ft_nm", filename, "file too small"));
	}
	ctx->map_size = (size_t)st.st_size;
	ctx->map = mmap(NULL, ctx->map_size, PROT_READ, MAP_PRIVATE, fd, 0);
	close(fd);
	if (ctx->map == MAP_FAILED)
		return (nm_error("ft_nm", filename, strerror(errno)));

	ident = ctx->map;
	/* check ELF magic */
	if (ident[0] != 0x7f || ident[1] != 'E'
		|| ident[2] != 'L' || ident[3] != 'F')
	{
		munmap(ctx->map, ctx->map_size);
		return (nm_error("ft_nm", filename, "file format not recognized"));
	}
	ctx->is_64 = (ident[EI_CLASS] == ELFCLASS64);
	if (ctx->is_64)
	{
		if (ctx->map_size < sizeof(Elf64_Ehdr))
		{
			munmap(ctx->map, ctx->map_size);
			return (nm_error("ft_nm", filename, "file too small"));
		}
		return (parse_shdr64(ctx));
	}
	else
	{
		if (ctx->map_size < sizeof(Elf32_Ehdr))
		{
			munmap(ctx->map, ctx->map_size);
			return (nm_error("ft_nm", filename, "file too small"));
		}
		return (parse_shdr32(ctx));
	}
}
