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

typedef struct s_data{
	void *map;
	struct stat *buff;
	bool is_64_or_32;

} t_data;

void ft_setup(t_data *data, const char *filename);
void ft_check_file(t_data *data, const char *filename);



#endif
