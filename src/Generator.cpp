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


#include "Generater.h"

using namespace std;

Generater::Generater(vector<int> N, vector<double> params): Data_Tree(N), params(params)
{
	//cout << "Generater constructor" << endl;
	setup_constants();
	build_social_model();
	make_brick_spaces();
}

Generater::Generater(string type, int n){
	if(type == "sparse")
		build_multiscale_sparse_matrix(n);
	else if(type == "full")
		build_multiscale_complete_matrix(n);
	else if (type == "small")
		build_small_tester();
	else
		cout << "Unknown matrix type" << endl;
	make_brick_spaces();
}

Generater::Generater(){
	//cout << "Generater default constructor" << endl;
}

Generater::~Generater(){	
}

void Generater::setup_constants(){
	w		= params[0];
	alpha1 	= params[1];
	beta1 	= params[2];
	alpha2 	= params[3];
	beta2 	= params[4];
	shape 	= params[5];
	scale 	= params[6];
	rate 	= w*(alpha1+beta1)*(alpha2+beta2)/(shape*scale*alpha1*alpha2);
}

void Generater::build_social_model(){
	nodes.resize(N[0], nullptr);

	//Random Uniform generator
	random_device rd_dev;
	default_random_engine generator(rd_dev());
	uniform_real_distribution<double> unif_dist(0.0,1.0); 

	//mt Generater
	boost::mt19937 gen(rd_dev());

	//Random Poisson generator :
	boost::poisson_distribution<int> dist_poisson(rate);
    boost::variate_generator<boost::mt19937&, boost::poisson_distribution<int>> poisson_gen(gen, dist_poisson);

 	

    //Random Beta generator
	boost::math::beta_distribution<> dist_beta1(alpha1, beta1);
	boost::math::beta_distribution<> dist_beta2(alpha2, beta2);

    //Random Gamma generator
	boost::math::gamma_distribution<> dist_gamma(shape, scale);

	//Poisson Result
	int number_of_rectangles = poisson_gen(); //(n)
	if(number_of_rectangles == 0)
		number_of_rectangles = 1;
	//Loop on rectangles
	double p1, p2, lambda, unif_number, value;
	vector<int> I, J;
	for(int n = 0; n < number_of_rectangles; n++){
		//cout << "rectangle " << n << endl;
		I.clear();
		J.clear();
		
		p1 = quantile(dist_beta1, unif_dist(generator));
		p2 = quantile(dist_beta2, unif_dist(generator));
		
		lambda = quantile(dist_gamma, unif_dist(generator));
		
		boost::poisson_distribution<int> dist_poisson_inRect(lambda);
	    boost::variate_generator<boost::mt19937&, boost::poisson_distribution<int>> poisson_gen_inRect(gen, dist_poisson_inRect);
	    
		for(int i = 0; i < N[0]; i++){
			unif_number = unif_dist(generator);
			if(unif_number < p1){
				I.push_back(i);
			}
		}
		for(int j = 0; j < N[1]; j++){
			unif_number = unif_dist(generator);
			if(unif_number < p2){
				J.push_back(j);
			}
		}
		for(int i : I){
			if(nodes[i] == nullptr){
				//cout << "Creating new Data Brick " << i << endl;
				nodes[i] = new Data_Brick(1);
			}
			for(int j : J){
				//cout << "Working on existing Data Brick " << i << " with neightbour " << j << endl;
				value = poisson_gen_inRect();
				E++;
				Ew += value;
				nodes[i]->insert_value(j, value);
			}
		}
	}
	//cout << "nodes size  " << nodes.size() << endl;
	// if(Ew != 1.0)
   	//  	normalize();
}

void Generater::build_multiscale_complete_matrix(int n){
	D = 2;
	N.push_back(8*n);
	N.push_back(8*n);
	nodes.resize(N[0], nullptr);

	vector<double> increment = {0, 0.1, 0.3, 0.6};
	//Building rectangle 1
	vector<int> shift_xaxis1 = {1, 1, 2, 2};
	for(int i = 0; i < 4*n; i++){
		int x = 0;
		int k;
		if(nodes[i] == nullptr)
			nodes[i] = new Data_Brick(1);
		for(int j = 0; j < 4; j++){
			for(k = x; k < x+shift_xaxis1[j]*n; ++k){
				value = 1+increment[j];
				E++;
				Ew += value;
				nodes[i]->insert_value(k, value);
			}
			x += shift_xaxis1[j]*n;
		}
	}
	//Building rectangle 2
	vector<int> shift_yaxis2 = {2, 2, 1, 1};
	int y = 0;
	int j;
	for(int i = 0; i < 4; i++){
		for(j = y; j < y+shift_yaxis2[i]*n; ++j){
			if(nodes[j] == nullptr)
				nodes[j] = new Data_Brick(1);
			for(int k = 6*n; k < 8*n; k++){
				value = 2+increment[i];
				E++;
				Ew += value;
				nodes[j]->insert_value(k, value);
			}
		}
		y += shift_yaxis2[i]*n;
	}


	//Building rectangle 3
	vector<int> shift_xaxis3 = {1,1,1,1};
	for(int i = 6*n; i < 8*n; i++){
		int x = 4*n;
		int k;
		if(nodes[i] == nullptr)
			nodes[i] = new Data_Brick(1);
		for(int j = 0; j < 4; j++){
			for(k = x; k < x+shift_xaxis3[j]*n; ++k){
				value = 3+increment[j];
				E++;
				Ew += value;
				nodes[i]->insert_value(k, value);
			}
			x += shift_xaxis3[j]*n;
		}
	}


	//Building rectangle 0.1
	vector<int> shift_xaxis01 = {1,1,2,2};
	for(int i = 4*n; i < 6*n; i++){
		int x = 0;
		int k;
		if(nodes[i] == nullptr)
			nodes[i] = new Data_Brick(1);
		for(int j = 0; j < 4; j++){
			for(k = x; k < x+shift_xaxis01[j]*n; ++k){
				value = 0.01+increment[j];
				E++;
				Ew += value;
				nodes[i]->insert_value(k, value);
			}
			x += shift_xaxis01[j]*n;
		}
	}

	//Building rectangle 0.2
	vector<int> shift_xaxis02 = {1,1,2};
	for(int i = 6*n; i < 8*n; i++){
		int x = 0;
		int k;
		if(nodes[i] == nullptr)
			nodes[i] = new Data_Brick(1);
		for(int j = 0; j < 3; j++){
			for(k = x; k < x+shift_xaxis02[j]*n; ++k){
				value = 0.01+increment[j];
				E++;
				Ew += value;
				nodes[i]->insert_value(k, value);
			}
			x += shift_xaxis02[j]*n;
		}
	}
	// if(Ew != 1.0)
   	//  	normalize();
}

void Generater::build_multiscale_sparse_matrix(int n){
	D = 2;
	N.push_back(8*n);
	N.push_back(8*n);
	nodes.resize(N[0], nullptr);

	vector<double> increment = {0, 0.1, 0.3, 0.6};
	//Building rectangle 1
	vector<int> shift_xaxis1 = {1, 1, 1, 1};
	for(int i = 0; i < 2*n; i++){
		int x = 0;
		int k;
		if(nodes[i] == nullptr)
			nodes[i] = new Data_Brick(1);
		for(int j = 0; j < 4; j++){
			for(k = x; k < x+shift_xaxis1[j]*n; ++k){
				value = 1+increment[j];
				E++;
				Ew += value;
				nodes[i]->insert_value(k, value);
			}
			x += shift_xaxis1[j]*n;
		}
	}
	//Building rectangle 2
	vector<int> shift_yaxis2 = {1, 1, 1, 1};
	int y = 0;
	int j;
	for(int i = 0; i < 4; i++){
		for(j = y; j < y+shift_yaxis2[i]*n; ++j){
			if(nodes[j] == nullptr)
				nodes[j] = new Data_Brick(1);
			for(int k = 6*n; k < 8*n; k++){
				value = 2+increment[i];
				E++;
				Ew += value;
				nodes[j]->insert_value(k, value);
			}
		}
		y += shift_yaxis2[i]*n;
	}

	//Building rectangle 3
	for(int i = 4*n; i < 5*n; i++){
		if(nodes[i] == nullptr)
			nodes[i] = new Data_Brick(1);
		for(int j = 0; j < 3*n; j++){
			value = 3;
			E++;
			Ew += value;
			nodes[i]->insert_value(j, value);
		}
	}

	//Building rectangle 4
	for(int i = 6*n; i < 6*n+n; i++){
		if(nodes[i] == nullptr)
			nodes[i] = new Data_Brick(1);
		for(int j = 2*n; j < 5*n; j++){
			value = 4;
			E++;
			Ew += value;
			nodes[i]->insert_value(j, value);
		}
	}
	
	// if(Ew != 1.0)
   	//  	normalize();
	
}



void Generater::build_small_tester(){
	D = 2;
	N.push_back(4);
	N.push_back(4);
	nodes.resize(N[0], nullptr);

	vector<double> increment = {0, 0.1, 0.3, 0.6};
	for(int i = 0; i < N[0]; i++){
		if(nodes[i] == nullptr)
			nodes[i] = new Data_Brick(1);
		value = 1+increment[i];
		for(int j = 0; j < N[1]; j++){
			E++;
			Ew += value;
			nodes[i]->insert_value(j, value);
		}
	}
	// if(Ew != 1.0)
   	//  	normalize();
}