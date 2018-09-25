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

#include <cmath>
#include <algorithm>
#include <numeric>
#include <iostream>

#include "Generater.h"
#include "Reader.h"

class Builder{
	
	public:
		// rect_structure is the core element of a Rectangle.
		// A two dimensional rectangles contains two vectors of integers designating indexes of nodes
		// The rect_structure is a vector that contains pointers to those two vectors
		typedef std::vector<std::shared_ptr<std::vector<int>>> rect_structure;

		// register is the full data structure of the registers.
		// The three levels correspond to:
		// 1 - A dimension,
		// 2 - A node id
		// 3 - A vector of Rectangles
		// Therefore registre[0][5] is the vector of all the rectangles containing the node 5 on their 0 dimension
		typedef std::vector< std::vector< std::vector<Rectangle*>>> registre;

		//regit is a vector of iterators on the re
		typedef std::vector< std::vector< std::vector<Rectangle*>>::iterator > regit;


		int Rsize; //Size of the partition

		//Partitions
		std::vector<Rectangle*> R, R_check, buffer;

		//Registers
		registre reg;
		
		//Meta data
		int D, E;
		double Ew;
		double log_E;
		std::vector<int> N;
		std::string starting_partition;

		//Data
		Data_Tree* data_tool;
	
		
		Builder(std::vector<int> N, std::vector<double> params);
		Builder(std::string input_filename);
		Builder(std::string type, int n);
		Builder(std::vector<int> N);
		Builder();
		~Builder();
		
		void macro();
		void micro();
		void build_R();
		void build_R(std::vector<Rectangle*>& R);
		void clean_R();
		void clean_R(std::vector<Rectangle*>& R);
		void reg_of_R();
		void clean_registers();

		std::vector<Rectangle*>* copy_partition();
		void copy_partition(std::vector<Rectangle*>* R_copy);

		void print_R();
		void print_R(std::vector<Rectangle*>& R);
		void print_reg();
		void print_reg_size();
		void print_rectangle(const Rectangle* r);
		void print_rectangle_full(const Rectangle* r);

		void print_data();
		void print_metadata();

		template <typename T> void print(T s){
			for (typename T::value_type elem : s)
			{std::cout << elem << ' ';}
    	std::cout << std::endl;
		};

		template <typename T> void print_multi_p(T s){
			for (const typename T::value_type t : s){
				print(*t);
			}
		};

		template <typename T> void print_multi(T s){
			for (const typename T::value_type t : s){
				print(t);
			}
		};

};
