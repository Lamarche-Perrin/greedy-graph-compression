#include<iostream>
#include <chrono>
#include <unistd.h>
#include <cstdlib>

#include "Optimizer.h"

using namespace std;
using namespace std::chrono;

main(int argc, char* argv[]){
	int lambda_flag = 0;
	int niter_flag = 0;
	int T0_flag = 0;
	int cexp_flag = 0;
	int filename_flag = 0;

	double lambda ;
	int niter;
	double T0;
	double cexp;
	string filename;
	
	int c;
	while ((c = getopt(argc, argv, "L:n:T:c:f:")) != -1)
		 switch (c){
		 	case 'L':
		 		lambda = stod(optarg);
		 		lambda_flag = 1;
		 	case 'n':
		 		niter = stoi(optarg);
		 		niter_flag = 1;
		 	case 'T':
		 		T0 = stod(optarg);
		 		T0_flag = 1;
		 	case 'c':
		 		cexp = stod(optarg);
		 		cexp_flag = 1;
		 	case 'f':
			 	filename = optarg;
		 		filename_flag = 1;
		 }




	//Optimizer G(N, params, lambda, niter, T0, cexp);
	Optimizer G(filename, lambda, niter, T0, cexp);
	G.macro();
	G.reg_of_R();
	G.simulated_annealing_RL();
}