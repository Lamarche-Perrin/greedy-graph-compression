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
#include <vector>
#include <iterator>
#include <fstream>

#include "Rectangle.h"

typedef std::vector<std::shared_ptr<std::vector<int>>> rect_structure;

class Brick{
	public:
		int dim;
		std::vector<int> nodes;
		
		std::vector<int>::iterator brick_cursor;
		std::vector<int>::iterator rect_cursor;

		static double weight;
		static double info;
		static int non_zero;
		static rect_structure* r;
		static std::vector<std::string> spaces;

	public:
		Brick(int d);
		virtual ~Brick();
		
		//Processing Rectangle
		virtual void get_weight();
		virtual void get_info();
		virtual void get_nnz(std::vector<int>& nnz_cells, std::vector<int>& shifts, int value);
		virtual void get_non_zero(Rectangle* R);
		//Tree construction	
		virtual Brick* insert_Node_Brick(int dim, int node);
		virtual Brick* insert_Data_Brick(int dim, int node);
		virtual void insert_value(int node, double value);
		virtual void normalize(double Eg);

		//print
		virtual void print_bricks();
		virtual void print_data_on_file(std::ofstream& datafile, int N);


		virtual void make_micro(std::vector<Rectangle*>& R, rect_structure* rect);
		//virtual void atomise(Rectangle* ref_rect, std::vector<Rectangle*>& new_Rs);
		virtual void shrink(rect_structure* rect);

};

