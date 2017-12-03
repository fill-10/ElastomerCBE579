#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <assert.h>
using namespace std;
#include "Read.h"


void Read::openFile()
{
	stream = fopen( filename.c_str(), "r" );
	assert(stream);
}

vector<double> Read::getLine()
{
	vector<double> doubles;
	double x,y,z;
    int num1;
	num1 = fscanf( stream, "%lf\t" "%lf\t" "%lf\n", &x, &y, &z);
	doubles.push_back(x);
	doubles.push_back(y);
	doubles.push_back(z);
	return doubles;
}

vector<double> Read::getLine6()
{
    vector<double> doubles;
    double x,y,z, n1, n2, n3;
    int num1;
	num1 = fscanf( stream, "%lf\t" "%lf\t" "%lf\t" "%lf\t", &x, &y, &z, &n1);
	doubles.push_back(x);
	doubles.push_back(y);
	doubles.push_back(z);
	doubles.push_back(n1);
	for (int i = 0; i <n1; i++) {
        num1 = fscanf(stream, "%lf\t" , &n2);
        doubles.push_back(n2);
	}

	return doubles;
}



double Read::getDouble()
{
	double rv;
	fscanf( stream, "%lf\n", &rv );
	return rv;
}

int Read::getInt()
{
	int rv;
	fscanf( stream, "%d\n", &rv);
	return rv;
}

void Read::skipLine()
{
	char junk[15];
	fscanf( stream, "%s\n", junk );
}

void Read::closeFile()
{
	if(stream) fclose( stream );
}
