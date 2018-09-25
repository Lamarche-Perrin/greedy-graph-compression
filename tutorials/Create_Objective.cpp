#include<iostream>
#include "Objective.h"

using namespace std;

/*The Objective class calculate the cost of a partition and of an operation
* Any modification of the objective function must be repercuted on
*   -initialize_objective()
*   -get_objective()
*   -check_objective()
*   -delta_cost()
*/

main(int argc, char* argv[]){
/* 	Create an Objective Object, initalize the Objective
    and verify the initialization */
    // double lambda = 0.5;
	// Objective G("sparse", 3, lambda);
	// G.micro();
    // G.reg_of_R();
    // G.initialize_objective();
    // G.check_objective();



/*  Create an Objective Object, perform Cluster operations,
    update the Objective, check the Partition and the Objective */
    double lambda = 0.5;
	Objective G("sparse", 1, lambda);
	G.micro();
    G.reg_of_R();
    G.initialize_objective();
    for(int i = 0; i < 400; i++){
        cout << "ITERATION " << i << endl;
        if(G.find_cluster()){
            G.delta_cost();
            G.do_operation();
        }
        else{
            G.clean_operator();
        }
        if(!(G.check_all() and G.check_objective())){
            G.print_cost();
            break;
        }
        cout << endl;
    }

/*  Create an Objective Object, perform Cluster operations
    if they reduce the objective -> update the objective */
    // double lambda = 0.5;
	// Objective G("sparse", 3, lambda);
	// G.micro();
    // G.reg_of_R();
    // G.initialize_objective();
    // for(int i = 0; i < 400; i++){
    //     cout << "ITERATION " << i << endl;
    //     if(G.find_cluster()){
    //         G.delta_cost();
    //         if(G.d_cost < 0){
    //             G.do_operation();
    //         }
    //         else{
    //             G.cancel_operation();
    //         }
    //     }
    //     else{
    //         G.clean_operator();
    //     }
    //     if(!(G.check_all() and G.check_objective()))
    //         break;
    //     cout << endl;
    // }


/*  Create an Objective Object and print infos about the partition */
    // double lambda = 0.5;
	// Objective G("sparse", 3, lambda);
	// G.micro();
    // G.reg_of_R();
    // G.initialize_objective();
    // G.print_cost();


/*  Create an Objective Object and print infos about the rectangles */
    // double lambda = 0.5;
	// Objective G("sparse", 3, lambda);
	// G.micro();
    // G.reg_of_R();
    // G.initialize_objective();
    // G.print_cost_rectangles();
}