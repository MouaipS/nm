#include "ft_nm.h"

static void process_nm(const char *filename, int jsp){
	t_data data;
	(void)jsp;
	data.header_info = malloc(sizeof(t_elf));
    data.ptr_symtab = malloc(sizeof(t_symtab));

	ft_setup(&data, filename);
	ft_check_file(&data, filename);
	if(data.is_64_or_32 == true)
		process_64(&data);
	else
		process_32(&data);
}

int main(int ac, char **av){
	if(ac < 2){
		process_nm("a.out", 0);
	} else{
		for(int i = 0; i < ac; i++){
			process_nm(av[i], 0);
		}
	}
	printf("tout va bien\n");
	return(0);
}