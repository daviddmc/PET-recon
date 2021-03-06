// PETrecon.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdio.h>
#include <math.h>
#include <time.h>

inline double phi(double alpha, double p1x, double dpx, double bx, double dx)
{
	return (p1x + alpha * dpx - bx) / dx;
}

double siddon(double p1x, double p1y, double p2x, double p2y, int Nx, int Ny, double bx, double by, double dx, double dy)
{
	double dpx = p2x - p1x;
	double dpy = p2y - p1y;
	double alphaX0 = (bx - p1x) / dpx;
	double alphaXEnd = ((Nx - 1)*dx + bx - p1x) / dpx;
	double alphaY0 = (by - p1y) / dpy;
	double alphaYEnd = ((Ny - 1)*dy + by - p1y) / dpy;


	double alphaXMin = alphaX0 < alphaXEnd ? alphaX0 : alphaXEnd;
	double alphaXMax = alphaX0 > alphaXEnd ? alphaX0 : alphaXEnd;

	double alphaYMin = alphaY0 < alphaYEnd ? alphaY0 : alphaYEnd;
	double alphaYMax = alphaY0 > alphaYEnd ? alphaY0 : alphaYEnd;

	double alphaMin = alphaXMin > alphaYMin ? alphaXMin : alphaYMin;
	double alphaMax = alphaXMax < alphaYMax ? alphaXMax : alphaYMax;

	int iMin, iMax, jMin, jMax;
	if (p1x < p2x)
	{
		iMin = alphaMin == alphaXMin ? 1 : ceil(phi(alphaMin, p1x, dpx, bx, dx));
		iMax = alphaMax == alphaXMax ? Nx - 1 : floor(phi(alphaMax, p1x, dpx, bx, dx));
	}
	else
	{
		iMax = alphaMin == alphaXMin ? Nx - 2 : floor(phi(alphaMin, p1x, dpx, bx, dx));
		iMin = alphaMax == alphaXMax ? 0 : ceil(phi(alphaMax, p1x, dpx, bx, dx));
	}

	if (p1y < p2y)
	{
		jMin = alphaMin == alphaYMin ? 1 : ceil(phi(alphaMin, p1y, dpy, by, dy));
		jMax = alphaMax == alphaYMax ? Ny - 1 : floor(phi(alphaMax, p1y, dpy, by, dy));
	}
	else
	{
		jMax = alphaMin == alphaYMin ? Ny - 2 : floor(phi(alphaMin, p1y, dpy, by, dy));
		jMin = alphaMax == alphaYMax ? 0 : ceil(phi(alphaMax, p1y, dpy, by, dy));
	}

	int Np = iMax - iMin + jMax - jMin + 2;
	
	double alphaX = p1x < p2x ? (iMin*dx + bx - p1x) / dpx : (iMax*dx + bx - p1x) / dpx;
	double alphaY = p1y < p2y ? (jMin*dy + by - p1y) / dpy : (jMax*dy + by - p1y) / dpy;

	int i = floor(phi(((alphaX < alphaY ? alphaX : alphaY) + alphaMin) / 2.0, p1x, dpx, bx, dx));
	int j = floor(phi(((alphaX < alphaY ? alphaX : alphaY) + alphaMin) / 2.0, p1y, dpy, by, dy));

	double alphaC = alphaMin;
	double alphaXU = dx / fabs(dpx);
	int iU = p1x < p2x ? 1 : -1;
	double alphaYU = dy / fabs(dpy);
	int jU = p1y < p2y ? 1 : -1;
	double dConv = sqrt((p1x - p2x)*(p1x - p2x) + (p1y - p2y)*(p1y - p2y));

	//double* l = new double[Np];
	double d = 0;
	while (true)
	{
		if (alphaX < alphaY)
		{
			//l = (alphaX - alphaC) * dConv;
			d += alphaX - alphaC;
			i += iU;
			alphaC = alphaX;
			alphaX += alphaXU;
			if (i < 0 || i > Nx-2)
				break;
		}
		else 
		{
			//l = (alphaY - alphaC) * dConv;
			d += alphaY - alphaC;
			j += jU;
			alphaC = alphaY;
			alphaY += alphaYU;
			if (j < 0 || j > Ny-2)
				break;
		}
	}
	return d * dConv;
}

double siddon3(double p1x, double p1y, double p1z,double p2x, double p2y, double p2z, int Nx, int Ny, int Nz, double bx, double by, double bz, double dx, double dy, double dz)
{
	double dpx = p2x - p1x;
	double dpy = p2y - p1y;
	double dpz = p2z - p1z;
	double alphaX0 = (bx - p1x) / dpx;
	double alphaXEnd = ((Nx - 1)*dx + bx - p1x) / dpx;
	double alphaY0 = (by - p1y) / dpy;
	double alphaYEnd = ((Ny - 1)*dy + by - p1y) / dpy;
	double alphaZ0 = (bz - p1z) / dpz;
	double alphaZEnd = ((Nz - 1)*dz + bz - p1z) / dpz;


	double alphaXMin = alphaX0 < alphaXEnd ? alphaX0 : alphaXEnd;
	double alphaXMax = alphaX0 > alphaXEnd ? alphaX0 : alphaXEnd;

	double alphaYMin = alphaY0 < alphaYEnd ? alphaY0 : alphaYEnd;
	double alphaYMax = alphaY0 > alphaYEnd ? alphaY0 : alphaYEnd;

	double alphaZMin = alphaZ0 < alphaZEnd ? alphaZ0 : alphaZEnd;
	double alphaZMax = alphaZ0 > alphaZEnd ? alphaZ0 : alphaZEnd;

	double alphaMin = fmax(fmax(alphaXMin, alphaYMin), alphaZMin);
	double alphaMax = fmin(fmin(alphaXMax, alphaYMax), alphaZMax);


	int iMin, iMax, jMin, jMax, kMin, kMax;
	if (p1x < p2x)
	{
		iMin = alphaMin == alphaXMin ? 1 : ceil(phi(alphaMin, p1x, dpx, bx, dx));
		iMax = alphaMax == alphaXMax ? Nx - 1 : floor(phi(alphaMax, p1x, dpx, bx, dx));
	}
	else
	{
		iMax = alphaMin == alphaXMin ? Nx - 2 : floor(phi(alphaMin, p1x, dpx, bx, dx));
		iMin = alphaMax == alphaXMax ? 0 : ceil(phi(alphaMax, p1x, dpx, bx, dx));
	}

	if (p1y < p2y)
	{
		jMin = alphaMin == alphaYMin ? 1 : ceil(phi(alphaMin, p1y, dpy, by, dy));
		jMax = alphaMax == alphaYMax ? Ny - 1 : floor(phi(alphaMax, p1y, dpy, by, dy));
	}
	else
	{
		jMax = alphaMin == alphaYMin ? Ny - 2 : floor(phi(alphaMin, p1y, dpy, by, dy));
		jMin = alphaMax == alphaYMax ? 0 : ceil(phi(alphaMax, p1y, dpy, by, dy));
	}

	if (p1z < p2z)
	{
		kMin = alphaMin == alphaZMin ? 1 : ceil(phi(alphaMin, p1z, dpz, bz, dz));
		kMax = alphaMax == alphaZMax ? Nz - 1 : floor(phi(alphaMax, p1z, dpz, bz, dz));
	}
	else
	{
		kMax = alphaMin == alphaZMin ? Nz - 2 : floor(phi(alphaMin, p1z, dpz, bz, dz));
		kMin = alphaMax == alphaZMax ? 0 : ceil(phi(alphaMax, p1z, dpz, bz, dz));
	}

	int Np = iMax - iMin + jMax - jMin + kMax - kMin + 3;

	double alphaX = p1x < p2x ? (iMin*dx + bx - p1x) / dpx : (iMax*dx + bx - p1x) / dpx;
	double alphaY = p1y < p2y ? (jMin*dy + by - p1y) / dpy : (jMax*dy + by - p1y) / dpy;
	double alphaZ = p1z < p2z ? (kMin*dz + bz - p1z) / dpz : (kMax*dz + bz - p1z) / dpz;

	double tmp = (fmin(fmin(alphaX, alphaY), alphaZ) + alphaMin) / 2.0;

	int i = floor(phi(tmp, p1x, dpx, bx, dx));
	int j = floor(phi(tmp, p1y, dpy, by, dy));
	int k = floor(phi(tmp, p1z, dpz, bz, dz));

	double alphaC = alphaMin;

	double alphaXU = dx / fabs(dpx);
	int iU = p1x < p2x ? 1 : -1;

	double alphaYU = dy / fabs(dpy);
	int jU = p1y < p2y ? 1 : -1;

	double alphaZU = dz / fabs(dpz);
	int kU = p1z < p2z ? 1 : -1;

	double dConv = sqrt((p1x - p2x)*(p1x - p2x) + (p1y - p2y)*(p1y - p2y) + (p1z - p2z)*(p1z - p2z));

	//double* l = new double[Np];
	double d = 0;
	char minCord;
	while (true)
	{
		if (alphaX < alphaY)
			if (alphaX < alphaZ)
				minCord = 'x';
			else
				minCord = 'z';
		else if (alphaY < alphaZ)
			minCord = 'y';
		else
			minCord = 'z';

		if (minCord == 'x')
		{
			//l = (alphaX - alphaC) * dConv;
			d += alphaX - alphaC;
			i += iU;
			alphaC = alphaX;
			alphaX += alphaXU;
			if (i < 0 || i > Nx - 2)
				break;
		}
		else if(minCord == 'y')
		{
			//l = (alphaY - alphaC) * dConv;
			d += alphaY - alphaC;
			j += jU;
			alphaC = alphaY;
			alphaY += alphaYU;
			if (j < 0 || j > Ny - 2)
				break;
		}
		else
		{
			d += alphaZ - alphaC;
			k += kU;
			alphaC = alphaZ;
			alphaZ += alphaZU;
			if (k < 0 || k > Nz - 2)
				break;
		}
	}
	return d * dConv;
}


int main()
{
	double p1x = 0;
	double p1y = 0;
	double p1z = 0;
	double p2x = 70;
	double p2y = 70;
	double p2z = 96;
	int Nx = 141;
	int Ny = 141;
	int Nz = 121;
	double bx = 0;
	double by = 0;
	double bz = 0;
	double dx = 0.5;
	double dy = 0.5;
	double dz = 0.8;
	time_t start, stop;
	start = time(NULL);
	double d;
	for (int i = 0; i < 10; i++)
	{
		//d = siddon(p1x, p1y, p2x, p2y, Nx, Ny, bx, by, dx, dy);
		d = siddon3(p1x, p1y, p1z, p2x, p2y, p2z, Nx, Ny, Nz, bx, by, bz, dx, dy, dz);
	}
	stop = time(NULL);
	printf("%f", d);
	//printf("%d", stop - start);
	return 0;
}