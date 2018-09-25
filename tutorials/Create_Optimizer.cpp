#include<iostream>
#include "Optimizer.h"

using namespace std;

/*The Optimizer run optimization functions 
* 
*/

main(int argc, char* argv[]){
	/* Create an Optimizer Object and run a Blind Gradient Descente Optimization
    with data output */
    // double lambda = 0.5;
    // int nfail_limit = 100;
	// Optimizer G("sparse", 3, lambda);
	// G.micro();
    // G.reg_of_R();
    // G.gradient_descente_while_cluster(nfail_limit);

	/* Create an Optimizer Object and run a Blind Gradient Descente Optimization
    without data output */
    // double lambda = 0.5;
    // int nfail_limit = 100;
	// Optimizer G("sparse", 3, lambda);
	// G.micro();
    // G.reg_of_R();
    // G.gradient_descente_while_cluster_succinct(nfail_limit);

	/* Create an Optimizer Object and run a Mixed program
    with data output */
    double lambda = 10.0;
    int nfail_limit = 100;
    int ndiv_start = 5;
    int ndiv_increm = 5;
    int ncycle = 10;
	Optimizer G("sparse", 3, lambda);
	G.micro();
    G.reg_of_R();
    G.set_parameters(nfail_limit, ndiv_start, ndiv_increm, ncycle);
    G.mix_program();


	/* Create an Optimizer Object and run a Mixed program
    without data output */
    // double lambda = 0.5;
    // int nfail_limit = 100;
    // int ndiv_start = 5;
    // int ndiv_increm = 5;
    // int ncycle = 10;
	// Optimizer G("sparse", 3, lambda);
	// G.micro();
    // G.reg_of_R();
    // G.set_parameters(nfail_limit, ndiv_start, ndiv_increm, ncycle);
    // G.mix_program_succinct();
}