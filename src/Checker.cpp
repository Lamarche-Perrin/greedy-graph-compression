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


#include "Checker.h"
using namespace std;

//Constructor for Checker with Data from a Random Matrix Model
Checker::Checker(vector<int> N, vector<double> params) : Builder(N, params){
	init();
}

//Constructor for Checker with Data imported from a file
Checker::Checker(string input_filename) : Builder(input_filename){
	init();
}

//Constructor for Checker with Benchmark Data
Checker::Checker(string type, int n) : Builder(type, n){
	init();
}

//Constructor for Checker without Data
Checker::Checker(std::vector<int> N) : Builder(N){
	init();
}

//Default Constructor
Checker::Checker(){}

Checker::~Checker(){
}

//Initialization function of the class attributes
void Checker::init(){
	n_permutations = accumulate(N.begin(), N.end(), 1, multiplies<>());
	log_n_perm = log2((double)n_permutations);
	shifts.resize(D, 1);
	rect_it.resize(D);
	reg_it.resize(D);
	cells.reserve(n_permutations);

	if(data_tool == nullptr){
		nnz_cells.resize(n_permutations);
		iota(nnz_cells.begin(), nnz_cells.end(), 0);
	}
	else
		data_tool->get_nnz(nnz_cells);
}

//Perform all the verifications
int Checker::check_all(){
	build_R(R_check);
	if(!content_of_partition())
		return 0;
	if(!content_of_rectangles())
		return 0;
	if(!constraints_on_partition())
		return 0;
	if(!order_in_registers())
		return 0;
	if(!constraints_on_registers())
		return 0;
	if(!rectangles_in_registers())
		return 0;
	if(!calculated_size_of_partition())
		return 0;
	return 1;
}

//Compare R and R_check. R_check must be built with build_R()
int Checker::content_of_partition(){
	cout << "***Checking content of partition vector" << endl;
	correct = 0;
	if(R.size() != R_check.size()){
		cout << " BAD - Sizes of vectors R and R_check are not equal" << endl;
		correct = 1;
	}
	else{
		if(!equal(R.begin(), R.end(), R_check.begin())){
			cout << "BAD - Content of partitions vectors differs" << endl;
			cout << "R : " << endl;
			print(R);
			cout << "Rcheck : " << endl;
			print(R_check);
			correct = 1;
		}
	}
	if(correct == 0){
		cout << "OK - Partition vector is fine" << endl;
		return 1;	
	}
	else
		return 0;

}

//Verifies that the rectangles don't have nullptr and have the good dimensions
//Basically it just verifies that rectangles are well constructed
int Checker::content_of_rectangles(){
	cout << "***Checking content of rectangles vector" << endl;
	correct = 0;
	for(Rectangle* r : R){
		if(r != nullptr){
			if((*r->rectangle).size() != D){
				cout << "BAD - Wrong size of rectangle in " << r << endl;
				cout << "(*r->rectangle).size()  " << (*r->rectangle).size() << "  expected " << D << endl;
				correct = 1;
				break;
			}
			else{
				for(int d = 0; d < D; d++){
					if((*r->rectangle)[d] == nullptr){
						cout << "nullptr in rectangle  " << r << "   on dim " << d << endl;
						correct = 1;
					}
				}
			}
		}
		else{
			cout << "nullptr in R found by content_of_rectangles()" << endl;
		}
	}
	if(correct == 0){
		cout << "OK - Rectangles all fine" << endl;
		return 1;
	}
	else
		return 0;
}

//Verifies that the constraints on the partition are verified
int Checker::constraints_on_partition(){
	cout <<"***Check constrains on partition"  << endl;
	if(nnz_cells.size() > 10e7){
		cout << "Carefull, you are working on more than 10e7 cells" << endl;
	}
	cells.clear();

	for(int d = 1; d < D; d++)
		shifts[d] =  accumulate(N.begin(), N.begin()+d, 1, multiplies<>());

	// Iterate over rectangles and calculate for each 
	// the cross product of the components
	for(Rectangle* r : R){
		if(r != nullptr){
			// Set an iterator at the begining of each dimension of the rectangle
			for(int d = 0; d < D; d++){
				rect_it[d] = (*r->rectangle)[d]->begin();
			}
			// Iterate over cells
			for(int i = 0; i < r->size; i++){
				// Calculate index number of the cell
				n_id = 0;
				for(int d = 0; d < D; d++){
					n_id += *rect_it[d]*shifts[d];
				}
				// Move forward the iterators
				for(int d = 0; d < D; d++){
					if(rect_it[d] != (*r->rectangle)[d]->end()-1){
						rect_it[d]++;
						break;
					}
					else {
						rect_it[d] = (*r->rectangle)[d]->begin();
					}
				}
				cells.push_back(n_id);
			}
		}
		else
			cout << "nullptr in R found by constraints_on_partition()" << endl;
	}

	// Now we get all the cells covered by the rectangles in the partition
	sort(cells.begin(), cells.end());

	// Check for duplicated cell (bad Packing)
	// and missing elements (bad Covering)
	correct = 0;
	dup = unique(cells.begin(), cells.end()); 
	if(dup == cells.end())
		cout << "    OK - All elements are unique" << endl;
	else{
		cout << "    BAD - Duplicated elements, first is : " << *dup << endl;
		correct == 1;
	}
	missing_cells.clear();
	set_difference(nnz_cells.begin(), nnz_cells.end(),
				   cells.begin(), cells.end(), back_inserter(missing_cells));
	if(missing_cells.size() == 0)
		cout << "    OK - No missing cells" << endl;
	else{
		cout << "    BAD - Missing Cells : " << endl;
		print(missing_cells);
	}
	if(correct == 0){
		cout << "OK - Partition all fine" << endl;
		return 1;
	}
	else 
		return 0;

}

//Verifies that the registers are sorted
int Checker::order_in_registers(){
	cout << "***Check order in registers"  << endl;
	correct = 0;
	for(int d = 0; d < D; d++){
		for(int n = 0; n < N[d]; n++){
			if(!is_sorted(reg[d][n].begin(), reg[d][n].end())){
				cout << "Register not sorted on dim " << d << " line " << n << endl;
				print(reg[d][n]);
				correct = 1;
			}
		}
	}
	if(correct == 0){
		cout << "OK - Register is sorted" << endl;
		return 1;
	}
	else 
		return 0;
}

//Verifies that the constraints on the rectangles are respected.
int Checker::constraints_on_registers(){
	cout << "***Check constrains on registers"  << endl;
	correct = 0;

	// Set iterators on registers for each dimension
	for(int d = 0; d < D; d++){
			reg_it[d] = reg[d].begin();
	}
	int s, h;
	for(int i = 0; i < n_permutations; i++){
		s = intersect(reg_it);
		if(s > 1){
			cout << "More than 1 Rectangle at intersection of register" << endl;
			correct = 1;
			cout << "Mistakes are in : " << endl;
			for(int d = 0; d < D; d++){
				h = distance(reg[d].begin(), reg_it[d]);
				cout << h << " ";
			}
			cout << endl;
		}
		for(int d = 0; d < D; d++){
			if(reg_it[d] != reg[d].end()-1){
				reg_it[d]++;
			}
			else {
				reg_it[d] = reg[d].begin();
			}
		}
	}
	if(correct == 0){
		cout << "OK - Register structure all fine" << endl;
		return 1;
	}
	else
		return 0;
}

// Verifies that the rectangles are in the good registers
int Checker::rectangles_in_registers(){
	cout << "***Check rectangles in registers"  << endl;
	correct = 0;
	//Make a copy of the register
	reg_copy = reg;
	//Clean the current register
	reg.clear();
	//Remake the  register
	reg_of_R();
	//Compare registers
	for(int d = 0; d < D; d++){
		for(int n = 0; n < N[d]; n++){
			sort(reg[d][n].begin(), reg[d][n].end());
			sort(reg_copy[d][n].begin(), reg_copy[d][n].end());
			if(reg[d][n] != reg_copy[d][n]){
				cout << "Incorrect registers" << endl;
				cout << "	Dimension " << d << ", numero " << n << endl;
				cout << "	Fresh register : " << endl;
				print(reg[d][n]);
				cout << "	Updated register : " << endl;
				print(reg_copy[d][n]);
				correct = 1;
			}
		}
	}
	if(correct == 0){
		cout << "OK - Good rectangles in registers" << endl;
		return 1;
	}
	else
		return 0;
}

//Verify that Rsize is correct
int Checker::calculated_size_of_partition(){
	cout << "***Checking calculated size" << endl;
	//Compare it with Rsize
	if(Rsize == R.size()){
		cout << "OK - Size of R is fine" << endl;
		return 1;
	}
	else{
		cout << " BAD Size of R : " << endl;
		cout << "Rsize : " << Rsize << ", in register : " << R.size() << endl;
		return 0;
	}
}

//Convenient function for calculating intersection of multiples registers 
int Checker::intersect(regit const& vecs){
    vector<Rectangle*> result;
    set_intersection(vecs[0]->begin(), vecs[0]->end(), vecs[1]->begin(), vecs[1]->end(), back_inserter(result));
    if (vecs.size() == 2) 
    	return result.size();

    vector<Rectangle*> buffer;
    for (int i = 2; i < vecs.size(); ++i){
        buffer.clear();
        set_intersection(result.begin(), result.end(), vecs[i]->begin(), vecs[i]->end(), back_inserter(buffer));
        swap(result, buffer);
    }
    return result.size();
}