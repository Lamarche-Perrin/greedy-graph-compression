#include<iostream>
#include "Checker.h"

using namespace std;

/*The Checker class is designed to check partitions
* It contains many tools for debuging
* Seven verifications can be performed on partitions
* All of them are related to the structure of the partitions and of the registers
* The correctness of the calculus is not verified here
* Neither is the data structure
* Please note that performing those checks takes time
*/

main(int argc, char* argv[]){
	/* Create a Checker Object and do all the possible checks on the partition */
   	// vector<int> N = {5,5};
	// Checker G(N);
	// G.macro();
    // G.reg_of_R();
	// G.build_R(G.R_check);
	// G.content_of_partition();
	// G.content_of_rectangles();
	// G.constraints_on_partition();
	// G.order_in_registers();
	// G.constraints_on_registers();
	// G.rectangles_in_registers();
	// G.calculated_size_of_partition();

	/* Create a Checker Object and do all the possible checks
	on a partition at once */
   	// vector<int> N = {5,5};
	// Checker G(N);
	// G.macro();
	// G.reg_of_R();
	// G.check_all();

	/* Create a Checker Object and do all the possible checks
	on a partition at once with data */
	Checker G("sparse", 1);
	G.macro();
	G.reg_of_R();
	G.check_all();
}