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


#include "Data_Tree.h"

using namespace std;


Data_Tree::Data_Tree(vector<int> N) : N(N), D(N.size()), E(0), Ew(0){	
}

Data_Tree::Data_Tree() : N(vector<int>(0)), D(0), E(0), Ew(0){
}

Data_Tree::~Data_Tree(){
	for(int i = 0; i < nodes.size(); i++){
		if(nodes[i] != nullptr)
			delete nodes[i];
	}
	nodes.clear();
}

//This function takes a hyperlinks and insert it in the data structure
void Data_Tree::make_bricks(){
	//cout << endl << "Making hyperlinks" << endl;
	
	int n = hyperlinks[0];
	//cout << "n : " << n << endl;
	//If D == 2 the creation of the Data_Brick is done here
	if(D == 2){
		//cout << "2D data struct" << endl;
		if(nodes[n] == nullptr){
			//cout << "Data_Brick created" << endl;
			nodes[n] = new Data_Brick(1);
		}
		nodes[n]->insert_value(hyperlinks[1], value);
	}
	//Else it will be done by the adjacent Node_Brick
	//Here we iterate over the dimensions do build the Node_Bricks
	else{
		//cout << D << "D data struct" << endl;
		//First we insert a Node_Brick in vector<Brick*> nodes if necessary
		if(nodes[n] == nullptr){
			nodes[n] = new Node_Brick(1);
			//cout << "Node_Brick created" << endl;
		}
		working_brick = nodes[n];
		//cout << "working_brick : " << working_brick << endl;
		//Then we iterate over the hyperlinks
		for(int d = 1; d < D; d++){
			n = hyperlinks[d];
			//cout << "Dimension : " << d << " Node : " << n << endl;
			//There is three options
			//Access a Node_Brick, insert a Data_Brick
			if(d == D-2){
				//cout << "Inserting Data Brick on " << working_brick << endl;
				working_brick = working_brick->insert_Data_Brick(d+1, n);
			}
			//Fill a Data_Brick
			else if (d == D-1){
				//cout << "Inserting Value of node " << n << " : " << hyperlinks[D] << endl;
				working_brick->insert_value(n, value);
			}
			//Access a Node_Brick,
			else{
				//cout << "Inserting Node Brick" << endl;
				working_brick = working_brick->insert_Node_Brick(d+1, n);
			}
		}
	}
	//cout << endl << "Making hyperlinks done" << endl;
	//cout << endl;
}

// void Data_Tree::normalize(){
// 	for(auto e : nodes){
// 		if(e != nullptr){
// 			e->normalize(Ew);
// 		}
// 	}
// }

void Data_Tree::get_weight(Rectangle* R){
	Brick::weight = 0;
	Brick::r = R->rectangle;
	
	for(int v : *((*R->rectangle)[0])){
		if(nodes[v]){
			nodes[v]->get_weight();
		}
		//do we have to check for null ptr ?
	}
	//cout << "Rectangle " << R << " weight : " << Brick::weight << endl;
}

void Data_Tree::get_info(Rectangle* R){
	Brick::info = 0;
	Brick::r = R->rectangle;
	
	for(int v : *((*R->rectangle)[0])){
		//cout << "node : " << v << endl;
		if(nodes[v]){
			nodes[v]->get_info();
		}
	}
	//cout << endl << "Rectangle info " << Brick::info << endl;
}

void Data_Tree::get_nnz(std::vector<int>& nnz_cells){
	shifts.resize(D, 1);
	for(int d = 1; d < D; d++)
		shifts[d] =  accumulate(N.begin(), N.begin()+d, 1, multiplies<>());
	//cout << "shifts datatree : " << endl;
	print(shifts);
	for(int i = 0; i < nodes.size(); i++){
		if(nodes[i]){
			nodes[i]->get_nnz(nnz_cells, shifts, i*shifts[0]);
		}
	}
	sort(nnz_cells.begin(), nnz_cells.end());
	//cout << endl << "Rectangle info " << Brick::info << endl;
}

void Data_Tree::get_non_zero(Rectangle* R){
	Brick::non_zero = 0;
	Brick::r = R->rectangle;
	
	for(int v : *((*R->rectangle)[0])){
		//cout << "node : " << v << endl;
		if(nodes[v]){
			nodes[v]->get_non_zero(R);
		}
	}
	//cout << endl << "Rectangle info " << Brick::info << endl;
}

void Data_Tree::make_brick_spaces(){
	vector<string> spaces;
	string space = "";
	for(int d = 0; d < D; d++){
		spaces.push_back(space);
		space = space + "   ";
	}
	Brick::spaces = spaces;
}

void Data_Tree::print_data(){
	cout << "First level Nodes : " << endl;
	for(int i = 0; i < N[0]; i++){
		if(nodes[i])
			cout << i << " : " << nodes[i] << endl;
		else
			cout << i << " : No out neighbours" << endl;
	}
	cout << endl;

	for(int i = 0; i < N[0]; i++){
		if(nodes[i]){
			cout << "Printing node " << i << " neighbours at " << nodes[i] << endl;
			nodes[i]->print_bricks();
		}
	}
	cout << "All data printed" << endl;
}

void Data_Tree::print_metadata(){
	cout << "DataTree Metadata : " << endl;
	cout << "D  :  " << D << endl;
	cout << "E  :  " << E << endl;
	cout << "Ew :  " << Ew << endl;
	cout << "N  :  " << endl;
	print(N);
}

void Data_Tree::print_data_on_file(string filename){
    ofstream datafile;
    datafile.open(filename, ios::out);
    if (datafile.is_open())
    {   
		for(auto e : nodes){
			if(e)
				e->print_data_on_file(datafile, N[1]);
			else{
				for(int i = 0; i < N[1]; i++){
					datafile << 0;
					if(i < N[1]-1)
						datafile << " ";
				}
				datafile << endl;
			}
		}
        datafile.close();
    }
    else
        std::cout << "Error opening file for writing datas" << endl;
}

void Data_Tree::make_micro(std::vector<Rectangle*>& R){
	for(int i = 0; i < N[0]; i++){
		if(nodes[i]){
			rect_structure* rect = new rect_structure();
			rect->push_back(make_shared<vector<int>>());
			(*rect)[0]->push_back(i);
			//cout << "Building rect " << rect << "on dim 0 adding " << i << endl;
			nodes[i]->make_micro(R, rect);
		}
	}
}

/*
void Data_Tree::atomise(Rectangle* ref_rect, std::vector<Rectangle*>& new_Rs){
	Brick::r = ref_rect->rectangle;
	
	for(int n : (*((*(ref_rect->rectangle))[0])) ){
		if(nodes[n]){
			rect_structure* rect = new rect_structure();
			rect->push_back(make_shared<vector<int>>());
			(*rect)[0]->push_back(n);
			//cout << "Building rect " << rect << "on dim 0 adding " << i << endl;
			nodes[n]->atomise(ref_rect, new_Rs, rect);
		}
	}
}
*/

void Data_Tree::shrink(Rectangle* R){
	Brick::r = R->rectangle;

	rect_structure* rect = new rect_structure(D);
	for(int d = 0; d < D; d++)
    	(*rect)[d] = make_shared<vector<int>>();
    for(int e : (*(*(R->rectangle))[0])){
    	if(nodes[e] != nullptr){
    		(*rect)[0]->push_back(e);
    		nodes[e]->shrink(rect);
    	}
	}

	//remove duplicates and sort
	for(int d = 0; d < D; d++){
		sort( (*rect)[d]->begin(), (*rect)[d]->end() );
		(*rect)[d]->erase(unique(
			(*rect)[d]->begin(), (*rect)[d]->end()), (*rect)[d]->end());
	}
	//Switch rectangles (delete old)
	delete R->rectangle;
	R->rectangle = rect;
}

//DEBUG

vector< vector <int>>* Data_Tree::make_macro_rectangle(){
	vector<int> temp;
	vector< vector <int>>* rectangle;
	rectangle = new vector< vector <int>>;

	for(int d = 0; d < D; d++){
		//cout << "size on dimension d : " << N[d] << endl;
		temp.resize(N[d]);
		iota(temp.begin(), temp.end(), 0);
		rectangle->push_back(temp);
	}
	return rectangle;
}

std::vector<std::vector <int>>* Data_Tree::make_random_rectangle(){
	double rdm;
	random_device rd;
	uniform_real_distribution<double> distribution(0.0,1.0);

	vector<int> temp;
	vector< vector <int>>* rectangle;
	rectangle = new vector< vector <int>>;

	for(int d = 0; d < D; d++){
		temp.clear();
		while(temp.size() == 0){
			for(int i = 0; i < N[d]; i++){
				rdm = distribution(rd);
				//cout << "rmd : " << rdm << endl;
				if(rdm < 0.5){
					temp.push_back(i);
				}
			}			
		}
		rectangle->push_back(temp);
	}
	return rectangle;
}


void Data_Tree::printRectangle(const Rectangle& r){
	for(auto const& m : *(r.rectangle)){
		print(*m);
	}
}
