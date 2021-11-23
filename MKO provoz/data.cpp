#include "data.h"
#include <fstream>

data::data()
{
	rows = 0;
	collums = 0;
	data_p = NULL;
}

data::data(int num_row, int num_coll) : rows(num_row), collums(num_coll)
{
	data_p = reinterpret_cast<double*>(calloc(rows * collums, sizeof(double)));
}

data::data(int num_row, int num_coll, std::string name) : rows(num_row), collums(num_coll), name(name)
{
	data_p = reinterpret_cast<double*>(calloc(rows * collums, sizeof(double)));
}

data::~data()
{
	free(data_p);
}

data::data(const data& other)
{
	rows = other.rows;
	collums = other.collums;
	data_p = reinterpret_cast<double*>(malloc(rows * collums * sizeof(double)));

	if (data_p)
	{
		std::memcpy(data_p, other.data_p, rows * collums * sizeof(double));
	}
}

void data::operator=(data& other)
{
	rows = other.rows;
	collums = other.collums;
	//data_p = reinterpret_cast<double*>(malloc(rows * collums * sizeof(double)));

	if (data_p)
	{
		std::memcpy(this->data_p, other.data_p, rows * collums * sizeof(double));
	}
}

double& data::operator()(int Row, int Col)
{
	return data_p[Row * collums + Col];
}

std::shared_ptr<data> data::operator+(data& other)
{
	std::shared_ptr<data> output = std::make_shared<data>(other.rows, other.collums);

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < collums; j++)
		{
			output->get(i, j) = this->get(i, j) + other.get(i, j);
		}
	}

	return output;
}

std::shared_ptr<data> data::operator*(double scalar)
{
	std::shared_ptr<data> output = std::make_shared<data>(rows, collums);

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < collums; j++)
		{
			output->get(i, j) = scalar * this->get(i, j);
		}
	}

	return output;
}

double& data::get(int row, int collum)
{
	return data_p[row * collums + collum];
}

std::shared_ptr<data> data::collum(int collum)
{
	std::shared_ptr<data> copied_coll = std::make_shared<data>(1, collums);

	for (int i = 0; i < rows; i++)
	{
		copied_coll->data_p[i] = data_p[i * rows + collum];
	}

	return copied_coll;
}

std::shared_ptr<data> data::row(int row)
{
	//data copied_row(1, collums);
	std::shared_ptr<data> copied_row = std::make_shared<data>(1, collums);

	double* dest = copied_row->data_p;
	double* src = data_p + row * collums * sizeof(double);

	memcpy(dest, src, collums * sizeof(double));

	return copied_row;
}


void data::print_to_file(int rows_to_print, int num_rows)
{
	std::ofstream file(name);

	int per_row = num_rows / rows_to_print;

	if (!per_row)
	{
		per_row = 1;
	}

	for (int t = 0; t < rows_to_print; t++)
	{
		for (int i = 0; i < collums; i++)
		{
			file << this->get(t, i) << " ";
		}
		file << "\n";
	}
	file.close();
}

void data::print_to_file(int per_row)
{
	std::ofstream file(name);


	for (int t = 0; t * per_row < rows; t++)
	{
		for (int i = 0; i < collums; i++)
		{
			file << this->get(t * per_row, i) << " ";
		}
		file << "\n";
	}
	file.close();
}

void data::print_to_file(std::string path)
{
	std::ofstream file(path, std::ios_base::app);

	int r = 1;
	for (int i = 0; i < rows * collums; i++)
	{
		file << this->data_p[i] << " ";

		if (i == (collums * r - 1))
		{
			file << "\n";
			r++;
		}
	}
	file.close();
}