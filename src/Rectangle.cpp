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


#include "Rectangle.h"

using namespace std;

Rectangle::Rectangle(){}

// Constructor
Rectangle::Rectangle(rect_structure* rectangle) : rectangle{rectangle}{
	for(auto const& m  : *rectangle)
		sizes.push_back(m->size());
	size = accumulate(sizes.begin(), sizes.end(), 1, multiplies<>());
	//cout << "Creating rectangle " << this << " containing " << rectangle << endl;
}

// Copy constructor
Rectangle::Rectangle(const Rectangle& r):
	size(r.size),
	sizes(r.sizes),
	rectangle(new rect_structure(*(r.rectangle))),
	weight(r.weight),
	info(r.info),
	loss(r.loss),
	dkl(r.dkl),
	weight_check(0),
	info_check(0),
	loss_check(0),
	dkl_check(0)
{}

Rectangle::~Rectangle(){
	//cout << "Deleting rectangle " << this << " containing " << rectangle << endl;
	delete rectangle;
	//cout << "Rectangle delete" << endl;
}

// Calculate Loss
void Rectangle::doLoss(){
	if(weight > 0){
		loss = -weight*log2(weight/size);
	}
	else
		loss = 0;
}

// Calculate DKL
void Rectangle::doDKL(){
	dkl = info + loss;
}

// Check Loss
void Rectangle::doLossCheck(){
	if(weight_check > 0)
		loss_check = -weight_check*log2(weight_check/size);
	else loss_check = 0;
}

//Check DKL
void Rectangle::doDKLCheck(){
	dkl_check = info_check + loss_check;
}

// To copy a rectangle
Rectangle* Rectangle::deep_copy(){
	Rectangle* R = new Rectangle();
	R->size = this->size;
	R->sizes = this->sizes;
	R->weight = this->weight;
	R->info = this->info;
	R->loss = this->loss;
	R->dkl = this->dkl;
	R->weight_check = 0;
	R->info_check = 0;
	R->loss_check = 0;
	R->dkl_check = 0;
	int D = this->sizes.size();
	R->rectangle = new rect_structure(D);
	for(int d = 0; d < D; d++){
    	(*R->rectangle)[d] = make_shared<vector<int>>(); //intersection
		*((*R->rectangle)[d]) = *((*this->rectangle)[d]);
	}
	return R;
}

// Comparison operator between two rectangles
bool Rectangle::operator==(const Rectangle& r) const
{
	for(int i = 0; i < rectangle->size(); i++){
		if(*((*rectangle)[i]) != *((*r.rectangle)[i]))
			return false;
	}
	return true;
}

// Custom indexing
vector<int> &Rectangle::operator[](int i){
	return *((*rectangle)[i]);
}


vector<int> &Rectangle::operator[](int i) const
{
	return *((*rectangle)[i]);
}