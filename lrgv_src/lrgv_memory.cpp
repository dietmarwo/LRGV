/*
Copyright (C) 2008-2013 Joseph Kasprzyk, Patrick Reed, Brian Kirsch, Greg Characklis and others.

LRGV is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

LRGV is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the LRGV.  If not, see <http://www.gnu.org/licenses/>.
*/


#include <global.h>

using namespace std;

void general_1d_allocate(double *(&data), int length)
{
	data = new double [length];

	for (int i = 0; i < length; i++) data[i] = 0.0;

	return;
}

void general_1d_allocate(char *(&data), int length)
{
	data = new char [length];

	return;
}

void general_1d_allocate(int *(&data), int length)
{
	data = new int [length];

	for (int i = 0; i < length; i++) data[i] = 0;

	return;
}

void general_2d_allocate(double **&data, int rows, int cols)
{
	data = new double *[rows];
	for (int row_it = 0; row_it<rows; row_it++) data[row_it] = new double[cols];

	for (int row_it = 0; row_it < rows; row_it++)
	{
		for (int col_it = 0; col_it < cols; col_it++)
		{
			data[row_it][col_it] = 0.0;
		}
	}
}

void general_2d_allocate(int **(&data), int rows, int cols)
{
	data = new int *[rows];
	for (int row_it = 0; row_it<rows; row_it++) data[row_it] = new int[cols];

	for (int row_it = 0; row_it < rows; row_it++)
	{
		for (int col_it = 0; col_it < cols; col_it++)
		{
			data[row_it][col_it] = 0;
		}
	}
}

void zap(double **(&data), int rows)
{
	if (data != NULL)
	{
		for (int row_it = 0; row_it < rows; row_it++)
		{
			delete [] data[row_it];
		}
		delete [] data;
		data = NULL;
	}
	return;
}

void zap(double *(&data))
{
	if (data != NULL)
	{
		delete [] data;
		data = NULL;
	}

	return;
}

void zap(char *(&data))
{
	delete data;
	return;
}

void zap(int *(&data))
{
	if (data != NULL)
	{
		delete [] data;
		data = NULL;
	}

	return;
}

void zap(int **(&data), int rows)
{
	if (data != NULL)
	{
		for (int row_it = 0; row_it < rows; row_it++)
		{
			delete [] data[row_it];
		}

		delete [] data;
		data = NULL;
	}
	return;
}
