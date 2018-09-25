/*
 * This file is part of Greedy Graph Compression.
 *
 * Greedy Graph Compression is a C++ implementation of a greedy algorithm
 * solving the graph compression problem (GCP). Several snippets are
 * provided in order to introduce the different classes of the project.
 * For details regarding the GCP, see <https://arxiv.org/abs/1807.06874>.
 * 
 * Copyright © 2018 Léonard Panichi
 * (<Leonard.Panichi@gmail.com>)
 * 
 * Greedy Graph Compression is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 * 
 * Greedy Graph Compression is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */


#include<iostream>
#include "Compressor.h"

using namespace std;

/*The Objective class calculate the cost of a partition and of an operation
* 
*/

main(int argc, char* argv[]){
	/* Create an Compressor Object and run a research of compression
    scale with checks */
    double lambda = 10.0;
    double gap = 1.0;
    double nsearch = 1;


    int nfail_limit = 100;
    int ndiv_start = 5;
    int ndiv_increm = 5;
    int ncycle = 10;


	Compressor G("sparse", 1, lambda, gap, nsearch);
	G.micro();
    G.reg_of_R();
    G.set_parameters(nfail_limit, ndiv_start, ndiv_increm, ncycle);
    G.dichotomic_search();

	/* Create an Compressor Object and run a research of compression
    scale with checks */
    // double lambda = 10.0;
    // double gap = 1.0;
    // double nsearch = 1;


    // int nfail_limit = 100;
    // int ndiv_start = 5;
    // int ndiv_increm = 5;
    // int ncycle = 10;


	// Compressor G("sparse", 1, lambda, gap, nsearch);
	// G.micro();
    // G.reg_of_R();
    // G.set_parameters(nfail_limit, ndiv_start, ndiv_increm, ncycle);
    // G.dichotomic_search();

    return 0;
}