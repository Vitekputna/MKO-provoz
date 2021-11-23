#pragma once
#include <iostream>
#include <cmath>
#include "data.h"

extern double dx , CFL, U1, U2, v_max, rho_max;
extern data Uold ,Unew , qe, qw;
extern int from, to, nx;

void init_func()
{
	for (int i = 0; i < nx; i++)
	{
		if (i * dx > from && i * dx < to)
		{
			Uold(0, i) = U2;
		}
		else
		{
			Uold(0, i) = U1;
		}
	}
}

void export_init(std::string path)
{
	remove(path.c_str());
}

double flux(double U)
{
	return v_max*(U-U*U/rho_max);
}

void boundary(double t)
{

	if (t < 1)
	{
		Unew(0, 0) = U1;
	}
	else
	{	
		Unew(0, 0) = 90;
	}
	Unew(0, nx - 1) = (Unew(0, nx - 2) + Unew(0, nx - 3))/2;
}

double Rusanov(double t)
{
	double dt_max = 100;
	for (int i = 0; i < nx-2; i++)
	{
		qe(0, i) = abs(v_max * (1 - (Uold(0,i) + Uold(0,i)) / rho_max));
		qw(0, i) = abs(v_max * (1 - (Uold(0, i) + Uold(0, i)) / rho_max));

		double q_max = std::max(qe(0, i), qw(0, i));
		dt_max = std::min(dt_max,dx/q_max);
	}
	dt_max = CFL * dt_max;

	for (int i = 1; i < nx-1; i++)
	{
		double Fe = 0.5 * (flux(Uold(0, i)) + flux(Uold(0, i + 1))) - qe(0,i) * (Uold(0, i + 1) - Uold(0, i));
		double Fw = 0.5 * (flux(Uold(0, i)) + flux(Uold(0, i - 1))) - qw(0,i) * (Uold(0, i) - Uold(0, i - 1));

		Unew(0, i) = Uold(0, i) - dt_max / dx * (Fe - Fw);
	}
	boundary(t);
	memcpy(Uold.data_p,Unew.data_p,nx*sizeof(double));
	return dt_max;
}



