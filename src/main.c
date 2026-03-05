#include "ft_nm.h"

void process_nm(const char *filename, int jsp){
	t_data *data;
	ft_setup(&data, filename);
}


int main(int ac, char **av){
	if(ac < 2){
		process_nm("a.out", 0);
	} else{
		for(int i = 0; i < ac; i++){
			process_nm(av[i], 0);
		}
	}
	return(0);
}