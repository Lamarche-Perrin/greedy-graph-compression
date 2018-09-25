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


#pragma once

#include <functional>
#include <iterator> 


#include "Builder.h"

class Checker : public Builder {
	protected:
		int n_permutations; 		// Total number of possible cells
		int log_n_perm;				// log2 of n_permutations
		int n_id;					// id number of a cell
		int correct;				// correct flag
		int all_correct;			// all correct flag
		std::vector<int> shifts;	// Used for maping N*N*..*N -> N
		std::vector<int> cells;		// Cells in the partitions
		std::vector<int> nnz_cells;	// id of non-zero cells in data
		std::vector<int>::iterator dup;	// Iterator on duplicated cell
		std::vector<int> missing_cells;	// Missing elements
		std::vector< std::vector<int>::iterator> rect_it;	// Iterator on rectangle components
		regit reg_it;		// Iterator on registers
		registre reg_copy;	// Copy of registers
	
	public:
		//using Builder::Builder;
		Checker(std::vector<int> N, std::vector<double> params);
		Checker(std::string input_filename);
		Checker(std::string type, int n);
		Checker(std::vector<int> N);
		Checker();
		~Checker();
		void init(); 

		int check_all();
		int content_of_partition();
		int content_of_rectangles();
		int constraints_on_partition();
		int order_in_registers();
		int constraints_on_registers();
		int rectangles_in_registers();
		int calculated_size_of_partition();
		
		
		int intersect(regit const& vecs);
};


