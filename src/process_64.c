#include "ft_nm.h"

void process_64(t_data *data){
	Elf64_Ehdr *elf_header = data->map; //Definie les 64 premiers octets comme le header;
	Elf64_Shdr *elf_section_header_table = data->map + elf_header->e_shoff; //Debut de la section header table
	
	data->header_info->nb_sections = elf_header->e_shnum; //nombre de sections 
	data->header_info->shstrtab_index = elf_header->e_shstrndx; //index section "annuaire des noms des symboles"
	Elf64_Shdr *shsrtab_header = &elf_section_header_table[data->header_info->shstrtab_index]; //header de la section "annuaire des noms des symboles"
	data->header_info->offset_shstrtab = (char *)data->map + shsrtab_header->sh_offset;
	find_tabs(data);
}