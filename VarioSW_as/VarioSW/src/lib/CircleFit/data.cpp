#include "data.h"
//



/************************************************************************
			BODY OF THE MEMBER ROUTINES
************************************************************************/
// Default constructor
Data::Data()
{
	write_index = 0;
	n=0;
	X = new reals[n];
	Y = new reals[n];
	for (int i=0; i<n; i++)
	{
		X[i]=0.;
		Y[i]=0.;
	}
}

// Constructor with assignment of the field N
Data::Data(int N)
{
	write_index = 0;
	n=N;
	X = new reals[n];
	Y = new reals[n];

	for (int i=0; i<n; i++)
	{
		X[i]=0.;
		Y[i]=0.;
	}
}

// Constructor with assignment of each field
Data::Data(int N, reals dataX[], reals dataY[])
{ 
	write_index = 0;
	n=N;
	X = new reals[n];
	Y = new reals[n];

	for (int i=0; i<n; i++)
	{
		X[i]=dataX[i];
		Y[i]=dataY[i];
	}
}


// Routine that computes the x- and y- sample means (the coordinates of the centeroid)

void Data::means(void)
{
	meanX=0.; meanY=0.;
	
	for (int i=0; i<n; i++)
	{
		meanX += X[i];
		meanY += Y[i];
	}
	meanX /= n;
	meanY /= n;
}

// Routine that centers the data set (shifts the coordinates to the centeroid)

void Data::center(void)
{
	reals sX=0.,sY=0.;  
	int i;
	
	for (i=0; i<n; i++)
	{
		sX += X[i];
		sY += Y[i];
	}
	sX /= n;
	sY /= n;
	
	for (i=0; i<n; i++)
	{
		X[i] -= sX;
		Y[i] -= sY;
	}
	meanX = 0.;
	meanY = 0.;
}

// Routine that scales the coordinates (makes them of order one)

void Data::scale(void)
{
	reals sXX=0.,sYY=0.,scaling;  
	int i;
	
	for (i=0; i<n; i++)
	{
		sXX += X[i]*X[i];
		sYY += Y[i]*Y[i];
	}
	scaling = sqrt((sXX+sYY)/n/2.0);
	
	for (i=0; i<n; i++)
	{
		X[i] /= scaling;
		Y[i] /= scaling;
	}
}

// Printing routine

void Data::print(void)
{
	//cout << endl << "The data set has " << n << " points with coordinates :"<< endl;
	
	//for (int i=0; i<n-1; i++) cout << setprecision(7) << "(" << X[i] << ","<< Y[i] << "), ";
	
//	cout << "(" << X[n-1] << ","<< Y[n-1] << ")\n";
}

// Destructor
Data::~Data()
{
	delete[] X;
        delete[] Y;
}


