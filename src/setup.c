#include "ft_nm.h"

void ft_setup(t_data *data, const char *filename){
    int fd;

    fd = open(filename, O_RDONLY);
    if (fd < 0)
        ft_error(strerror(errno));
    if (fstat(fd, data->buff) < 0){
        close(fd);
        ft_error(strerror(errno));
    }
    if(data->buff->st_size < (off_t)EI_NIDENT){
        close(fd);
        ft_error("file too small");
    }
    data->map = mmap(NULL, (size_t)data->buff->st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if(data->map == MAP_FAILED){
        close(fd);
        ft_error(strerror(errno));
    }
    close(fd);
}

void ft_check_file(t_data *data, const char *filename){
    if (&data->map[0] != 0x7f || &data->map[1] != 'E' || &data->map[2] != 'L' || &data->map[3] != 'F'){
        munmap(data->map, data->buff->st_size);
        ft_error("Not a ELF file");
    }
    data->is_64_or_32 = &data->map[ELFC] 
} 