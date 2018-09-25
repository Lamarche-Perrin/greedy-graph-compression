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


#include "Builder.h"
using namespace std;


//Create a Builder Object and  build a Random Matrix Model
Builder::Builder(vector<int> N, vector<double> params) : N(N), D(N.size()){
	data_tool = new Generater(N, params);
	E = data_tool->E;
	Ew = data_tool->Ew;
	log_E = log2((double)E);
}

//Create a Builder Object and load a Graph from a csv or a json file
Builder::Builder(string input_filename){
	cout << "Builder Reader constructor" << endl;
	data_tool = new Reader(input_filename);
	cout << "reader done" << endl;
	N = data_tool->N;
	D = N.size();
	E = data_tool->E;
	Ew = data_tool->Ew;
	log_E = log2((double)E);
	//cout << "Builder constructor done, DT kind" << endl;
}

//Create a Builder Object with a Benchmark Graph of scale n
Builder::Builder(string type, int n){
	//cout << "Builder Generater constructor" << endl;
	data_tool = new Generater(type, n);
	N = data_tool->N;
	D = data_tool->D;
	E = data_tool->E;
	Ew = data_tool->Ew;
	log_E = log2((double)E);
}

//Create a Builder Object in which a partition can be built
Builder::Builder(vector<int> N): N(N), D(N.size()), E(0), Ew(0), data_tool(nullptr){
}

//Create a Builder Object
Builder::Builder(): D(0), N(vector<int>(0)), E(0), Ew(0), data_tool(nullptr){
}

//Destructor cleans up the partition and the data
Builder::~Builder(){
	//Clean Partition
	//R.clear();
	//if(reg.size() > 0)
	//	build_R(R);
	if(R.size() > 0)
		for (int i = 0; i < R.size(); i++)
		    delete R[i];
	R.clear();

	//Clean data
	if(data_tool != nullptr)
		delete data_tool;
}

//Build a macro partition
void Builder::macro(){
	rect_structure* rect = new rect_structure(D);
	for(int d = 0; d < D; d++)
    	(*rect)[d] = make_shared<vector<int>>(); //intersection

	for(int d = 0; d < D; d++){
		(*rect)[d]->resize(N[d]);
		iota((*rect)[d]->begin(), (*rect)[d]->end(), 0);
	}
	Rectangle* r = new Rectangle(rect);
	R.push_back(r);
	Rsize = 1;
	starting_partition = "macro ";
}

//Build a micro partition by using the data
void Builder::micro(){
	data_tool->make_micro(R);
	Rsize = R.size();
	sort(R.begin(), R.end());
	starting_partition = "micro ";
}

//Rebuild the partition using the registers
void Builder::build_R(){
	R.clear();
	for (auto it = reg[0].begin(); it != reg[0].end(); ++it){
        buffer.clear();
        set_union(R.begin(), R.end(), it->begin(), it->end(),
        back_inserter(buffer));

        swap(R, buffer);
	}
	sort(R.begin(), R.end());
	R.erase(unique(R.begin(), R.end()), R.end());
}

//Rebuild the partition using the register and put it in a vector passed by reference
void Builder::build_R(vector<Rectangle*>& R){
	R.clear();

	for (auto it = reg[0].begin(); it != reg[0].end(); ++it){
        buffer.clear();
        set_union(R.begin(), R.end(), it->begin(), it->end(),
        back_inserter(buffer));

        swap(R, buffer);
	}
	sort(R.begin(), R.end());
	R.erase(unique(R.begin(), R.end()), R.end());
}

//Clean up the partition
void Builder::clean_R(){
	if(R.size() > 0)
		for (int i = 0; i < R.size(); ++i)
		    delete R[i];
	R.clear();
}

//Clean up a partition given by reference
void Builder::clean_R(vector<Rectangle*>& R){
	if(R.size() > 0){
		//cout << "Cleaning partition of size " << R.size() << endl;
		for (int i = 0; i < R.size(); ++i){
			//cout << "Cleaning number " << i << endl;
		    //cout << "Adress " << R[i] << endl;
		    delete R[i];
		}
	}
	R.clear();
}

//Build the registers
void Builder::reg_of_R(){
	//Resize registers :
	reg.resize(D);
	//cout << "reg.size() " << reg.size() << endl;
	for(int d = 0; d < D; d++)
		reg[d].resize(N[d]);

	//iterate over rectangles : index over R
	for(Rectangle* r : R){
		if(r != nullptr){
			//iterate over dimensions of the rectangle
			for(int d = 0; d < D; d++){
				//iterate over module of current dimension
				for(int n : (*r)[d]){
					//Append to register of node N in dimension d the index of the rectangle
					reg[d][n].push_back(r);
				}
			}
		}
	}
}

//Clean the registers
void Builder::clean_registers(){
	if(reg.size() == D)
	    for(int d = 0; d < D; d++)
    	    for(int i = 0; i < N[d]; i++)
        	    reg[d][i].clear();
}

//Returns a copy of the partition
vector<Rectangle*>* Builder::copy_partition(){
	std::vector<Rectangle*>* R_copy = new std::vector<Rectangle*>;
	for(auto r : R){
		//rect_structure* rect = new rect_structure(r->rectangle);
		Rectangle* r_copy = new Rectangle(*r);
		R_copy->push_back(r_copy);
	}
	sort(R_copy->begin(), R_copy->end());
	return R_copy;
}

//Copy a partition in the object's dedicated memory for the partition
//Cleaning the object's dedicated memory for the partition is not managed here.
void Builder::copy_partition(vector<Rectangle*>* R_copy){
	for(auto r_copy : *R_copy){
		Rectangle* r = new Rectangle(*r_copy);
		R.push_back(r);
	}
	sort(R.begin(), R.end());
}

//Print all the rectangles of the partition
void Builder::print_R(){
	cout << "***Printing partition : " << endl;
	for (const Rectangle* r : R){
		if(r != nullptr){
			cout << "Rectangle : " << r << " components : " << r->rectangle << "  with " << r->rectangle->size() << " dimensions" << endl;
	    	cout << "Sizes are : ";
			for(int d = 0; d < D; d++){
	    		cout << (*r)[d].size() << "   ";
	    	}
	    	cout << endl;
	    	print_rectangle(r);
    	}
    cout << endl;
    }
}

//Print all the rectangles of a suplied partition
void Builder::print_R(vector<Rectangle*>& R){
	cout << "***Printing supplied partition : " << endl;
	for (const Rectangle* r : R){
		if(r != nullptr){
			cout << "Rectangle : " << r << " components : " << r->rectangle << "  with " << r->rectangle->size() << " dimensions" << endl;
	    	cout << "Sizes are : ";
			for(int d = 0; d < D; d++){
	    		cout << (*r)[d].size() << "   ";
	    	}
	    	cout << endl;
	    	print_rectangle(r);
    	}
    cout << endl;
    }
}

//Print the size of the registers
void Builder::print_reg_size(){
	cout << endl << "***Registers size : " << endl;
	for(int d = 0; d < D; d++){
		for(int i = 0; i < reg[d].size(); i++){
			cout << "Register dimension " << d << " line : " << i << " size : " << reg[d][i].size() << endl;
		}
	}
}

//Print the registers
void Builder::print_reg(){
	cout << endl << "***Printing registers" << endl;
	for(int d = 0; d < D; d++){
		cout << "Register dimension " << d << " size : " << reg[d].size() << endl;
		for(int i = 0; i < reg[d].size(); i++){
			cout << "line " << i << "   ";
			for(auto const &e : reg[d][i]){
				cout << e << " ";
			}
			cout << endl;
		}
		cout << endl;
	}
}

//Print the rectangle in a Rectangle Object
void Builder::print_rectangle(const Rectangle* r){
	for(auto const& m : *(r->rectangle)){
		print(*m);
	}
}

//Print a Rectangle Object with details
void Builder::print_rectangle_full(const Rectangle* r){
	cout << "Printing : " << endl;
	cout << "Adress : " << r << endl;
	cout << "rect adress : " << r->rectangle << endl;
	cout << "Modules adresses : " << endl;
	for(int d = 0; d < D; d++){
		cout << "	module " << d << " : " << (*(r->rectangle))[d] << endl;
	}
	for(auto const& m : *(r->rectangle)){
		print(*m);
	}
}

//Print data
void Builder::print_data(){
	if(data_tool != nullptr)
		data_tool->print_data();
	else
		cout << "No data to print" << endl << endl;
}

//Print metadata contained in Builder Object
//and Data_Tree Object if it exists
void Builder::print_metadata(){
	cout << "Metadata stored in Builder Object : " << endl;
	cout << "D  :  " << D << endl;
	cout << "E  :  " << E << endl;
	cout << "Ew :  " << Ew << endl;
	cout << "N  :  " << endl;
	print(N);

	if(data_tool != nullptr)
		data_tool->print_metadata();
	else
		cout << "No Data_Tree Object" << endl;
}