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

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include <sstream>

#include <stdlib.h>

#include "Rectangle.h"
#include "Brick.h"
#include "Node_Brick.h"
#include "Data_Brick.h"


class Data_Tree {
	public:
		typedef std::vector<std::shared_ptr<std::vector<int>>> rect_structure;
		
		//Brick structure
		Brick* working_brick;
		std::vector<Brick*> nodes;

		//metadata and data
		int D, E;
		double value, Ew;
		std::vector<int> N;
		std::vector<int> shifts;
		std::vector<int> hyperlinks;

	public:
		Data_Tree(std::vector<int> N);
		Data_Tree();
		virtual ~Data_Tree();


		//Build data tree
		void make_bricks();
		// void normalize();

		//Use data tree
		void get_weight(Rectangle* R);
		void get_info(Rectangle* R);
		void get_nnz(std::vector<int>& nnz_cells);
		void get_non_zero(Rectangle* R);
		
		//Print data_tree
		void make_brick_spaces();
		void print_data();	
		void print_metadata();
		void print_data_on_file(std::string filename);

		//Build a Partition from the dataset
		void make_micro(std::vector<Rectangle*>& R);
		//void atomise(Rectangle* ref_rect, std::vector<Rectangle*>& new_Rs);
		void shrink(Rectangle* R);

		//debug
		std::vector<std::vector <int>>* make_macro_rectangle();
		std::vector<std::vector <int>>* make_random_rectangle();
		void printRectangle(const Rectangle& r);

		template <typename T> void print(T& s){
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