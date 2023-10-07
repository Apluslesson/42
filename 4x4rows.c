#ifndef INCLUDE_H
# define INCLUDE_H

# define UNASSIGNED 0
# define TRUE 1
# define FALSE 0

enum	e_view_point
{
	col_up,
	col_down,
	row_left,
	row_right
};
typedef enum e_view_point	t_view_point;
typedef int**				t_board;
typedef int**				t_views;

int		solve(t_board board, t_views views, int *slv);
int		find_next_unassigned(t_board board, int *row, int *col);
int		is_alone(t_board board, int bfloor, int row, int col);
int		check_viewpoints(t_board board, t_views views);
int		check_line(t_board board, t_view_point view, int view_line,
			int building_viewed);
t_views	parse_arg(char *arg);
int		**init_square(int size);
int		**dup_square(int **square);
void	destroy_square(int **square);
void	print_square(t_board board);
int		get_with_view(t_board board, t_view_point view, int view_line,
			int line_index);
void	ft_putchar(char c);
void	ft_putnbr(int nb);
int		check_arg(char *str);

#endif

#include <unistd.h>
//#include "include.h"
#include <stdlib.h>
//---------------------------------------------------------------
int	main(int argc, char **argv)
{
	t_views views;
	t_board board;
	int		solved;

	if (argc != 2)
	{
		write(1, "Error\n", 6);
		return (0);
	}
	if (!check_arg(argv[1]))
	{
		write(1, "Error\n", 6);
		return (0);
	}
	views = parse_arg(argv[1]);
	board = init_square(4);
	solved = FALSE;
	solve(board, views, &solved);
	if (!solved)
		write(1, "Error\n", 6);
	return (0);
}

//---------------------------------------------------------------------
/*
** Parse the program's first argument where each row is in order
** col up, col down, row left, row right
*/

t_views	parse_arg(char *arg)
{
	int		i;
	int		j;
	t_views views;

	views = init_square(4);
	i = 0;
	j = 0;
	while (i < 16)
	{
		if (arg[i] == ' ')
		{
			arg++;
			continue;
		}
		views[j][i % 4] = arg[i] - '0';
		if (i % 4 == 3)
			j++;
		i++;
	}
	return (views);
}

/*
** Allocate memory for a 2D array(square) of `size`
** initialize each cell to 0.
*/

int		**init_square(int size)
{
	int i;
	int j;
	int **square;

	square = malloc(sizeof(int*) * size);
	i = 0;
	while (i < size)
	{
		square[i] = malloc(sizeof(int) * size);
		j = 0;
		while (j < size)
			square[i][j++] = 0;
		i++;
	}
	return (square);
}

/*
** Duplicate the square, create an empty clone
** and copy each element of `square` in it.
*/

int		**dup_square(int **square)
{
	int **clone;
	int	i;
	int	j;

	clone = init_square(4);
	i = 0;
	while (i < 4)
	{
		j = 0;
		while (j < 4)
		{
			clone[i][j] = square[i][j];
			j++;
		}
		i++;
	}
	return (clone);
}

/*
** Free each row of the square and the square itself.
*/

void	destroy_square(int **square)
{
	int i;

	i = 0;
	while (i < 4)
		free(square[i++]);
	free(square);
}

/*
** Print each row followed by a line break
** and each element of the row but the last followed by a space.
*/

void	print_square(t_board board)
{
	int	i;
	int	j;

	i = 0;
	while (i < 4)
	{
		j = 0;
		while (j < 4)
		{
			ft_putnbr(board[i][j]);
			j++;
			if (j != 4)
				ft_putchar(' ');
		}
		ft_putchar('\n');
		i++;
	}
}

//-----------------------------------------------------------------------------
/*
** Find all the sudoku 4x4 grid recursively
** print the one that checks out with the given viewpoints
*/

int	solve(t_board board, t_views views, int *solved)
{
	int		row;
	int		col;
	int		i;
	t_board	board_clone;

	if (!find_next_unassigned(board, &row, &col))
		return (TRUE);
	i = 0;
	while (++i <= 4)
	{
		if (!is_alone(board, row, col, i))
			continue ;
		board_clone = dup_square(board);
		board_clone[row][col] = i;
		if (solve(board_clone, views, solved)
			&& check_viewpoints(board_clone, views))
		{
			*solved = TRUE;
			print_square(board_clone);
			destroy_square(board_clone);
			return (TRUE);
		}
		destroy_square(board_clone);
	}
	return (FALSE);
}

/*
** Move `row` and `col` to the next unassigned(== 0) position
** returns FALSE if the board is already filled with number, TRUE otherwise
*/

int	find_next_unassigned(t_board board, int *row, int *col)
{
	*row = 0;
	while (*row < 4)
	{
		*col = 0;
		while (*col < 4)
		{
			if (board[*row][*col] == UNASSIGNED)
				return (TRUE);
			(*col)++;
		}
		(*row)++;
	}
	return (FALSE);
}

/*
** Check if `building_floor` is already is unique in the row and column
*/

int	is_alone(t_board board, int row, int col, int building_floor)
{
	int i;

	i = 0;
	while (i < 4)
		if (board[row][i++] == building_floor)
			return (FALSE);
	i = 0;
	while (i < 4)
		if (board[i++][col] == building_floor)
			return (FALSE);
	return (TRUE);
}

/*
** Checks if the grid is valid according to the viewpoints
*/

int	check_viewpoints(t_board board, t_views views)
{
	t_view_point	view;
	int				j;

	view = col_up;
	while (view <= row_right)
	{
		j = 0;
		while (j < 4)
		{
			if (!check_line(board, view, j, views[view][j]))
				return (FALSE);
			j++;
		}
		view++;
	}
	return (TRUE);
}

/*
** Returns TRUE if the number buildings viewed
** with some viewpoint on a line are equal to `building_viewed`.
*/

int	check_line(t_board board, t_view_point view, int view_line,
				int building_viewed)
{
	int	i;
	int	tmp_building_floor;

	i = 0;
	while (i < 4)
	{
		tmp_building_floor = get_with_view(board, view, view_line, i);
		if (tmp_building_floor == UNASSIGNED)
			return (FALSE);
		building_viewed--;
		while (i + 1 < 4 && tmp_building_floor > get_with_view(
					board, view, view_line, i + 1))
			i++;
		i++;
	}
	return (building_viewed == 0);
}

//------------------------------------------------------------------------------
void	ft_putchar(char c)
{
	write(1, &c, 1);
}

void	ft_putnbr(int nb)
{
	unsigned int	p_nb;

	p_nb = nb;
	if (nb < 0)
	{
		write(1, "-", 1);
		p_nb = -nb;
	}
	if (p_nb > 9)
		ft_putnbr(p_nb / 10);
	ft_putchar(p_nb % 10 + '0');
}

int		get_with_view(t_board board, t_view_point view, int view_line,
						int line_index)
{
	if (view == row_left)
		return (board[view_line][line_index]);
	if (view == row_right)
		return (board[view_line][3 - line_index]);
	if (view == col_up)
		return (board[line_index][view_line]);
	if (view == col_down)
		return (board[3 - line_index][view_line]);
	return (-1);
}

int		check_arg(char *str)
{
	int		i;

	i = 0;
	while (str[i])
	{
		if (!(str[i] >= '1' && str[i] <= '4'))
			return (FALSE);
		i++;
		if (!str[i])
			break ;
		if (str[i] != ' ')
			return (FALSE);
		i++;
	}
	if (i != 31)
		return (FALSE);
	return (TRUE);
}

