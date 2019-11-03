#include <iostream>
#include <Source\raytracer.h>

#include "ctime"

using namespace std;
//------------------------------------------------------------
//  Main Function
//------------------------------------------------------------
int main(int argc, char *argv[])
{
	clock_t begin = clock();

	RayTracer r = RayTracer();
	r.execute(argv[1], argv[2]);
	clock_t end = clock();
	float elapsed_secs = float(end - begin) / CLOCKS_PER_SEC;
	cout << elapsed_secs << '\n';

	return 0;
}