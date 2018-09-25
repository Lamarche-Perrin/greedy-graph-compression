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
#include <memory>
#include <vector>
#include <functional>
#include <numeric>
#include <math.h>
#include <iostream>

class Rectangle{
	typedef std::vector<std::shared_ptr<std::vector<int>>> rect_structure;

	friend class Builder;
	friend class Checker;
	friend class Operator;
	friend class Brick;
	public:
		int size;
		std::vector<int> sizes;
		rect_structure* rectangle;

		double weight;
		double info;
		double loss;
		double dkl;

		double weight_check;
		double info_check;
		double loss_check;
		double dkl_check;


	public:
		Rectangle();
		Rectangle(rect_structure* rectangle);
		Rectangle(const Rectangle& r);
		~Rectangle();
				
		void doLoss();
		void doDKL();
		
		void doLossCheck();
		void doDKLCheck();

		Rectangle* deep_copy();

		bool operator==(const Rectangle& r) const;
		std::vector<int> &operator[](int i);
		std::vector<int> &operator[](int i) const;
};
