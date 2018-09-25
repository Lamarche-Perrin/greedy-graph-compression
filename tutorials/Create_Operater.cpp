#include <iostream>
#include "Operator.h"

using namespace std;

/*The Operator class modifies partitions
* An operation is done in two steps :
* 1 - drawing a rectangle and calculating the rectangles in the partition that are intersectiong with it
* 2 - applying the rectangle
*/

main(int argc, char* argv[]){
	/* Create an Operator Object and apply a Cluster operator */
   	// vector<int> N = {5,5};
	// Operator G(N);
	// G.macro();
    // G.reg_of_R();
    // if(G.find_cluster())
    //     G.doCluster();
    // else
    //     G.clean_operator();
    // G.check_all();
    // G.print_R();


    /* Create an Operator Object and shuffle the partition */
   	// vector<int> N = {5,5};
	// Operator G(N);
	// G.macro();
    // G.reg_of_R();
    // G.shuffle_partition(30);
    // G.check_all();

    /* Create an Operator Object, apply Cluster Operator Several Times,
    and verify that the calculus is correct. */
   	// vector<int> N = {5,5};
	// Operator G(N);
	// G.macro();
    // G.reg_of_R();
    // for(int i = 0; i < 200; i++){
    //     cout << "ITERATION : " << i << endl;
    //     if(G.find_cluster())
    //         G.doCluster();
    //     else
    //         G.clean_operator();
    //     if(!G.check_all())
    //         break;
    //     cout << endl;
    // }


    /* Create an Operator Object, apply Division and Constrained 
    Cluster Operator Several Times,
    and verify that the calculus is correct. */
   	// vector<int> N = {5,5};
	// Operator G(N);
	// G.macro();
    // G.reg_of_R();
    // for(int i = 0; i < 200; i++){
    //     cout << "ITERATION : " << i << endl;
    //     cout << "DIVISION" << endl;
    //     if(G.find_division())
    //         G.doCluster();
    //     else
    //         G.clean_operator();
    //     if(!G.check_all())
    //         break;
    //     cout << endl;
    //     cout << "UNION" << endl;
    //     if(G.find_union_constrained())
    //         G.doCluster();
    //     else
    //         G.clean_operator();
    //     if(!G.check_all())
    //         break;
    //     cout << endl;
    // }
}