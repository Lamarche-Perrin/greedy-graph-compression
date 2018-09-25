#include<iostream>
#include "Builder.h"

using namespace std;

/*The Builder class is designed to build partitions
* It contains many tools for debuging
*/

main(int argc, char* argv[]){
	/* Create a Builder Object which does nothing */
    // Builder G;
	// G.macro();
	// G.print_R();
	// G.print_metadata();

	/* Create a Builder Object with a multi dimensional partition */
	// vector<int> N = {5,5};
    // Builder G(N);
	// G.macro();
	// G.reg_of_R();
	// G.print_R();
	// G.print_reg_size();
	// G.print_reg();
	// G.print_metadata();
	// G.print_data();

	/* Create a Builder Object, a micro partition
	and a Full Benchmark Graph of scale 1 */
	// Builder G("full", 1);
	// G.micro();
	// G.reg_of_R();	
	// G.print_R();
	// G.print_metadata();
	// G.print_data();

	/* Create a Builder Object, a micro partition 
	and a Sparse Benchmark Graph of size 3 */
	// Builder G("sparse", 3);
	// G.micro();
	// G.reg_of_R();
	// G.print_R();
	// G.print_metadata();
	// G.print_data();

	/* Create a Builder Object and load data from a csv or a json file */
	// string moreno = "moreno.json";
	// Builder G(moreno);
	// G.micro();
	// G.reg_of_R();
	// G.print_R();
	// G.print_metadata();
	// G.print_data();

	/* Create a Builder Object and a graph built with the Random Matrix Model */
	// vector<double> params = {6, 10, 40, 15, 90, 8, 5};// w a1 b1 a2 b2 shape scale
	// vector<int> N = {5,5};
	// Builder G(N, params);
	// G.macro();
	// G.print_R();
	// G.print_metadata();
	// G.print_data();
}