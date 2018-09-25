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

#include <vector>
#include "Data_Brick.h"


class Node_Brick: public Brick{
	protected:
		Brick* newbrick;
		std::vector<Brick*> bricks;

	public:
		Node_Brick(int d);
		~Node_Brick();

		void get_weight();
		void get_info();
		void get_nnz(std::vector<int>& nnz_cells, std::vector<int>& shifts, int value);
		void get_non_zero(Rectangle* R);

		Brick* insert_Node_Brick(int dim, int node);
		Brick* insert_Data_Brick(int dim, int node);
		void normalize(double Eg);
		void print_bricks();

		void make_micro(std::vector<Rectangle*>& R, rect_structure* rect);
		//void atomise(Rectangle* ref_rect, std::vector<Rectangle*>& new_Rs);
		void shrink(rect_structure* rect);

};