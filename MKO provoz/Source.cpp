#include <iostream>
#include <cmath>
#include <string>

#include "data.h"
#include "Func.h"

//PP
int from = 100;
int to = 120;
double U1 = 10;
double U2 = 180;

const int x_size = 200; //km
const int t_size = 5; //h

double dx = 2;
double dt = 1e-3;
double CFL = 0.1;

int nx = x_size / dx;

double v_max = 130;
double rho_max = 200;

data Unew(1, nx);
data Uold(1, nx);

data qe(1, nx-1);
data qw(1, nx-1);

std::string path = "U.txt";

int main()
{
	export_init(path);
	init_func();
	Uold.print_to_file(path);
	double t = 0;
	int ii = 0;
	while (t < t_size)
	{
		dt = Rusanov(t);
		t = t + dt;
		ii++;
		if (!(ii % 30))
		{
			Unew.print_to_file(path);
			std::cout << "time:" << t << std::endl;
		}
	}

	Unew.print_to_file(path);
}