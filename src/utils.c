#include "ft_nm.h"

void find_tabs(t_data *data, Elf64_Shdr *elf_header){
	int i = 0;
	Elf64_Shdr *tmpShdr;
	Elf64_Shdr *header_string_table;
	while(i < data->header_info->nb_sections){
		tmpShdr = &elf_header[i];
		if(tmpShdr->sh_type == SHT_SYMTAB){
			data->ptr_symtab->symtab = data->map + tmpShdr->sh_offset;
			data->ptr_symtab->symbole_size = tmpShdr->sh_size;
			if(tmpShdr->sh_entsize == 0) //taille du fichier erronee
				data->ptr_symtab->symbole_size = sizeof(Elf64_Shdr);
			data->ptr_symtab->symbole_size = tmpShdr->sh_entsize;
			if(tmpShdr->sh_link > data->header_info->nb_sections){
				header_string_table = data->header_info->offset_shstrtab[tmpShdr->sh_link];
				data->ptr_symtab->strtab = data->map + header_string_table->sh_offset;
				data->ptr_symtab->strtab_size = tmpShdr->sh_size;
			}
		}
		i++;
	}
}

void ft_error(const char *error){
	printf("Error : %s\n", error);
	exit(0);
}