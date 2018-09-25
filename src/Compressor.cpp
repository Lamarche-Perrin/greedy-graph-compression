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


#include "Compressor.h"

using namespace std;
using namespace std::chrono;

// Constructor for Compressor with Data from a Random Matrix Model
Compressor::Compressor(vector<int> N, vector<double> params, double lambda,
                        double gap, int nsearch):
						Optimizer(N, params, lambda),
						gap(gap), nsearch(nsearch){
}

// Constructor for Compressor with Data imported from a file
Compressor::Compressor(string input_filename, double lambda,
						double gap, int nsearch):
						Optimizer(input_filename, lambda),
						gap(gap), nsearch(nsearch){
}

// Constructor for Compressor with Benchmark Data
Compressor::Compressor(string type, int n, double lambda,
						double gap, int nsearch):
						Optimizer(type, n, lambda),
						gap(gap), nsearch(nsearch){
}

Compressor::Compressor(){}

Compressor::~Compressor(){
    for(int i = 0; i < explored_lambda.size(); i++){
    	clean_R(*get<2>(explored_lambda[i]));
    }
}

// A Dichotomic Search With several tests
void Compressor::dichotomic_search(){
    data_tool->print_data_on_file(generated_data_filename);
    t_start = steady_clock::now();

	//MACRO
	clean_optimization_program();
	lambda = 0.0;
	macro();
	reg_of_R();
	initialize_objective();
    explored_lambda.push_back(make_tuple(lambda, cost, copy_partition(), Rsize, info, loss, dkl, ldkl, penalty));
    cout << "macro done" << endl;

	//MICRO
	clean_optimization_program();
	lambda = 100.0;
	micro();
	reg_of_R();
	initialize_objective();
    explored_lambda.push_back(make_tuple(lambda, cost, copy_partition(), Rsize, info, loss, dkl, ldkl, penalty));
    cout << "micro done" << endl;
    
	low_explo = explored_lambda.begin();
    up_explo = explored_lambda.end()-1;

	cout << "micro size : " << get<2>(*low_explo)->size() << " adress : " << get<2>(*low_explo) << endl;
	cout << "macro size : " << get<2>(*up_explo)->size() << " adress : " << get<2>(*up_explo) << endl;

	do{
		d_lambda = get<0>(*up_explo) - get<0>(*low_explo);
		if(get<2>(*low_explo) != get<2>(*up_explo) and d_lambda > gap){
			lambda = get<0>(*low_explo) + d_lambda/2;
			optimization_program();
			full_check_solution_dichotomic_search();
			// print_optim_trials();
			// print_explored_lambda();
			// cout << endl;

		}
		else{
			low_explo++;
			up_explo++;
		}
	}
	while(up_explo != explored_lambda.end());

    make_homogeneous();

    t_stop = steady_clock::now();
    optim_time = duration_cast<duration<double>>(t_stop - t_start);
    write_partitions(homogeneous_solutions);
    free_memory(explored_lambda);
    print_to_print();
}

// A Dichotomic Search Without tests
void Compressor::dichotomic_search_no_check(){
    data_tool->print_data_on_file(generated_data_filename);
    t_start = steady_clock::now();
	
	//MACRO
	clean_optimization_program();
	lambda = 0.0;
	macro();
	reg_of_R();
	initialize_objective();
    explored_lambda.push_back(make_tuple(lambda, cost, copy_partition(), Rsize, info, loss, dkl, ldkl, penalty));
    cout << "macro done" << endl;

	//MICRO
	clean_optimization_program();
	lambda = 100.0;
	micro();
	reg_of_R();
	initialize_objective();
    explored_lambda.push_back(make_tuple(lambda, cost, copy_partition(), Rsize, info, loss, dkl, ldkl, penalty));
    cout << "micro done" << endl;
    
	low_explo = explored_lambda.begin();
    up_explo = explored_lambda.end()-1;

	cout << "micro size : " << get<2>(*low_explo)->size() << " adress : " << get<2>(*low_explo) << endl;
	cout << "macro size : " << get<2>(*up_explo)->size() << " adress : " << get<2>(*up_explo) << endl;

	do{
		d_lambda = get<0>(*up_explo) - get<0>(*low_explo);
		if(get<2>(*low_explo) != get<2>(*up_explo) and d_lambda > gap){
			lambda = get<0>(*low_explo) + d_lambda/2;
			optimization_program();
			// print_optim_trials();
			// print_explored_lambda();
			// cout << endl;

		}
		else{
			low_explo++;
			up_explo++;
		}
	}
	while(up_explo != explored_lambda.end());

    t_stop = steady_clock::now();
    optim_time = duration_cast<duration<double>>(t_stop - t_start);
    write_partitions(homogeneous_solutions);
    free_memory(explored_lambda);
    print_to_print();
}

// The Otpimization program. It runs several mix program and
// keeps the best solution found
void Compressor::optimization_program(){
	clean_optimization_program();
	micro();
	reg_of_R();
	mix_program_succinct();
	optim_trials = best_solution;

	for(int s = 1; s < nsearch; s++){
		clean_optimization_program();
		micro();
		reg_of_R();
		mix_program_succinct();

		if(cost < get<1>(optim_trials)){
			clean_R(*get<2>(optim_trials));
			optim_trials = best_solution;
		}
		else{
			clean_R();
		}
	}
}

// Cleans the object. It allows to rebuild entirely a new partition
// and its registers
void Compressor::clean_optimization_program(){
	clean_registers();
	clean_temp_registers();
	clean_R();
}

// To update a storage structure with the current state of the object.
void Compressor::update_storage(storage& S){
	get<1>(S) = cost;
	get<3>(S) = Rsize;
	get<4>(S) = info;
	get<5>(S) = loss;
	get<6>(S) = dkl;
	get<7>(S) = ldkl;
	get<8>(S) = penalty;
}


// Check the actuel partition against the partition with the closuer upper
// lambda and againt the partition with the closest lower lambda.
void Compressor::full_check_solution_dichotomic_search(){
	//Check Copy
	cout << "Checking solution" << boolalpha << endl;
	cout << "Testing clow on " << get<2>(*low_explo) << " and " << get<2>(optim_trials) << endl;
	clow = compare_partitions(get<2>(*low_explo), get<2>(optim_trials));
	cout << "clow : " << clow << endl;
	if(clow){
		test_weak_up();
		cout << "    Testing wup" << endl;
		clean_R(*get<2>(optim_trials));
		cout << "    wup : " << wup << endl;
		if(wup){
			update_storage(optim_trials);
			get<2>(optim_trials) = get<2>(*up_explo);
			up_explo = explored_lambda.insert(up_explo, optim_trials);
			low_explo = up_explo-1;
		}
		else{
			get<2>(optim_trials) = get<2>(*low_explo);
			low_explo = explored_lambda.insert(up_explo, optim_trials);
			up_explo = low_explo+1;
		}
		return;
	}

	cout << "Testing cup on " << get<2>(*up_explo) << " and " << get<2>(optim_trials) << endl;
	cup = compare_partitions(get<2>(*up_explo), get<2>(optim_trials));
	cout << "cup : " << cup << endl;
	if(cup){
		test_weak_low();
		cout << "    Testing wlow" << endl;
		clean_R(*get<2>(optim_trials));
		cout << "    wlow : " << wlow << endl;
		if(wlow){
			update_storage(optim_trials);
			get<2>(optim_trials) = get<2>(*low_explo);
			low_explo = explored_lambda.insert(up_explo, optim_trials);
			up_explo = low_explo+1;
		}
		else{
			get<2>(optim_trials) = get<2>(*up_explo);
			up_explo = explored_lambda.insert(up_explo, optim_trials);
			low_explo = up_explo-1;
		}
		return;
	}

	
	//Test strength
	cout << "Testing slow" << endl;
	test_strength_low();
	cout << "slow : " << slow << endl;
	if(slow){
		if(low_explo == explored_lambda.begin() or get<2>(*prev(low_explo)) != get<2>(*low_explo))
			clean_R(*get<2>(*low_explo));
		update_storage(*low_explo);
		get<2>(*low_explo) = get<2>(optim_trials);
		cout << "    Testing wup" << endl;
		test_weak_up();
		cout << "    wup : " << wup << endl;
		if(wup){
			update_storage(optim_trials);
			get<2>(optim_trials) = get<2>(*up_explo);
			up_explo = explored_lambda.insert(up_explo, optim_trials);
			low_explo = up_explo-1;
			if(low_explo != explored_lambda.begin()){
				low_explo--;
				up_explo--;
			}
			return;
		}
	}

	cout << "Testing sup" << endl;
	test_strength_up();
	cout << "sup : " << sup << endl;
	if(sup){
		if(next(up_explo) == explored_lambda.end() or get<2>(*next(up_explo)) != get<2>(*up_explo))
			clean_R(*get<2>(*up_explo));
		update_storage(*up_explo);
		get<2>(*up_explo) = get<2>(optim_trials);
		cout << "    Testing wlow" << endl;
		test_weak_low();
		cout << "    wlow : " << wlow << endl;
		if(wlow){
			update_storage(optim_trials);
			get<2>(optim_trials) = get<2>(*low_explo);
			low_explo = explored_lambda.insert(up_explo, optim_trials);
			up_explo = low_explo+1;
			return;
		}
	}
	
	if(slow and sup){
		low_explo = explored_lambda.insert(up_explo, optim_trials);
		low_explo++;
		up_explo = low_explo+1;
		return;
	}
	if(slow){
		low_explo = explored_lambda.insert(up_explo, optim_trials);
		up_explo = low_explo+1;
		return;
	}
	if(sup){
		up_explo = explored_lambda.insert(up_explo, optim_trials);
		low_explo = up_explo-1;
		return;
	}

	//Test weakness
	cout << "Testing wlow" << endl;
	test_weak_low();
	cout << "wlow : " << wlow << endl;
	if(wlow){
		clean_R(*get<2>(optim_trials));
		update_storage(optim_trials);
		get<2>(optim_trials) = get<2>(*low_explo);
		cout << "    Testing wup" << endl;
		test_weak_up();
		cout << "    wup : " << wup << endl;
		if(wup){
			update_storage(optim_trials);
			get<2>(optim_trials) = get<2>(*up_explo);
			up_explo = explored_lambda.insert(up_explo, optim_trials);
			low_explo = up_explo-1;
			return;
		}
		low_explo = explored_lambda.insert(up_explo, optim_trials);
		up_explo = low_explo+1;
		return;
	}
	

	cout << "Testing wup" << endl;
	test_weak_up();
	cout << "wup : " << wup << endl;
	if(wup){
		clean_R(*get<2>(optim_trials));
		update_storage(optim_trials);
		get<2>(optim_trials) = get<2>(*up_explo);
		up_explo = explored_lambda.insert(up_explo, optim_trials);
		low_explo = up_explo-1;
		return;
	}

	//Treat as unique
	up_explo = explored_lambda.insert(up_explo, optim_trials);
	low_explo = up_explo-1;
}


// Remove duplicate partitions from explored_lambda
void Compressor::make_homogeneous(){
	homogeneous_solutions = explored_lambda;
	c_explo = explored_lambda.begin();
	h_explo = homogeneous_solutions.begin();

    while(c_explo != explored_lambda.end()){
    	//gather all candidate partitions
    	partitions.clear();
    	if(c_explo != explored_lambda.begin()){
    		partitions.push_back(get<2>(*prev(c_explo)));
    		partitions.push_back(get<2>(*prev(h_explo)));
    	}
    	if(c_explo != explored_lambda.end()-1){
    		partitions.push_back(get<2>(*next(c_explo)));
    		partitions.push_back(get<2>(*next(h_explo)));
    	}

    	//Sort and remove duplicate
    	vector<vector<Rectangle*>*>::iterator it = remove(partitions.begin(), partitions.end(), get<2>(*h_explo));
    	partitions.resize(distance(partitions.begin(), it));

    	sort(partitions.begin(), partitions.end());
    	partitions.erase(unique(partitions.begin(), partitions.end()), partitions.end());

    	//Test all on h_explo lambda's
    	lambda = get<0>(*h_explo);
    	for(auto p : partitions){
    		get_objective(*p);
			if(cost < get<1>(*h_explo)){
				update_storage(*h_explo);
				get<2>(*h_explo) = p;
			}
		}
		c_explo++;
		h_explo++;
    }
}

// Test if the low_explo partition gives a better cost for the actual lambda
// than the actual partition
void Compressor::test_weak_low(){
	wlow = false;
	lambda = get<0>(optim_trials);
	get_objective(*get<2>(*low_explo));
	if(cost < get<1>(optim_trials))
		wlow = true;
}

// Test if the up_explo partition gives a better cost for the actual lambda
// than the actual partition
void Compressor::test_weak_up(){
	wup = false;
	lambda = get<0>(optim_trials);
	get_objective(*get<2>(*up_explo));
	if(cost < get<1>(optim_trials))
		wup = true;
}

// Test if the actual partition gives a better cost for the low_explo lambda
// than the low_explo partition
void Compressor::test_strength_low(){
	slow = false;
	lambda = get<0>(*low_explo);
	get_objective(*get<2>(optim_trials));
	if(cost <= get<1>(*low_explo))
		slow = true;
}

// Test if the actual partition gives a better cost for the up_explo lambda
// than the up_explo partition
void Compressor::test_strength_up(){
	sup = false;
	lambda = get<0>(*up_explo);
	get_objective(*get<2>(optim_trials));
	if(cost <= get<1>(*up_explo))
		sup = true;
}

// Compare the content of two partitions
bool Compressor::compare_partitions(vector<Rectangle*>* R1, vector<Rectangle*>* R2){
	if(R1->size() != R2->size()){
		return false;
	}
	for(auto r1 : *R1){
		shared = 0;
		for(auto r2 : *R2){
			if(*r1 == *r2){
				shared = 1;
				break;
			}
		}
		if(shared == 0){
			return false;
		}
	}
	return true;
}

// Free all the partitions contained in a vector of storage
void Compressor::free_memory(vector<storage>& S){
    for(auto p : S){
    	clean_R(*get<2>(p));
    }
}

// Print the explored_lambda vector
void Compressor::print_explored_lambda(){
	cout << "Explored lambda : " << endl;
	for(auto tpl : explored_lambda){
		cout << "lambda : " << get<0>(tpl)
			 << "  cost : " << get<1>(tpl)
			 << "  partition : " << get<2>(tpl) << endl;
	}
}

// Print to_print :s
void Compressor::print_to_print(){
	cout << "To print lambda : " << endl;
	for(auto tp : to_print){
		cout << "lambda : " << get<0>(tp)
			 << "  cost : " << get<1>(tp)
			 << "  partition : " << get<2>(tp) << endl;
	}
}

// Print optim_trials
void Compressor::print_optim_trials(){
	cout << "optim trials : " << endl;
	cout << "lambda : " << get<0>(optim_trials)
		 << " cost : " << get<1>(optim_trials)
		 << " partition : " << get<2>(optim_trials) << endl;
}

// Compare two pointers
bool compare_memory(const storage& lhs, const storage& rhs) {
  return get<2>(lhs) == get<2>(rhs);
}

// Write a partition a in file
void Compressor::write_partitions(vector<storage>& S){
	make_filenames("");
    write_header();
	
	to_print = S;
	vector<storage>::iterator it = unique(to_print.begin(), to_print.end(), compare_memory);
	to_print.resize(distance(to_print.begin(), it));

    for(auto p : to_print){
    	make_filenames("_lambda=" + to_string(get<0>(p)) + "_");
   		write_partition(p);
    }
}