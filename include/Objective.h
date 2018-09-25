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
#include "Operator.h"

class Objective : public Operator{	
	public:
		double lambda;

		double info;
		double loss;
		double dkl;
		double ldkl;
		double penalty;
		double cost;

		double dkl_norm;
		double dkl_factor;
		double penalty_factor;

		// Deltas
		double d_dkl;
		int d_Rsize;
		double d_ldkl;
		double d_penalty;
		double d_cost;

		// Checks
		double weight_check;
		double info_check;
		double loss_check;
		double calculated_loss_check;
		double dkl_check;

		double ldkl_check;
		double penalty_check;
		double cost_check;

		// Utils
		double wR;
		double Wacc;
		std::vector<Rectangle*>::iterator max_it;
		int max_pos;

	public:
	Objective(std::vector<int> N, std::vector<double> params, double lambda);
	Objective(std::string input_filename, double lambda);
	Objective(std::string type, int n, double lambda);
	Objective();
	~Objective();


	void initialize_objective();
	void get_objective(std::vector<Rectangle*>& R_);
	double get_dkl_norm();

	int check_objective();
	
	void calculate_rectangle();
	void calculate_operation();
	void delta_cost();
	void do_operation();
	void cancel_operation();
	
	void print_cost();
	void print_cost_rectangles();
};

static bool Compare(Rectangle* a, Rectangle* b)
{
    return (a->size < b->size);
}
