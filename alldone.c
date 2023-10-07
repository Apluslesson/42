#ifndef HEADER_H
# define HEADER_H

# include <unistd.h>

int	is_valid(char number, char grid[9][9], int row, int column);
int	solve_cell(char grid[9][9], int i, int j);
void	build_table(char **argv, char grid[9][9]);
int	ft_strlen(char *str);
int	line_is_valid(char *str);
int	input_is_valid(int argc, char **argv);
void	print_table(char grid[9][9]);

#endif

//main function

int	main(int argc, char **argv) //This function takes argc as an argument count and argv as a parameter
{
	char	grid[9][9]; //declaring a 9x9 array to represent the grid

	if (!(input_is_valid(argc, argv))) //checks whether the input agurments are valid using input_is_valid
	{
		write(1, "Error\n", 6); //returns Error if input is not valid
		return (0); //return 0 to indicate an error
	}
	build_table(argv + 1, grid); //calls the build_table function to fill in the grid
	solve_cell(grid, 0, 0); //starting from top-left corner (0, 0) of the grid
	print_table(grid); //prints grid by calling the print_table function
	return (0); //if is successful it should return 0 which means no errors
}

// input function

int	ft_strlen(char *str)
{
	int l;

	l = 0;
	while (*str)
	{
		l++;
		str++;
	}
	return (l);
}

int	line_is_valid(char *str)
{
	if (ft_strlen(str) != 9)
	{
		return (0);
	}
	while (*str)
	{
		if (('1' <= *str && *str <= '9') || *str == '.')
			str++;
		else
			return (0);
	}
	return (1);
}

int	input_is_valid(int argc, char **argv)
{
	int	i;

	if (argc != 10)
	{
		return (0);
	}
	i = 1;
	while (i < 10)
	{
		if (!(line_is_valid(argv[i])))
		{
			return (0);
		}
		i++;
	}
	return (1);
}

// cell function

int	is_valid(char number, char grid[9][9], int row, int column)
{
	int i;
	int row1;
	int row2;
	int col1;
	int col2;

	i = 0;
	row1 = (row + 2) % 3;
	row2 = (row + 4) % 3;
	col1 = (column + 2) % 3;
	col2 = (column + 4) % 3;
	while (i < 9)
		if (grid[i][column] == number || (grid[row][i++] == number))
			return (0);
	if (grid[row1 + (3 * (row / 3))][col1 + (3 * (column / 3))] == number)
		return (0);
	if (grid[row2 + (3 * (row / 3))][col1 + (3 * (column / 3))] == number)
		return (0);
	if (grid[row1 + (3 * (row / 3))][col2 + (3 * (column / 3))] == number)
		return (0);
	if (grid[row2 + (3 * (row / 3))][col2 + (3 * (column / 3))] == number)
		return (0);
	return (1);
}

int	solve_cell(char grid[9][9], int i, int j)
{
	char	n;

	if (((n = '0') || 1) && (i == 9))
		return (1);
	if (grid[i][j] != '0')
	{
		if (j == 8 && (solve_cell(grid, i + 1, 0)))
			return (1);
		else if (solve_cell(grid, i, j + 1))
			return (1);
		return (0);
	}
	while (n <= '8')
		if ((n++ || 1) && is_valid(n, grid, i, j))
		{
			grid[i][j] = n;
			if (j == 8 && (solve_cell(grid, i + 1, 0)))
				return (1);
			else if (solve_cell(grid, i, j + 1))
				return (1);
			grid[i][j] = '0';
		}
	return (0);
}

void	build_table(char **argv, char grid[9][9])
{
	int i;
	int j;

	i = 0;
	while (i < 9)
	{
		j = 0;
		while (j < 9)
		{
			if (argv[i][j] == '.')
				grid[i][j] = '0';
			else
				grid[i][j] = argv[i][j];
			j++;
		}
		i++;
	}
}

void	print_table(char grid[9][9])
{
	int		i;
	int		j;

	i = 0;
	while (i < 9)
	{
		j = 0;
		while (j < 8)
		{
			write(1, &grid[i][j], 1);
			write(1, " ", 1);
			j++;
		}
		write(1, &grid[i][j], 1);
		write(1, "\n", 1);
		i++;
	}
}

