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


#include "Node_Brick.h"
using namespace std;


Node_Brick::Node_Brick(int d):Brick(d){}

Node_Brick::~Node_Brick(){
	//cout << "Node Brick destructor called" << endl;
	for(int i = 0; i < bricks.size(); i++)
		delete bricks[i];
}

void Node_Brick::get_weight(){

	//cout << spaces[dim] << "get weight in brick : " << this << " belonging to dim " << dim << endl;
	brick_cursor = nodes.begin();
	rect_cursor = (*r)[dim]->begin();

	while(rect_cursor != (*r)[dim]->end() and brick_cursor != nodes.end()){
		if(distance((*r)[dim]->begin(), (*r)[dim]->end()) < distance(nodes.begin(), nodes.end())){
			//cout << spaces[dim] << "searching rect in brick : node " << *rect_cursor << endl;
			brick_cursor = lower_bound(brick_cursor, nodes.end()-1, *rect_cursor);
			if(*brick_cursor == *rect_cursor){
				//cout << spaces[dim] << "node found" << endl;
				bricks[distance(nodes.begin(), brick_cursor)]->get_weight();
			}
			rect_cursor++;
		}
		else{
			//cout << spaces[dim] << "searching brick in rect : node " << *brick_cursor << endl;
			rect_cursor = lower_bound(rect_cursor, (*r)[dim]->end()-1, *brick_cursor);
			if(*brick_cursor == *rect_cursor){
				//cout << spaces[dim] << "node found" << endl;
				bricks[distance(nodes.begin(), brick_cursor)]->get_weight();
			}
			brick_cursor++;
		}
	}
}

void Node_Brick::get_info(){
	brick_cursor = nodes.begin();
	rect_cursor = (*r)[dim]->begin();

	while(rect_cursor != (*r)[dim]->end() and brick_cursor != nodes.end()){
		if(distance((*r)[dim]->begin(), (*r)[dim]->end()) < distance(nodes.begin(), nodes.end())){
			//cout << spaces[dim] << "searching rect in brick : node " << *rect_cursor << endl;
			brick_cursor = lower_bound(brick_cursor, nodes.end()-1, *rect_cursor);
			if(*brick_cursor == *rect_cursor){
				//cout << spaces[dim] << "node found" << endl;
				bricks[distance(nodes.begin(), brick_cursor)]->get_info();
			}
			rect_cursor++;
		}
		else{
			//cout << spaces[dim] << "searching brick in rect : node " << *brick_cursor << endl;
			rect_cursor = lower_bound(rect_cursor, (*r)[dim]->end()-1, *brick_cursor);
			if(*brick_cursor == *rect_cursor){
				//cout << spaces[dim] << "node found" << endl;
				bricks[distance(nodes.begin(), brick_cursor)]->get_info();
			}
			brick_cursor++;
		}
	}
}

void Node_Brick::get_nnz(std::vector<int>& nnz_cells, std::vector<int>& shifts, int value){
	for(int i = 0; i < nodes.size(); i++){
		bricks[i]->get_nnz(nnz_cells, shifts, value+nodes[i]*shifts[dim]);
	}
}

void Node_Brick::get_non_zero(Rectangle* R){
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

Brick* Node_Brick::insert_Node_Brick(int dim, int node){
	//Si le noeud existe
	brick_cursor = lower_bound(nodes.begin(), nodes.end(), node);
	if(brick_cursor == nodes.end()){
		nodes.push_back(node);
		newbrick = new Node_Brick(dim);
		bricks.push_back(newbrick);
		return newbrick;
	}
	else if(*brick_cursor != node){
		nodes.insert(brick_cursor, node);
		newbrick = new Node_Brick(dim);
		bricks.insert(bricks.begin() + distance(nodes.begin(), brick_cursor), newbrick);
		return newbrick;
	}
	else{
		return bricks[distance(nodes.begin(), brick_cursor)];
	}	
}

Brick* Node_Brick::insert_Data_Brick(int dim, int node){
	//cout << "	Node Brick " << this << " inserting databrick" << endl;
	brick_cursor = lower_bound(nodes.begin(), nodes.end(), node);
	if(brick_cursor == nodes.end()){
		//cout << "	appending at end of nodes" << endl;
		nodes.push_back(node);
		newbrick = new Data_Brick(dim);
		bricks.push_back(newbrick);
		return newbrick;
	}
	else if(*brick_cursor != node){
		//cout << "	inserting in nodes" << endl;
		//cout << "	node : " << node << endl;
		//cout << "	*brick cursor : " << *brick_cursor<< endl;
		brick_cursor = nodes.insert(brick_cursor, node);
		newbrick = new Data_Brick(dim);
		bricks.insert(bricks.begin() + distance(nodes.begin(), brick_cursor), newbrick);
		return newbrick;
	}
	else{
		//cout << "	This brick already exist" << endl;
		return bricks[distance(nodes.begin(), brick_cursor)];
	}	
}

void Node_Brick::normalize(double Eg){
	for(auto e : bricks){
		e->normalize(Eg);
	}
}

void Node_Brick::print_bricks(){
	cout << spaces[dim] << "printing brick : " << this << " belonging to dim " << dim << endl;
	cout << spaces[dim];
	for(int i = 0; i < nodes.size(); i++){
		cout << nodes[i] << " ";
	}
	cout << endl;
	cout << spaces[dim];
	for(int i = 0; i < nodes.size(); i++){
		cout << bricks[i] << " ";
	}
	cout << endl;
	for(int i = 0; i < nodes.size(); i++){
		cout << spaces[dim] << "Checking node " << nodes[i] << endl;
		bricks[i]->print_bricks();
	}
}


void Node_Brick::make_micro(std::vector<Rectangle*>& R, rect_structure* rect){	

	if(nodes.size() > 1){
		for(int i = 1; i < nodes.size(); i++){
			rect_structure* rect_copy = new rect_structure(*rect);
			//cout << spaces[dim] << "Making the copy " << rect_copy << " of rect " << rect << " on dim " << dim << " adding " << nodes[i] << dim << endl;
			rect_copy->push_back(make_shared<vector<int>>());
			(*rect_copy)[dim]->push_back(nodes[i]);
			bricks[i]->make_micro(R, rect_copy);
		}
	}

	rect->push_back(make_shared<vector<int>>());
	(*rect)[dim]->push_back(nodes[0]);
	//cout << spaces[dim] << "Building rect " << rect << " on dim " << dim << " adding " << nodes[0] << endl;
	bricks[0]->make_micro(R, rect);
}

//void Node_Brick::atomise(Rectangle* ref_rect, std::vector<Rectangle*>& new_Rs){}
void Node_Brick::shrink(rect_structure* rect){
	brick_cursor = nodes.begin();
	rect_cursor = (*r)[dim]->begin();

	while(rect_cursor != (*r)[dim]->end() and brick_cursor != nodes.end()){
		if(distance((*r)[dim]->begin(), (*r)[dim]->end()) < distance(nodes.begin(), nodes.end())){
			//cout << spaces[dim] << "searching rect in brick : node " << *rect_cursor << endl;
			brick_cursor = lower_bound(brick_cursor, nodes.end()-1, *rect_cursor);
			if(*brick_cursor == *rect_cursor){
				//cout << spaces[dim] << "node found" << endl;
				(*rect)[dim]->push_back(*brick_cursor);
				bricks[distance(nodes.begin(), brick_cursor)]->shrink(rect);
			}
			rect_cursor++;
		}
		else{
			//cout << spaces[dim] << "searching brick in rect : node " << *brick_cursor << endl;
			rect_cursor = lower_bound(rect_cursor, (*r)[dim]->end()-1, *brick_cursor);
			if(*brick_cursor == *rect_cursor){
				//cout << spaces[dim] << "node found" << endl;
				(*rect)[dim]->push_back(*brick_cursor);
				bricks[distance(nodes.begin(), brick_cursor)]->shrink(rect);
			}
			brick_cursor++;
		}
	}

}

