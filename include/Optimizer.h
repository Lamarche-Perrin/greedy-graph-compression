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
#include <iomanip>
#include <ctime>
#include <sstream>
#include <chrono>
#include <set>
#include "Objective.h"

//					0		1		2							3	4		5		6		7		8		
//				   lambda  cost    partition    	         Rsize info   loss	   dkl	   ldkl    penalty        	
typedef std::tuple<double, double, std::vector<Rectangle*>*, int, double, double, double, double, double> storage;


class Optimizer : public Objective{	
	public:
		//Operations flags
		int found, accepted;

		//parameter for mix program
		int nfail; int nfail_limit;
		int ndiv_start; int ndiv_increm; int ndiv_limit;
		int ncycle;

  		//Storage for bes solution found
		storage best_solution;

		//Timers
	    std::chrono::steady_clock::time_point tbegin, tend, tfind, tdelta, tdone, t0;
		std::chrono::duration<double> optim_time, ope_time;

		//Parameter for file output
		int data_size;
		std::vector<double> data;
		std::string path_to_output_data;
		std::string filename;
		std::string meta_filename;
		std::string data_filename;
		std::string partition_filename;
		std::string generated_data_filename;
		std::string experiment;
		std::string algorithm;

	public:
	Optimizer(std::vector<int> N, std::vector<double> params, double lambda);
	Optimizer(std::string input_filename, double lambda);
	Optimizer(std::string type, int n, double lambda);
	Optimizer();
	~Optimizer();
	
	void init();
	void make_filenames(std::string prefix = "");
	void init_data();
	void fill_data();
	void set_parameters(int nfail_limit, int ndiv_start, int ndiv_increm, int ncycle);

	void gradient_descente_while_cluster(int nfail_limit);
	void gradient_descente_while_cluster_succinct(int nfail_limit);

	void mix_program();
	void mix_program_succinct();

	void gradient_descente_while_union(std::vector<double>& data, std::ofstream& datafile);
	void gradient_descente_while_union_succinct();

	void divide_random(std::vector<double>& data, std::ofstream& datafile);
	void divide_random_succinct();

	void restart_from_best();

	void write_column_names(std::ofstream& file);
	void write_annexes();
	void write_header();
	void write_partition();
	void write_partition(std::vector<Rectangle*>& R_);
	void write_partition(storage& S);

	
	template <typename T>
	void write_vector(T& s, std::ofstream& file){
		for (typename T::value_type elem : s)
			file << elem << " ";
    	file << std::endl;
		};
};