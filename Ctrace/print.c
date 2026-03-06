#include "ft_nm.h"

/*
** nm output format:
**   64-bit: 16 hex digits  space  letter  space  name
**   32-bit:  8 hex digits  space  letter  space  name
**   undefined symbols:     spaces          letter  space  name
*/
void	nm_print(const t_sym *syms, size_t count, int is_64)
{
	size_t		i;
	const t_sym	*s;
	int			width;
	char		buf[32];

	width = is_64 ? 16 : 8;
	i = 0;
	while (i < count)
	{
		s = &syms[i];
		if (!s->has_value
			|| s->letter == 'U' || s->letter == 'u'
			|| s->letter == 'w')
		{
			/* undefined / weak-undefined: print spaces instead of address */
			int j = 0;
			while (j < width)
			{
				write(1, " ", 1);
				j++;
			}
		}
		else
		{
			/* format address */
			if (is_64)
				snprintf(buf, sizeof(buf), "%016llx",
					(unsigned long long)s->value);
			else
				snprintf(buf, sizeof(buf), "%08lx",
					(unsigned long)s->value);
			write(1, buf, width);
		}
		write(1, " ", 1);
		write(1, &s->letter, 1);
		write(1, " ", 1);
		write(1, s->name, strlen(s->name));
		write(1, "\n", 1);
		i++;
	}
}
