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


#include "Operator.h"

using namespace std;


// Constructor for Operator with Data from a Random Matrix Model
Operator::Operator(vector<int> N, vector<double> params) : Checker(N, params){
    init();
}

// Constructor for Operator with Data imported from a file
Operator::Operator(string input_filename) : Checker(input_filename){
    init();
}

// Constructor for Operator with Benchmark Data
Operator::Operator(string type, int n): Checker(type, n){
    init();
}

// Constructor for Operator without Data
Operator::Operator(std::vector<int> N): Checker(N){
    init();
}

//Default Constructor
Operator::Operator(){
}

Operator::~Operator(){
}

// Initialization function of the class attributes
void Operator::init(){
    //Random number generator
    random_device rd_dev;
    generator = default_random_engine(rd_dev());
    uni_r = uniform_real_distribution<double>(0.0, 1.0);
    uni_D = uniform_int_distribution<int>(0, D-1);
    eng = mt19937(rd_dev());
    for(int d = 0; d < D; d++){
        uni_reg.push_back(uniform_int_distribution<int>(0, N[d]-1));
    }

    //This is supposed to be given by user or chosen by the program
    binomial_p = {0.5, 0.5, 0.5, 0.5, 0.5};

    //Resizing class containers to dimension D
    setup_registers_and_storage();
    //Initializing a very useful number
    n_gen = pow(2, D);
    sizeCheck.resize(n_gen);
}

//Initializing the class containers
void Operator::setup_registers_and_storage(){
    del_reg.resize(D);
    add_reg.resize(D);
    regUnionResult.resize(D);

    for(int d = 0; d < D; d++){
        del_reg[d].resize(N[d]);
        add_reg[d].resize(N[d]);        
    }

    
    n_move.resize(2);
    n_move[0].resize(D);
    n_move[1].resize(D);

    inter_diffe.resize(2);
    for(int i = 0; i < D; i++){
        inter_diffe[0].push_back(make_shared<vector<int>>()); //intersection
        inter_diffe[1].push_back(make_shared<vector<int>>()); //difference
    }
}

//To test the random number generator
void Operator::testRandomNumberGenerator(){
	double u;
	for(int i = 0; i < 5; i++){
    	u = uni_r(generator);
    	cout << u << endl;		
	}
}

//Builds randomly a new rectangle
int Operator::find_cluster(){
    rect = new rect_structure(D);
    for(int d = 0; d < D; d++){
        (*rect)[d] = make_shared<vector<int>>();
        for(int i = 0; i < N[d]; i++){
            if(uni_r(generator) < binomial_p[d]){
                (*rect)[d]->push_back(i);
            }
        }
        if((*rect)[d]->size() == 0)
            return 0;
    }
    to_del.clear();
    regUnion2();       //Union of register on all dimensions
    regIntersection(); //Intersect the unions
    return 1;
}

//Builds a rectangle by sampling two rectangles from a randomly
//selected register and calculating the union of them.
int Operator::find_union_constrained(){
    rect = new rect_structure(D);
    if(R.size() < 2)
        return 0;
    int d = uni_D(generator);
    int n = uni_reg[d](generator);

    if(reg[d][n].size() <= 1)
        return 0;

    sampled_rects.clear();
    uni_i = uniform_int_distribution<int>(0, reg[d][n].size()-1);

    sampled_rects.push_back(reg[d][n][uni_i(generator)]);
    sampled_rects.push_back(reg[d][n][uni_i(generator)]);
	
    if(sampled_rects[0] == sampled_rects[1])
        return 0;

    for(d = 0; d < D; d++){
        (*rect)[d] = make_shared<vector<int>>();
        set_union((*((sampled_rects[0])->rectangle))[d]->begin(), (*((sampled_rects[0])->rectangle))[d]->end(),
                    (*((sampled_rects[1])->rectangle))[d]->begin(), (*((sampled_rects[1])->rectangle))[d]->end(),
                    back_inserter(*((*rect)[d])));
    }

    to_del.clear();
    regUnion2();        //Union of register on all dimensions
    regIntersection(); //Intersect the unions
    return 1;
}

//Build a rectangle within a randomly selected rectangle
int Operator::find_division(){
    rect = new rect_structure(D);

    //Pick a rectangle
    uni_i = uniform_int_distribution<int>(0, Rsize-1);
    ref_rect = R[uni_i(generator)];

    if(ref_rect->size == 1)
        return 0;
    //Select a subset on each dimension
    check = 0;
    for(int d = 0; d < D; d++){
        (*rect)[d] = make_shared<vector<int>>();
        for(auto e : *((*(ref_rect->rectangle))[d])){
            if(uni_r(generator) < 0.5){
                (*rect)[d]->push_back(e);
            }
        }
        if((*rect)[d]->size() ==  0)
            return 0;
        if((*rect)[d]->size() !=  (*(ref_rect->rectangle))[d]->size())
            check = 1;

    }
    if(check == 1){
        to_del.clear();
        to_del.push_back(ref_rect);
        return 1;
    }
    else
        return 0;
}



//Extract the components of a Rectangle to prepare an operation
//Allows to use a rectangle calculated elsewhere in the program as an operating rectangle
int Operator::applyCluster(Rectangle* clust){
    rect = new rect_structure(*(clust->rectangle));
    to_del.clear();
    regUnion();        //Union of register on all dimensions
    regIntersection(); //Intersect the unions
    return 1;
}

//To shuffle the partition if needed
void Operator::shuffle_partition(int n_ope){
    for(int i = 0; i < n_ope; i++)
        if(find_cluster())
            doCluster();
        else
            clean_operator();
    cout << "Size of R after shuffle : " <<  Rsize << endl;
}

// Build a rectangle with the calculated intersection and difference
void Operator::calculate_rectangle(){
    new_r = new rect_structure(D);
    for(dim = 0; dim < D; dim++){
        i_d_selector = (int)(n/pow(2, dim))%2;
        // Check if the pointer should be copied or move
        if(n != n_move[i_d_selector][dim]){
            (*new_r)[dim] = inter_diffe[i_d_selector][dim];
        }
        else{
            (*new_r)[dim] = move(inter_diffe[i_d_selector][dim]);
            inter_diffe[i_d_selector][dim] = make_shared<vector<int>>();
            n_move[i_d_selector][dim] = -1;
        }
    }                
    if(n != 0){
        new_R = new Rectangle(new_r);
        new_Rs.push_back(new_R);
    }
    else
        delete new_r;
}

// Calculate intersection and difference of modules, the size of the resulting rectangles,
// and when to move the pointer to the calculated components
void Operator::calculate_operation(){
    // Calculate components and sizes of resulting vectors
    fill(sizeCheck.begin(), sizeCheck.end(), 1);
    for(dim = 0; dim < D; dim++){
        intersection();
        difference();
        pow2d = pow(2, dim);

        for(n = 0; n < n_gen; n++){
            //Remove this if, store (int)(n/pow2d)%2 in a var and use it on inter_diffe
            i_d_selector = (int)(n/pow2d)%2;
            if(inter_diffe[i_d_selector][dim]->size() > 0){       //Check if the module is empty
                sizeCheck[n] *= inter_diffe[i_d_selector][dim]->size();
            }
            else{
                sizeCheck[n] = 0;
                inter_diffe[i_d_selector][dim]->clear();
            }   
        }
    }
    //Calculate when to move the shared ptr
    for(n = 0; n < n_gen; n++){
        if(sizeCheck[n] > 0){
            for(dim = 0; dim < D; dim++){   
                i_d_selector = (int)(n/pow(2, dim))%2;        
                n_move[i_d_selector][dim] = n;
            }
        }
    }
    //Build the non-empty rectangles, remember to replace the moved shared ptr
    for(n = 0; n < n_gen; n++){
        if(sizeCheck[n] > 0){
            calculate_rectangle();
        }
        else{
            for(dim = 0; dim < D; dim++){
                i_d_selector = (int)(n/pow(2, dim))%2;
                if(inter_diffe[i_d_selector][dim]->size() == 0)
                    n_move[i_d_selector][dim] = -1;
            }
        }
    }
}

// Apply a calculated operation
void Operator::doCluster(){
    new_Rs.clear();

    for(k = to_del.begin(); k != to_del.end(); k++){
        calculate_operation();
    }

    new_R = new Rectangle(rect);
    new_Rs.push_back(new_R);

    clean_temp_registers();

    sort(new_Rs.begin(), new_Rs.end());
    
    buffer.clear();
    set_difference(R.begin(), R.end(), to_del.begin(), to_del.end(), back_inserter(buffer));
    buffer.swap(R);

    buffer.clear();
    set_union(R.begin(), R.end(), new_Rs.begin(), new_Rs.end(), back_inserter(buffer));
    buffer.swap(R);

    for(Rectangle* e : to_del){
        make_del_reg(e);
        delete e;
    }

    for(Rectangle* e : new_Rs){
        make_add_reg(e);
    }

    //*** Setting up registres
    update_registers();

    //*** Update Rsize
    Rsize += new_Rs.size() - to_del.size();
}

// Cancel an operation
void Operator::clean_operator(){
    delete rect;
}

// Calculated the intersection on each dimension between the modules of the operating
// rectangle and the modules of the rectangle being affected
void Operator::intersection(){
    set_intersection((*((*k)->rectangle))[dim]->begin(), (*((*k)->rectangle))[dim]->end(),
        (*rect)[dim]->begin(), (*rect)[dim]->end(), back_inserter(*inter_diffe[0][dim]));
}

// Calculated the difference on each dimension between the modules of the operating
// rectangle and the modules of the rectangle being affected
void Operator::difference(){
    set_difference((*((*k)->rectangle))[dim]->begin(), (*((*k)->rectangle))[dim]->end(),
        (*rect)[dim]->begin(), (*rect)[dim]->end(), back_inserter(*inter_diffe[1][dim]));
}

//Calculate the union of registers
void Operator::regUnion(){
    for(int d = 0; d < D; d++){
        regUnionResult[d].clear();
        if((*rect)[d]->size() == 1){
            regUnionResult[d] = reg[d][(*(*rect)[d])[0]];
        }
        else{
            set_union(reg[d][(*((*rect)[d]))[0]].begin(), reg[d][(*((*rect)[d]))[0]].end(),
                    reg[d][(*((*rect)[d]))[1]].begin(), reg[d][(*((*rect)[d]))[1]].end(),
                    back_inserter(regUnionResult[d]));
            if((*rect)[d]->size() > 2){
                for (size_t i = 2; i < (*rect)[d]->size(); ++i){
                    buffer.clear();

                    set_union(regUnionResult[d].begin(), regUnionResult[d].end(),
                    reg[d][(*((*rect)[d]))[i]].begin(), reg[d][(*((*rect)[d]))[i]].end(),
                    back_inserter(buffer));

                    swap(regUnionResult[d], buffer);
                }
            }
        }
    }
}

// An alternative to regUnion
void Operator::regUnion2(){
    for(int d = 0; d < D; d++){
        regUnionResult[d].clear();
        if((*rect)[d]->size() == 1){
            regUnionResult[d] = reg[d][(*(*rect)[d])[0]];
        }
        else{
            set_union(reg[d][(*((*rect)[d]))[0]].begin(), reg[d][(*((*rect)[d]))[0]].end(),
                    reg[d][(*((*rect)[d]))[1]].begin(), reg[d][(*((*rect)[d]))[1]].end(),
                    back_inserter(regUnionResult[d]));
            if((*rect)[d]->size() > 2){
                pq = priority_queue<vector<Rectangle*>*, vector<vector<Rectangle*>*>, Compare_reg_size>();
                for(int e : (*(*rect)[d]))
                    pq.push(&reg[d][e]);
                regUnionResult[d] = *(pq.top());
                pq.pop();
                while(!pq.empty()){
                    buffer.clear();
                    set_union(regUnionResult[d].begin(), regUnionResult[d].end(),
                        pq.top()->begin(), pq.top()->end(),
                        back_inserter(buffer));
                        swap(regUnionResult[d], buffer);
                        pq.pop();
                }
            }
        }
    }
}

// Calculate the intersection of the result of regUnion
void Operator::regIntersection(){
    set_intersection(regUnionResult[0].begin(), regUnionResult[0].end(),
        regUnionResult[1].begin(), regUnionResult[1].end(), back_inserter(to_del));
    if (D == 2) return;
    for (size_t i = 2; i < D; ++i) {
        buffer.clear();
        set_intersection(to_del.begin(), to_del.end(),
            regUnionResult[i].begin(), regUnionResult[i].end(), back_inserter(buffer));
        swap(to_del, buffer);
    }
}


// Make the register of the deleted rectangles
void Operator::make_del_reg(Rectangle* n){
    for(int d = 0; d < D; d++){
        for(auto j : *((*n->rectangle)[d])){
            del_reg[d][j].push_back(n);
        }
    }
}

// Make the register of the added rectangles
void Operator::make_add_reg(Rectangle* n){
    for(int d = 0; d < D; d++){
        for(auto j : *((*n->rectangle)[d])){
            add_reg[d][j].push_back(n);
        }
    }
}

// Clean del_reg and add_reg
void Operator::clean_temp_registers(){
    for(int d = 0; d < D; d++)
        for(int n = 0; n < N[d]; n++){
            del_reg[d][n].clear();
            add_reg[d][n].clear();
        }
}

// Update reg with del_reg and add_reg
void Operator::update_registers(){
    //remove to be deleted to reg
    for(int d = 0; d < D; d++){
        for(int n = 0; n < N[d]; n++){
            buffer.clear();
            set_difference(reg[d][n].begin(), reg[d][n].end(),
        del_reg[d][n].begin(), del_reg[d][n].end(), back_inserter(buffer));
            reg[d][n].swap(buffer);
        }
    }
    //add to be added to reg
    for(int d = 0; d < D; d++){
        for(int n = 0; n < N[d]; n++){
            buffer.clear();
            set_union(reg[d][n].begin(), reg[d][n].end(),
        add_reg[d][n].begin(), add_reg[d][n].end(), back_inserter(buffer));
            reg[d][n].swap(buffer);
        }
    }
}


// Debugs functions
void Operator::print_add_reg(){
    cout << "***print add reg" << endl;
    for(int d = 0; d < D; d++){
        cout << "Register dimension " << d << endl;
        for(int i = 0; i < add_reg[d].size(); i++){
            cout << "line " << i << "   ";
            for(auto const &e : add_reg[d][i]){
                cout << e << " ";
            }
            if(!is_sorted(add_reg[d][i].begin(), add_reg[d][i].end()))
                cout << "line not sorted" << endl;
            cout << endl;
        }
        cout << endl;
    }
}

void Operator::print_del_reg(){
    cout << "***print dell reg" << endl;
    for(int d = 0; d < D; d++){
        cout << "Register dimension " << d << endl;
        for(int i = 0; i < del_reg[d].size(); i++){
            cout << "line " << i << "   ";
            for(auto const &e : del_reg[d][i]){
                cout << e << " ";
            }
            if(!is_sorted(del_reg[d][i].begin(), del_reg[d][i].end()))
                cout << "line not sorted" << endl;
            cout << endl;
        }
        cout << endl;
    }
}

void Operator::print_reg_union(){
    for(int d = 0; d < D; d++){
        print(regUnionResult[d]);
    }
}

void Operator::print_component_storage(component_storage& inter_diffe){
    cout << "Component strorage inter_diffe" << endl;
    for(int i = 0; i < 2; i++){
        for(int j = 0; j < inter_diffe[0].size(); j++){
            if(inter_diffe[i][j] == nullptr)
                cout << "(" << i << "," << j << ")  ptr is null" << endl;
            else
                cout << "(" << i << "," << j << ")  " << inter_diffe[i][j]->size() << endl;
        }
    }
}
