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


#include "Data_Brick.h"
using namespace std;

Data_Brick::Data_Brick(int d):Brick(d){}

Data_Brick::~Data_Brick(){}

void Data_Brick::get_weight(){

	//cout << spaces[dim] << "get weight in brick : " << this << " belonging to dim " << dim << endl;
	brick_cursor = nodes.begin();
	rect_cursor = (*r)[dim]->begin();

	while(rect_cursor != (*r)[dim]->end() and brick_cursor != nodes.end()){
		if(distance((*r)[dim]->begin(), (*r)[dim]->end()) < distance(nodes.begin(), nodes.end())){
			//cout << spaces[dim] << "searching rect in brick : node " << *rect_cursor << endl;
			brick_cursor = lower_bound(brick_cursor, nodes.end()-1, *rect_cursor);
			//cout << "Search result in brick_cursor on : " << *rect_cursor << endl;
			if(*brick_cursor == *rect_cursor){
				//cout << spaces[dim] << "node found,";
				weight += datas[distance(nodes.begin(), brick_cursor)];
				//cout << " increasing weight by " << datas[distance(nodes.begin(), brick_cursor)] << endl; 
			}
			rect_cursor++;
		}
		else{
			//cout << spaces[dim] << "searching brick in rect : node " << *brick_cursor << endl;
			rect_cursor = lower_bound(rect_cursor, (*r)[dim]->end()-1, *brick_cursor);
			//cout << "Search result in rect_cursor on : " << *rect_cursor << endl;
			if(*brick_cursor == *rect_cursor){
				//cout << spaces[dim] << "node found";
				weight += datas[distance(nodes.begin(), brick_cursor)];
				//cout << " increasing weight by " << datas[distance(nodes.begin(), brick_cursor)] << endl; 
			}
			brick_cursor++;
		}
	}
}

void Data_Brick::get_info(){
	brick_cursor = nodes.begin();
	rect_cursor = (*r)[dim]->begin();

	while(rect_cursor != (*r)[dim]->end() and brick_cursor != nodes.end()){
		if(distance((*r)[dim]->begin(), (*r)[dim]->end()) < distance(nodes.begin(), nodes.end())){
			//cout << spaces[dim] << "searching rect in brick : node " << *rect_cursor << endl;
			brick_cursor = lower_bound(brick_cursor, nodes.end()-1, *rect_cursor);
			if(*brick_cursor == *rect_cursor){
				//cout << spaces[dim] << "node found,";
				info += datas[distance(nodes.begin(), brick_cursor)]*log2(datas[distance(nodes.begin(), brick_cursor)]);
				//cout << " increasing weight by " << datas[distance(nodes.begin(), brick_cursor)] << endl; 
			}
			rect_cursor++;
		}
		else{
			//cout << spaces[dim] << "searching brick in rect : node " << *brick_cursor << endl;
			rect_cursor = lower_bound(rect_cursor, (*r)[dim]->end()-1, *brick_cursor);
			if(*brick_cursor == *rect_cursor){
				//cout << spaces[dim] << "node found";
				info += datas[distance(nodes.begin(), brick_cursor)]*log2(datas[distance(nodes.begin(), brick_cursor)]);
				//cout << " increasing weight by " << datas[distance(nodes.begin(), brick_cursor)] << endl; 
			}
			brick_cursor++;
		}
	}
}

void Data_Brick::get_nnz(std::vector<int>& nnz_cells, std::vector<int>& shifts, int value){
	for(int i = 0; i < nodes.size(); i++){
		nnz_cells.push_back(value+nodes[i]*shifts[dim]);
	}
}

void Data_Brick::get_non_zero(Rectangle* R){
	//cout << spaces[dim] << "get weight in brick : " << this << " belonging to dim " << dim << endl;
	brick_cursor = nodes.begin();
	rect_cursor = (*r)[dim]->begin();

	while(rect_cursor != (*r)[dim]->end() and brick_cursor != nodes.end()){
		if(distance((*r)[dim]->begin(), (*r)[dim]->end()) < distance(nodes.begin(), nodes.end())){
			//cout << spaces[dim] << "searching rect in brick : node " << *rect_cursor << endl;
			brick_cursor = lower_bound(brick_cursor, nodes.end()-1, *rect_cursor);
			//cout << "Search result in brick_cursor on : " << *rect_cursor << endl;
			if(*brick_cursor == *rect_cursor){
				//cout << spaces[dim] << "node found,";
				non_zero++;
				//cout << " increasing weight by " << datas[distance(nodes.begin(), brick_cursor)] << endl; 
			}
			rect_cursor++;
		}
		else{
			//cout << spaces[dim] << "searching brick in rect : node " << *brick_cursor << endl;
			rect_cursor = lower_bound(rect_cursor, (*r)[dim]->end()-1, *brick_cursor);
			//cout << "Search result in rect_cursor on : " << *rect_cursor << endl;
			if(*brick_cursor == *rect_cursor){
				//cout << spaces[dim] << "node found";
				non_zero++;
				//cout << " increasing weight by " << datas[distance(nodes.begin(), brick_cursor)] << endl; 
			}
			brick_cursor++;
		}
	}
}

void Data_Brick::insert_value(int node, double value){
	//	cout << "inserting value : " << value << endl;
	brick_cursor = lower_bound(nodes.begin(), nodes.end(), node);
	//If the node doesn't exist : insert it
	if(brick_cursor == nodes.end()){
		//cout << "Inserting new value at end of datas" << endl;
		nodes.push_back(node);
		datas.push_back(value);
		//brick_cursor = nodes.insert(brick_cursor, node);
		//datas.insert(datas.begin() + distance(nodes.begin(), brick_cursor), value);
	}
	else if(*brick_cursor != value){
		//cout << "Inserting new value somewhere in data" << endl;
		brick_cursor = nodes.insert(brick_cursor, node);
		datas.insert(datas.begin() + distance(nodes.begin(), brick_cursor), value);
	}
	//If the node exist, we are gong to add the value to the already existing one
	else{
		//cout << "Adding to existing value" << endl;
		datas[distance(nodes.begin(), brick_cursor)] += value;
	}	

}

void Data_Brick::normalize(double Eg){
	for(int k = 0; k < datas.size(); k++){
		datas[k] /= Eg; 
	}
}



void Data_Brick::print_bricks(){

	cout << "dim : " << dim << endl;
	cout << spaces[dim] << "printing brick : " << this << " belonging to dim " << dim << endl;
	cout << spaces[dim];
	for(int i = 0; i < nodes.size(); i++){
		cout << nodes[i] << " ";
	}
	cout << endl;
	cout << spaces[dim];
	for(int i = 0; i < nodes.size(); i++){
		cout << setprecision(10) << datas[i] << " ";
	}
	cout << endl;
}

void Data_Brick::print_data_on_file(std::ofstream& datafile, int N){
	for(int i = 0; i < N; i++){
		brick_cursor = lower_bound(nodes.begin(), nodes.end(), i);
		if(brick_cursor != nodes.end()){
			if(*brick_cursor == i){
				datafile << datas[distance(nodes.begin(), brick_cursor)];
			}
			else{
				datafile << 0;

			}
			if(i < N-1)
				datafile << " ";
		}
		else{
			datafile << 0;
			if(i < N-1)
				datafile << " ";
		}
	}
	datafile << endl;
}


void Data_Brick::make_micro(std::vector<Rectangle*>& R, rect_structure* rect){
	Rectangle* r;

	if(nodes.size() > 1){
		for(int i = 1; i < nodes.size(); i++){
			rect_structure* rect_copy = new rect_structure(*rect);
			//cout << spaces[dim] << "Making the copy " << rect_copy << " of rect " << rect << " on dim " << dim << " adding " << nodes[i] <<endl;
			rect_copy->push_back(make_shared<vector<int>>());
			(*rect_copy)[dim]->push_back(nodes[i]);
			r = new Rectangle(rect_copy);
			R.push_back(r);
		}
	}

	rect->push_back(make_shared<vector<int>>());
	(*rect)[dim]->push_back(nodes[0]);
	//cout << spaces[dim] << "Making the rect " << rect << " on dim " << dim << " adding " << nodes[0] << endl;
	r = new Rectangle(rect);
	R.push_back(r);
}


//void Data_Brick::atomise(Rectangle* ref_rect, std::vector<Rectangle*>& new_Rs){}

void Data_Brick::shrink(rect_structure* rect){
	brick_cursor = nodes.begin();
	rect_cursor = (*r)[dim]->begin();

	while(rect_cursor != (*r)[dim]->end() and brick_cursor != nodes.end()){
		if(distance((*r)[dim]->begin(), (*r)[dim]->end()) < distance(nodes.begin(), nodes.end())){
			//cout << spaces[dim] << "searching rect in brick : node " << *rect_cursor << endl;
			brick_cursor = lower_bound(brick_cursor, nodes.end()-1, *rect_cursor);
			if(*brick_cursor == *rect_cursor){
				//cout << spaces[dim] << "node found" << endl;
				(*rect)[dim]->push_back(*brick_cursor);
			}
			rect_cursor++;
		}
		else{
			//cout << spaces[dim] << "searching brick in rect : node " << *brick_cursor << endl;
			rect_cursor = lower_bound(rect_cursor, (*r)[dim]->end()-1, *brick_cursor);
			if(*brick_cursor == *rect_cursor){
				//cout << spaces[dim] << "node found" << endl;
				(*rect)[dim]->push_back(*brick_cursor);
			}
			brick_cursor++;
		}
	}
}




