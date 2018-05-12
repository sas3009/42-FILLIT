/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fhong <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/11 12:31:53 by fhong             #+#    #+#             */
/*   Updated: 2018/05/11 20:48:36 by olmishyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include "fillit.h"
//#include "libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

typedef struct    s_point
{
    int x;
    int y;
}                t_point;

void	ft_putchar(char c)
{
	unsigned char	b;

	b = (unsigned char)c;
	write(1, &b, 1);
}

void	ft_putstr(char const *s)
{
	int	i;

	if (s == 0)
		return ;
	i = 0;
	while (s[i])
	{
		ft_putchar(s[i]);
		i++;
	}
}

void	ft_memdel(void **ap)
{
	if (ap && *ap)
	{
		free(*ap);
		*ap = NULL;
	}
}

char	*ft_strnew(size_t size)
{
	char	*b;

	b = malloc(size + 1);
	if (b == NULL)
		return (NULL);
	while (size + 1)
	{
		b[size] = 0;
		size--;
	}
	return (b);
}

char	**make_map(int size)
{
	char	**map;
	int		i;
	int		j;

	map = (char **)malloc(sizeof(char *) * size);
	i = 0;
	while (i < size)
	{
		map[i] = ft_strnew(size);
		j = 0;
		while (j < size)
		{
			printf("i = %d, j = %d\n", i, j);
			map[i][j] = '.';
			j++;
		}
		i++;
	}
	return (map);
}

void	free_map(char **map)
{
	ft_memdel((void **)map);
}

void	print_map(char **map, int size)
{
	int i;

	i = 0;
	while (i < size)
	{
		ft_putstr(map[i]);
		ft_putchar('\n');
		i++;
	}
}


/*
** check if it is availibe spot in the map
*/
int		check_point(char **map, t_point point)
{
	if (map[point.x][point.y] != '.')
		return (1);
	return (0);
}

/*
** find next empty point
*/

t_point	empty_point(char **map, t_point point)
{
	while (check_point(map, point))
	{
		if (!map[++point.x][point.y])
		{
			point.x = 0;
			point.y++;
		}
	}
	return (point);
}

/*
** i like to move it move it~~~
*/

t_point	move_point(t_point point, int move)
{
	if (move == 1)
		point.y--;
	else if (move == 2)
		point.x++;
	else if (move == 3)
		point.y++;
	else if (move == 4)
		point.x--;
	else
		ft_putstr("wrong hole in movement!!");
	return (point);
}

void	set_tetris(char **map, t_point point, int move, int c)
{
	int 	tmp_move;
	t_point	tmp_point;

	tmp_move = move;
	tmp_point = point;
	while (tmp_move)
	{
		if (check_point(map, point))
			break;
		map[point.x][point.y] = c + 'A';
		move_point(point, tmp_move);
		tmp_move /= 10;
	}
	if (tmp_move != 0)
		while (move != tmp_move)
		{
			map[tmp_point.x][tmp_point.y] = '.';
			move_point(point, move);
			move /= 10;
		}
}

const int		tetris_map[19] =
{
	51015, 123, 1510, 567, 5910, 127, 5610, 456, 4510, 126,
	51011, 345, 1611, 125, 156, 145, 5611, 167, 459
};

const int		move_map[19] =
{
	333, 222, 433, 322, 334, 223, 3312, 3422, 3314, 2243,
	332, 344, 233, 22443, 234, 212, 323, 232, 343
};

int		ft_strlen(char *str)
{
	int i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}


void	*ft_memset(void *b, int c, size_t len)
{
	unsigned char	*dst;

	dst = (unsigned char *)b;
	while (len-- > 0)
		dst[len] = c;
	return (b);
}

/*
** check newline
*/

int 	check_newline(char *str)
{
	int index;
	int length;

	index = 0;
	length = ft_strlen(str);
	while (index < 20)
	{
		if (index % 5 == 4)
			if (str[index] != '\n')
				return (1);
		if (str[index] != '.' && str[index] != '#' && str[index] != '\n')
			return (1);
		index++;
	}
	if (length == 21)
		if (str[20] != '\n')
			return (1);
	return (0);
}

/*
** produce tetris number in our way. Look at tetris.txt
*/

int		my_tetris_num(char *str)
{
	int tetris_num;
	int index;

	tetris_num = 0;
	index = 0;
	while (*str != '#')
		str++;
	while (str[index++])
	{
		if (str[index] == '#')
		{
			if (index < 10)
				tetris_num = tetris_num * 10 + index;
			else
				tetris_num = tetris_num * 100 + index;
		}
	}
	return (tetris_num);
}

/*
**
*/

int		check_tetris(int tetris)
{
	int index;

	index = 0;
	while (tetris != tetris_map[index] && index < 19)
		index++;
	if (index == 19)
		return (0);
	printf("tetris_map: %d\n", tetris_map[index]);
	printf("move map: %d\n", move_map[index]);
	return (move_map[index]);
}

/*
** store every 21 character in buffer
** 4 blocks = 1 tetris
*/

int		*read_tetris(int fd)
{
	char	*buf;
	int		*move;
	int		i;

	buf = ft_strnew(21);
	i = 0;
	if (!(move = (int*)malloc(sizeof(int) * 27)))
	{
		printf("return READ_TETRIS11\n");
		return (0);
	}
	while (read(fd, buf, 21))
	{
		if (check_newline(buf) || !(check_tetris(my_tetris_num(buf))))
		{
			ft_memdel((void **)&buf);
			ft_memdel((void **)&move);
			printf("return READ_TETRIS22\n");
			return (0);
		}
		move[i++] = check_tetris(my_tetris_num(buf));
	}
	move[i] = '\0';
	if (ft_strlen(buf) != 20)
	{
		ft_memdel((void **)&buf);
		printf("Last is not 20!!");
		return (0);
	}
	ft_memdel((void **)&buf);
	printf("move: %p\n", move);
	return (move);
}
int		main(int ac, char **av)
{
	int	*list;
	char **mapy;

	if (ac != 2)
	{
		ft_putstr("usage: fillit input\n");
		return (1);
	}
	if ((list = read_tetris(open(av[1], O_RDONLY))) == NULL)
	{
		ft_putstr("erre\n");
		return (1);
	}
//	while (*list)
	printf("%d\n", list[0]);
//	printf("%d\n", list[1]);
	mapy = make_map(5);
//	printf("mapy --- %s\n", mapy[0]);
	print_map(mapy, 5);
	free_map(mapy);
	return (0);
}
