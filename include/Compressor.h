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
#include <tuple> 
#include <utility>

#include "Optimizer.h"

//					0		1		2							3	4		5		6		7		8		
//				   lambda  cost    partition    	         Rsize info   loss	   dkl	   ldkl    penalty        	
//typedef std::tuple<double, double, std::vector<Rectangle*>*, int, double, double, double, double, double> storage;

class Compressor : public Optimizer{
	public:
		double d_lambda;
		double gap;
		int nsearch;

		std::pair<double, double> current_interval;
		
		std::vector<std::vector<Rectangle*>*> partitions;
		
		std::vector<storage> explored_lambda;
		std::vector<storage> homogeneous_solutions;
		std::vector<storage> to_print;
		std::vector<storage>::iterator low_explo, up_explo, c_explo, h_explo;
		storage optim_trials;

		// Chronos
	    std::chrono::steady_clock::time_point t_start, t_stop;
		std::chrono::duration<double> total_time;

		//Partitions Comparison
		int shared;
		bool clow, cup, slow, sup, wlow, wup;

	public:
		Compressor(std::vector<int> N, std::vector<double> params, double lambda,
			double gap, int nsearch);
		Compressor(std::string input_filename, double lambda,
			double gap, int nsearch);
		Compressor(std::string type, int n, double lambda,
			double gap, int nsearch);
		Compressor();
		~Compressor();

		void init();
		
		void dichotomic_search();
		void dichotomic_search_no_check();

		void optimization_program();
		void clean_optimization_program();
		void update_storage(storage& S);

		void full_check_solution_dichotomic_search();
		void make_homogeneous();

		void test_weak_low();
		void test_weak_up();
		void test_strength_low();
		void test_strength_up();

		bool compare_partitions(std::vector<Rectangle*>* R1, std::vector<Rectangle*>* R2);

		void free_memory(std::vector<storage>& S);

		void print_explored_lambda();
		void print_to_print();
		void print_optim_trials();

		void write_partitions(std::vector<storage>& S);
		
};

//bool compare_memory(const storage& lhs, const storage& rhs);
bool compare_memory(const storage& lhs, const storage& rhs);