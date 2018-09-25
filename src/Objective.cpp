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


#include "Objective.h"

using namespace std;

// Constructor for Objective with Data from a Random Matrix Model
Objective::Objective(vector<int> N, vector<double> params, double lambda) : Operator(N, params), lambda(lambda){}

// Constructor for Objective with Data imported from a file
Objective::Objective(string input_filename, double lambda) : Operator(input_filename), lambda(lambda){}

// Constructor for Objective with Benchmark Data
Objective::Objective(string type, int n, double lambda) : Operator(type, n), lambda(lambda){}

// Default Constructor
Objective::Objective(){}

Objective::~Objective(){}

// To initialize the objective function
void Objective::initialize_objective(){
	info = 0;
    loss = 0;
	dkl = 0;

	for(Rectangle* r : R){
		data_tool->get_weight(r);
		data_tool->get_info(r);
		r->weight = Brick::weight;
		r->info = Brick::info;
		r->doLoss();
		r->doDKL();
		info += r->info;
		loss += r->loss;
		dkl += r->dkl;
	}
    // dkl_norm = get_norm();
    dkl_factor = lambda/Ew;
    penalty_factor = 1.0;

	ldkl = dkl_factor*dkl;
	penalty = penalty_factor*Rsize;
	cost = ldkl + penalty;
}

// Calculate the objective for a given partition
void Objective::get_objective(vector<Rectangle*>& R_){
    info = 0;
    loss = 0;
    dkl = 0;
    Rsize = R_.size();
    for(Rectangle* r : R_){
        data_tool->get_info(r);
        r->info = Brick::info;
        r->doLoss();
        r->doDKL();
        info += r->info;
        loss += r->loss;
        dkl += r->dkl;
    }
    dkl_factor = lambda/Ew;
    penalty_factor = 1.0;

	ldkl = dkl_factor*dkl;
	penalty = penalty_factor*Rsize;
	cost = ldkl + penalty;
}

// Calculate a norm
double Objective::get_dkl_norm(){
    //Make a macro rectangle
    rect_structure* rect = new rect_structure(D);
    for(int d = 0; d < D; d++)
        (*rect)[d] = make_shared<vector<int>>(); //intersection

    for(int d = 0; d < D; d++){
        (*rect)[d]->resize(N[d]);
        iota((*rect)[d]->begin(), (*rect)[d]->end(), 0);
    }
    Rectangle* r = new Rectangle(rect);
    
    //Get it's weight and info
    data_tool->get_weight(r);
    data_tool->get_info(r);
    r->weight = Brick::weight;
    r->info = Brick::info;
    r->doLoss();
    r->doDKL();
    double retvalue = r->dkl;
    delete r;
    return retvalue;
}

// Check Objective by recalculating everything
int Objective::check_objective(){
	cout << "***check_objective" << endl;
	cout.precision(17);

	int correct = 0;

	weight_check = 0;
	info_check = 0;
	loss_check = 0;
    calculated_loss_check = 0;
	dkl_check = 0;

	for(Rectangle* r : R){
		data_tool->get_weight(r);
		data_tool->get_info(r);
		r->weight_check = Brick::weight;
		r->info_check = Brick::info;
		r->doLossCheck();
		r->doDKLCheck();
		weight_check +=  Brick::weight;
		info_check += Brick::info;
		loss_check += r->loss_check;
        calculated_loss_check += r->loss;
		dkl_check += r->dkl_check;
		if(abs(r->weight_check - r->weight) > 10e-10){
			cout << "weight wrong for rectangle " << r << endl;
			cout << "calculated : " << r->weight << ", check : " << r->weight_check << endl;
			correct = 1;
		}
		if(abs(r->loss_check - r->loss) > 10e-10){
			cout << "loss wrong for rectangle " << r << endl;
			correct = 1;
		}
        if(r->weight < 10e-10){
            cout << "weight null in rectangle " << r << "  value : " << r->weight << endl;
            correct = 1;
        }
    }
	if(abs(weight_check - Ew) > 10e-10){
		cout << "total check weight wrong, it sums at " << weight_check << endl;
		correct = 1;
	}
	if(abs(info_check - info) > 10e-10){
		cout << "info wrong, calculated :  " << info << ", check : " << info_check << endl;
		correct = 1;
	}
	if(abs(loss_check - loss) > 10e-10){
		cout << "loss wrong, calculated :  " << loss << ", check : " << loss_check << endl;
		correct = 1;
	}
    if(abs(calculated_loss_check - loss) > 10e-10){
        cout << "calculated loss wrong, calculated :  " << loss << ", sum of rects : " << calculated_loss_check << endl;
        correct = 1;
    }
	if(abs(dkl_check - dkl) > 10e-10){
		cout << "dkl wrong, calculated :  " << dkl << ", check : " << dkl_check << endl;
		correct = 1;
	}

	ldkl_check = dkl_factor*dkl_check;
	penalty_check = penalty_factor*Rsize;
	cost_check = ldkl_check + penalty_check;

	if(abs(ldkl_check - ldkl) > 10e-10){
		cout << "ldkl wrong, calculated :  " << ldkl << ", check : " << ldkl_check << endl;
		correct = 1;
	}
	if(abs(penalty_check - penalty) > 10e-10){
		cout << "penalty wrong, calculated :  " << penalty << ", check : " << penalty_check << endl;
		correct = 1;
	}
	if(abs(cost_check - cost) > 10e-10){
		cout << "cost wrong, calculated :  " << cost << ", check : " << cost_check << endl;
		correct = 1;
	}
	if(correct == 0){
		cout << "OK - Objectif all fine" << endl;
        return 1;
    }
    else
        return 0;
}

// Build a rectangle with the calculated intersection and difference
void Objective::calculate_rectangle(){
    new_r = new rect_structure(D);
    for(dim = 0; dim < D; dim++){
        i_d_selector = (int)(n/pow(2, dim))%2;
        //Check if the pointer should be copied or move
        if(n != n_move[i_d_selector][dim]){
            (*new_r)[dim] = inter_diffe[i_d_selector][dim];
        }
        else{
            (*new_r)[dim] = move(inter_diffe[i_d_selector][dim]);
            inter_diffe[i_d_selector][dim] = make_shared<vector<int>>();
            n_move[i_d_selector][dim] = -1;
        }
    }
    new_R = new Rectangle(new_r);
    new_Rs.push_back(new_R);
}

// Calculate intersection and difference of modules, the size of the resulting rectangles,
// and when to move the pointer to the calculated components
void Objective::calculate_operation(){
    new_Rs.clear();
    fill(sizeCheck.begin(), sizeCheck.end(), 1);
    //Calculate components
    for(dim = 0; dim < D; dim++){
        intersection();
        difference();
        pow2d = pow(2, dim);
        for(n = 0; n < n_gen; n++){
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
    //Build the non empty rectangles
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

// Calculus of the variation of objective related
// to the current operation
void Objective::delta_cost(){
	//Initialize
	wR = 0;
	d_dkl = 0;
    new_Rs_total.clear();

	//Operations on Partition
    for(k = to_del.begin(); k != to_del.end(); k++){
        //Build the new_Rs for this k
        calculate_operation();
        //Calculus of DKL
        //Find max
        max_it = max_element(new_Rs.begin(), new_Rs.end(), Compare);
        //Option 1: the max is inside the operating rectangle -> skip it
        if(max_it == new_Rs.begin()){
        	for(n = 1; n < new_Rs.size(); n++){
    			data_tool->get_weight(new_Rs[n]);
    			new_Rs[n]->weight = Brick::weight;
    			new_Rs[n]->doLoss();
    			d_dkl += new_Rs[n]->loss;
    			wR -= new_Rs[n]->weight;
        	}
        	wR += (*k)->weight;
        }
        //option 2: the max is a rectangle that shall be fully calculated
        else{
         	Wacc = 0;
            max_pos = distance(new_Rs.begin(), max_it);

            data_tool->get_weight(new_Rs[0]);
            new_Rs[0]->weight = Brick::weight;
            new_Rs[0]->doLoss();
            Wacc += new_Rs[0]->weight;

        	for(n = 1; n < max_pos; n++){
    			data_tool->get_weight(new_Rs[n]);
    			new_Rs[n]->weight = Brick::weight;
    			new_Rs[n]->doLoss();
				d_dkl += new_Rs[n]->loss;
    			Wacc += new_Rs[n]->weight;
        	}
        	
        	for(n = max_pos+1; n < new_Rs.size(); n++){
    			data_tool->get_weight(new_Rs[n]);
    			new_Rs[n]->weight = Brick::weight;
    			new_Rs[n]->doLoss();
				d_dkl += new_Rs[n]->loss;    			
    			Wacc += new_Rs[n]->weight;
        	}
            
            wR += new_Rs[0]->weight;
        	new_Rs[max_pos]->weight = (*k)->weight - Wacc;
        	new_Rs[max_pos]->doLoss();
			d_dkl += new_Rs[max_pos]->loss;
        }
        delete new_Rs[0];
        d_dkl -= (*k)->loss;
        for(int i = 1; i < new_Rs.size(); i++)
            if(new_Rs[i]->weight > 10e-10)
                new_Rs_total.push_back(new_Rs[i]);
            else{
                delete new_Rs[i];
            }
    }

    if(wR > 10e-10){
        new_R = new Rectangle(rect);
        new_R->weight = wR;
        new_R->doLoss();
        d_dkl += new_R->loss;
        new_Rs_total.push_back(new_R);
    }
    else{
        delete rect;
    }

    d_Rsize = new_Rs_total.size() - to_del.size();
    d_ldkl = dkl_factor*d_dkl;
    d_penalty = penalty_factor*d_Rsize;
    d_cost = d_ldkl + d_penalty;
}

// Apply an the current operation to thep partition
void Objective::do_operation(){
	clean_temp_registers();
    //*** Update partition
    sort(new_Rs_total.begin(), new_Rs_total.end());

    buffer.clear();
    set_difference(R.begin(), R.end(), to_del.begin(), to_del.end(), back_inserter(buffer));
    buffer.swap(R);

    buffer.clear();
    set_union(R.begin(), R.end(), new_Rs_total.begin(), new_Rs_total.end(), back_inserter(buffer));
    buffer.swap(R);

    for(Rectangle* e : to_del){
        if(e == nullptr)
            cout << "nullptr in todel" << endl;
        make_del_reg(e);
        delete e;
    }

    for(Rectangle* e : new_Rs_total){
        make_add_reg(e);
    }

    //*** Setting up registres III
    update_registers();

    //Update objectif
	Rsize += d_Rsize;
    loss += d_dkl;
    dkl += d_dkl;
	ldkl += d_ldkl;
	penalty += d_penalty;
	cost += d_cost;

}

// Cancel the current operation
void Objective::cancel_operation(){
	for(Rectangle* e : new_Rs_total){
        delete e;
    }
    new_Rs_total.clear();
}

// Print details related to the objetive
void Objective::print_cost(){
    cout << "Data of objective : " << endl;
    cout << "lambda         : " << lambda << endl;
    cout << "info           : " << info << endl;
    cout << "loss           : " << loss << endl;
    cout << "dkl            : " << dkl << endl;
    cout << "ldkl           : " << ldkl << endl;
    cout << "penalty        : " << penalty << endl;
    cout << "cost           : " << cost << endl;
    cout << "dkl_norm       : " << dkl_norm << endl;
    cout << "dkl_factor     : " << dkl_factor << endl;
    cout << "penalty_factor : " << penalty_factor << endl << endl;
    
    cout << "d_dkl     : " << d_dkl << endl;
    cout << "d_Rsize   : " << d_Rsize << endl;
    cout << "d_ldkl    : " << d_ldkl << endl;
    cout << "d_penalty : " << d_penalty << endl;
    cout << "d_cost    : " << d_cost << endl << endl;

    cout << "weight_check          : " << weight_check << endl;
    cout << "info_check            : " << info_check << endl;
    cout << "loss_check            : " << loss_check << endl;
    cout << "calculated_loss_check : " << calculated_loss_check << endl;
    cout << "dkl_check             : " << dkl_check << endl;
    cout << "ldkl_check            : " << ldkl_check << endl;
    cout << "penalty_check         : " << penalty_check << endl;
    cout << "cost_check            : " << cost_check << endl << endl;
}

// Print details for each rectangle in the partition
// related to the objective 
void Objective::print_cost_rectangles(){
    cout << "Data of objective in rectangles: " << endl;
    for(Rectangle* e : R){
        cout << "Rectangle : " << e << endl;
        cout << "weight    : " << e->weight << endl;
        cout << "info      : " << e->info << endl;
        cout << "loss      : " << e->loss << endl;
        cout << "dkl       : " << e->dkl << endl;

        cout << "weight_check : " << e->weight_check << endl;
        cout << "info_check   : " << e->info_check << endl;
        cout << "loss_check   : " << e->loss_check << endl;
        cout << "dkl_check    : " << e->dkl_check << endl;
        cout << endl;
    }
}