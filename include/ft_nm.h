#ifndef FT_NM_H
# define FT_NM_H

#include "stdio.h"
#include "unistd.h"
#include "fcntl.h"
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <elf.h>
#include <sys/mman.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct s_elf{
	uint16_t nb_sections;
	/**
	 * Index pour acceder à la section shstrtab
	 * Cette section correspond à la liste des noms
	 */
	uint16_t shstrtab_index;
	/**
	 * Offset qui contient les noms des elements
	 * Appartient au header shstrtrab
	 */
	char *offset_shstrtab;
} t_elf;

typedef struct s_data{
	void *map;
	struct stat buff;
	/**True = 64
	 * False = 32
	 */
	bool is_64_or_32;
	t_elf *header_info;

} t_data;

void ft_setup(t_data *data, const char *filename);
void ft_check_file(t_data *data, const char *filename);

//process_64.c
void process_64(t_data *data);
void process_32(t_data *data);

//utils.c
void ft_error(const char *error);
void find_tabs(t_data *data);

#endif
