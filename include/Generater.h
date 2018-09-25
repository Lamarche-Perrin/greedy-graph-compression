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
#include <boost/math/distributions/beta.hpp> 
#include <boost/math/distributions/gamma.hpp>
#include <boost/math/distributions/poisson.hpp>

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/poisson_distribution.hpp>
#include <boost/random/variate_generator.hpp>



#include "Data_Tree.h"


class Generater: public Data_Tree{

	public:
		//Choose :
		std::vector<double> params;
		double w;

		//Betas law
		double alpha1, beta1;
		double alpha2, beta2;

		//Gamma law
		double shape, scale;

		//Poisson law
		double rate;

	public:
		Generater(std::vector<int> N, std::vector<double> params);
		Generater(std::string type, int n);
		Generater();
		~Generater();
		
		void setup_constants();
		void build_social_model();
		void build_multiscale_complete_matrix(int n);
		void build_multiscale_sparse_matrix(int n);
		void build_small_tester();
};


