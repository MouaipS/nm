#include "ft_nm.h"

void symbols64(t_data *data){
	Elf64_Sym *actual_symbol;
	size_t nb_symbol = data->ptr_symtab->size / data->ptr_symtab->symbole_size;
	data->sym_array = malloc(sizeof(t_sym) * nb_symbol);
	size_t i = 0;
	while(i < nb_symbol){
		actual_symbol = (Elf64_Sym *)((uint8_t *)data->ptr_symtab->symtab + i * data->ptr_symtab->symbole_size);
		int stype = ELF64_ST_TYPE(actual_symbol->st_info);
		if (stype == STT_FILE || stype == STT_SECTION)
			continue ;
		const char *name = data->ptr_symtab->strtab + actual_symbol->st_name;
		printf("aaa%s\n", name);
		i++;
	}
}

void process_64(t_data *data){
	Elf64_Ehdr *elf_header = data->map; //Definie les 64 premiers octets comme le header;
	Elf64_Shdr *elf_section_header_table = data->map + elf_header->e_shoff; //Debut de la section header table
	
	data->header_info->nb_sections = elf_header->e_shnum; //nombre de sections 
	data->header_info->shstrtab_index = elf_header->e_shstrndx; //index section "annuaire des noms des symboles"
	Elf64_Shdr *shsrtab_header = &elf_section_header_table[data->header_info->shstrtab_index]; //header de la section "annuaire des noms des symboles"
	data->header_info->offset_shstrtab = (char *)data->map + shsrtab_header->sh_offset;
	find_tabs(data, elf_section_header_table);
	parse_symbol(data);
}