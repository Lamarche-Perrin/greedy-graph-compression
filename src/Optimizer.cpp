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


#include "Optimizer.h"

using namespace std;
using namespace std::chrono;

// Constructor for Optimizer with Data from a Random Matrix Model
Optimizer::Optimizer(vector<int> N, vector<double> params, double lambda):
    Objective(N, params, lambda){
    experiment = "Matrix Generated, params = ";
    for(auto const& w : params){
        experiment += (to_string(w)+",");
    }
    init();
}

// Constructor for Optimizer with Data imported from a file
Optimizer::Optimizer(string input_filename, double lambda):
    Objective(input_filename, lambda){
    experiment = "Input Data : " + input_filename;
    init();
}

// Constructor for Optimizer with Benchmark Data
Optimizer::Optimizer(string type, int n, double lambda):
    Objective(type, n, lambda){
    experiment = "Benchmark graph type " + type + " scale : " + to_string(n);
    init();
}

//Default Constructor
Optimizer::Optimizer(){
}

Optimizer::~Optimizer(){
    cout << "Optimizer Destructor" << endl;
}


// Initialize objects
void Optimizer::init(){
    path_to_output_data = "output/";
    make_filenames();
}

// Make filenames
void Optimizer::make_filenames(string prefix){
    filename = path_to_output_data + "gag05_clean_" + prefix;

    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d_at_%H-%M-%S_");
    auto date = oss.str();
    filename += date;
    meta_filename = filename + "meta.txt";
    data_filename = filename + "optim_data.csv";
    partition_filename = filename + "partition.json";
    generated_data_filename = filename + "matrix_data.csv";
}

// Initialize data vector
void Optimizer::init_data(){
    data_size = 13;
    data.resize(data_size);
    data[0] = cost;     //cost      0
    data[1] = ldkl;     //ldkl      1
    data[2] = penalty;  //penalty   2
    data[3] = Rsize;    //Rsize     3
    data[4] = 0;        //d_cost    4
    data[5] = 0;        //d_ldkl    5
    data[6] = 0;        //d_penalty 6
    data[7] = 0;        //d_Rsize   7
    data[8] = 0;        //found     8      
    data[9] = 0;        //accepted  9
    data[10] = 0;       //tfind     10
    data[11] = 0;       //tdelta    11
    data[12] = 0;       //tdone     12
}

// Fill data vector with class attributes
void Optimizer::fill_data(){
    data[0] = cost;
    data[1] = ldkl;
    data[2] = penalty;
    data[3] = Rsize;
    data[4] = d_cost;
    data[5] = d_ldkl;
    data[6] = d_penalty;
    data[7] = d_Rsize;
    data[8] = found;
    data[9] = accepted;
    ope_time = duration_cast<duration<double>>(tfind - t0);
    data[10] = ope_time.count();
    ope_time = duration_cast<duration<double>>(tdelta - tfind);
    data[11] = ope_time.count();
    ope_time = duration_cast<duration<double>>(tdone - tdelta);
    data[12] = ope_time.count();
}

void Optimizer::set_parameters(int _nfail_limit, int _ndiv_start, int _ndiv_increm, int _ncycle){
    nfail_limit = _nfail_limit;
    ndiv_start = _ndiv_start;
    ndiv_increm = _ndiv_increm;
    ncycle = _ncycle;
}


// Do a gradient descente with the cluster operator
void Optimizer::gradient_descente_while_cluster(int nfail_limit){
    cout << "Gradient descente while cluster" << endl;
    cout << "data_filename : " << data_filename << endl;
    ofstream datafile;
    datafile.open(data_filename, ios::out);

    if (datafile.is_open())
    {
        algorithm = "gradient_descente";
        initialize_objective();
        init_data();
        write_column_names(datafile);
        write_vector(data, datafile);
        nfail = 0;
        tbegin = steady_clock::now();
        while(nfail < nfail_limit and Rsize > 1){
            cout << "nfail : " << nfail << "  nfail limit : " << nfail_limit << " Rsize : " << Rsize << endl;
            t0 = steady_clock::now();
            if(find_cluster()){
                tfind = steady_clock::now();
                found = 1;
                delta_cost();
                tdelta = steady_clock::now();
                if(d_cost < 0){
                    nfail = 0;
                    accepted = 1;
                    do_operation();
                    tdone = steady_clock::now();
                }
                else{
                    nfail++;
                    cancel_operation();
                    accepted = 0;
                    tdone = steady_clock::now();
                }
            }
            else{
                tfind = steady_clock::now();
                tdelta = steady_clock::now();
                tdone = steady_clock::now();
                accepted = 0;
                found = 0;
                nfail++;
                clean_operator();
            }
            fill_data();
            write_vector(data, datafile);
        }
        tend = steady_clock::now();
        optim_time = duration_cast<duration<double>>(tend - tbegin);
        datafile.close();
        write_annexes();
    }
    else
        std::cout << "Error opening data file" << endl;
}

// Do a gradient descent with the cluster operator
// and does not write the data calculated throught the gradient descente
void Optimizer::gradient_descente_while_cluster_succinct(int nfail_limit){
    cout << "Gradient descente while cluster succinct" << endl;
    algorithm = "gradient_descente_succinct";
    initialize_objective();
    nfail = 0;
    tbegin = steady_clock::now();
    while(nfail < nfail_limit and Rsize > 1){
        cout << "nfail : " << nfail << "  nfail limit : " << nfail_limit << " Rsize : " << Rsize << endl;
        if(find_cluster()){
            found = 1;
            delta_cost();
            if(d_cost < 0){
                nfail = 0;
                accepted = 1;
                do_operation();
            }
            else{
                nfail++;
                cancel_operation();
                accepted = 0;
            }
        }
        else{
            accepted = 0;
            found = 0;
            nfail++;
            clean_operator();
        }
    }
    tend = steady_clock::now();
    optim_time = duration_cast<duration<double>>(tend - tbegin);
    write_annexes();
}

// Do a mix programme
void Optimizer::mix_program(){
    cout << "Mix program" << endl;
    cout << "data_filename : " << data_filename << endl;
    ofstream datafile;
    datafile.open(data_filename, ios::out);

    if(datafile.is_open())
    {
        algorithm = "mix program";

        initialize_objective();
        init_data();
        write_column_names(datafile);
        write_vector(data, datafile);

        tbegin = steady_clock::now();

        gradient_descente_while_union(data, datafile);
        best_solution = make_tuple(lambda, cost, copy_partition(), Rsize, info, loss, dkl, ldkl, penalty);

        for(int cycle = 0; cycle < ncycle; cycle++){
            cout << "Cycle : " << cycle << endl;
            divide_random(data, datafile);
            gradient_descente_while_union(data, datafile);
            
            if(get<1>(best_solution) > cost){
                clean_R(*get<2>(best_solution));
                best_solution = make_tuple(lambda, cost, copy_partition(), Rsize, info, loss, dkl, ldkl, penalty);
                ndiv_limit = ndiv_start;
            }
            else{
                restart_from_best();
                ndiv_limit += ndiv_increm;
            }
        }
        tend = steady_clock::now();
        optim_time = duration_cast<duration<double>>(tend - tbegin);
        datafile.close();
        data_tool->print_data_on_file(generated_data_filename);
        write_header();
        write_partition(best_solution);
    }
    else
        std::cout << "Error opening data file" << endl;
}

// Do a mix programme
// and does not write the data calculated throught the gradient descente
void Optimizer::mix_program_succinct(){
    cout << "Mix program succinct" << endl;
    cout << "data_filename : " << data_filename << endl;
    algorithm = "mix program";

    initialize_objective();
    init_data();
    tbegin = steady_clock::now();

    gradient_descente_while_union_succinct();
    best_solution = make_tuple(lambda, cost, copy_partition(), Rsize, info, loss, dkl, ldkl, penalty);

    for(int cycle = 0; cycle < ncycle; cycle++){
        divide_random_succinct();
        gradient_descente_while_union_succinct();
        
        if(get<1>(best_solution) > cost){
            clean_R(*get<2>(best_solution));
            best_solution = make_tuple(lambda, cost, copy_partition(), Rsize, info, loss, dkl, ldkl, penalty);
            ndiv_limit = ndiv_start;
        }
        else{
            restart_from_best();
            ndiv_limit += ndiv_increm;
        }
    }
    tend = steady_clock::now();
    optim_time = duration_cast<duration<double>>(tend - tbegin);
    // data_tool->print_data_on_file(generated_data_filename);
    // write_header();
    // write_partition(best_solution);
}

// Do a gradient descente
void Optimizer::gradient_descente_while_union(vector<double>& data, ofstream& datafile){
    nfail = 0;
    while(nfail < nfail_limit and Rsize > 1){
        cout << "nfail : " << nfail << "  nfail limit : " << nfail_limit << " Rsize : " << Rsize << endl;
        t0 = steady_clock::now();
        if(find_union_constrained()){
            tfind = steady_clock::now();
            found = 1;
            delta_cost();
            tdelta = steady_clock::now();
            if(d_cost < 0){
                nfail = 0;
                accepted = 1;
                do_operation();
                tdone = steady_clock::now();
            }
            else{
                nfail++;
                cancel_operation();
                accepted = 0;
                tdone = steady_clock::now();
            }
        }
        else{
            tfind = steady_clock::now();
            tdelta = steady_clock::now();
            tdone = steady_clock::now();
            accepted = 0;
            found = 0;
            nfail++;
            clean_operator();
        }
        fill_data();
        write_vector(data, datafile);
    }
    cout << "Gradient descente while union done, size of R : " << Rsize << endl;
}

// Do a gradient descente
// and does not write the data calculated throught the gradient descente
void Optimizer::gradient_descente_while_union_succinct(){
    nfail = 0;
    while(nfail < nfail_limit and Rsize > 1){
        t0 = steady_clock::now();
        if(find_union_constrained()){
            tfind = steady_clock::now();
            found = 1;
            delta_cost();
            tdelta = steady_clock::now();
            if(d_cost < 0){
                nfail = 0;
                accepted = 1;
                do_operation();
                tdone = steady_clock::now();
            }
            else{
                nfail++;
                cancel_operation();
                accepted = 0;
                tdone = steady_clock::now();
            }
        }
        else{
            tfind = steady_clock::now();
            tdelta = steady_clock::now();
            tdone = steady_clock::now();
            found = 0;
            clean_operator();
        }
    }
}

// Do random divisions
void Optimizer::divide_random(vector<double>& data, ofstream& datafile){
    for(int i = 0; i < ndiv_limit; i++){
        if(find_division()){
            found = 1;
            delta_cost();
            do_operation();
        }
        else{
            found = 0;
            clean_operator();
        }
        fill_data();
        write_vector(data, datafile);
    }
}

// Do random divisions
// and does not write the data calculated throught the gradient descente
void Optimizer::divide_random_succinct(){
    for(int i = 0; i < ndiv_limit; i++){
        if(find_division()){
            found = 1;
            delta_cost();
            do_operation();
        }
        else{
            found = 0;
            clean_operator();
        }
    }
}

// Copy a previously saved partition into the class attributes
// One can start over an optimization process after using this function
void Optimizer::restart_from_best(){
    //Copier la partition de best dans R
    clean_R();
    copy_partition(get<2>(best_solution));

    //Reconstruire registres
    clean_registers();
    reg_of_R();

    //Recopier les paramètres objectifs
    cost    = get<1>(best_solution);
    Rsize   = get<3>(best_solution);
    info    = get<4>(best_solution);
    loss    = get<5>(best_solution);
    dkl     = get<6>(best_solution);
    ldkl    = get<7>(best_solution);
    penalty = get<8>(best_solution);
}

// To initialize the file where the data will be written
void Optimizer::write_column_names(std::ofstream& file){
    file << "cost ";
    file << "ldkl ";
    file << "penalty ";
    file << "Rsize ";
    file << "d_cost ";
    file << "d_ldkl ";
    file << "d_penalty ";
    file << "d_Rsize ";
    file << "found ";
    file << "accepted ";
    file << "tfind ";
    file << "tdelta ";
    file << "tdone ";
    file << endl;
}

// Write all the files that are not the data files
void Optimizer::write_annexes(){
    data_tool->print_data_on_file(generated_data_filename);
    write_header();
    write_partition();
}

// Write the meta data file
void Optimizer::write_header(){
    ofstream metafile;
    metafile.open(meta_filename, ios::out);
    if (metafile.is_open())
    {
        metafile << "GagV05 Optimization" << endl;
        metafile << "Experiment : " << experiment << endl;
        metafile << "Algorithm : " << algorithm << endl << endl;

        metafile << "Problem metadata :" << endl;
        metafile << "Dimensions : " << D << endl;
        metafile << "N : ";
        write_vector(N, metafile);
        metafile << "E : " << E << endl;
        metafile << "Density : " << (double)E/(double)n_permutations << endl;
        metafile << "Ew : " << Ew << endl << endl;
        metafile << "Algorithm metadata : " << endl;
        metafile << "lambda : " << lambda << endl;

        metafile << "warning : the following meta value are not fully set automatically in V04" << endl;
        metafile << "please check the main function to know what where the values used" << endl;
        
        metafile << "Initial partition : " << starting_partition << endl;
        metafile << "Shuffling         : none" << endl;
        metafile << "Operator set      : all" << endl;
        metafile << "Probability laws  : binomial, p = 0.5" << endl;
        metafile << "RL                : off" << endl;
        metafile << endl << endl;
        metafile << "Time spent in optim : " << optim_time.count() << " seconds." << endl;
        metafile << endl << endl;
        metafile << "Printed Values : " << endl;
        write_column_names(metafile);        
        metafile.close();
    }
    else
        std::cout << "Error opening meta file for writing" << endl;
}

//Write the partition contained in R, the class attributes
void Optimizer::write_partition(){
    ofstream partfile;
    partfile.open(partition_filename, ios::out);
    if (partfile.is_open())
    {   
        partfile << setprecision(15) << "{\"final\":{";
        partfile << "\"E\":" << E << ", ";
        partfile << "\"Ew\":" << Ew << ", ";
        partfile << "\"Rsize\":" << Rsize << ", ";
        partfile << "\"lambda\":" << lambda << ", ";
        partfile << "\"info\":" << info << ", ";
        partfile << "\"loss\":" << loss << ", ";
        partfile << "\"dkl\":" << dkl << ", ";
        partfile << "\"ldkl\":" << ldkl << ", ";
        partfile << "\"penalty\":" << penalty << ", ";
        partfile << "\"cost\":" << cost;
        partfile << "}," << endl;



        partfile << "\"R\":{" << endl;
        for(int r = 0; r < R.size(); r++){
            data_tool->get_info(R[r]);
            data_tool->get_non_zero(R[r]);
            R[r]->info = Brick::info;
            R[r]->doLoss();
            R[r]->doDKL();

            partfile << "\"" << r << "\":{\"geometry\":[";
            for(int d = 0; d < D; d++){
                partfile << "[";
                for(int e = 0; e < (*R[r]->rectangle)[d]->size(); e++){
                    partfile << (*(*R[r]->rectangle)[d])[e];
                    if(e < (*R[r]->rectangle)[d]->size()-1)
                        partfile << ","; 
                }
                partfile << "]";
                if(d < D-1)
                    partfile << ",";
            }
            partfile << "], \"weight\" : " << R[r]->weight << ", ";
            partfile << "\"info\" : " << R[r]->info << ", ";
            partfile << "\"loss\" : " << R[r]->loss << ", ";
            partfile << "\"dkl\" : " << R[r]->dkl << ", ";
            partfile << "\"size\" : " << R[r]->size << ", ";
            partfile << "\"non zero\" : " << Brick::non_zero;
            partfile << "}";
            if(r < R.size()-1)
                partfile << ",";
            partfile << endl;
        }
        partfile << "}}" << endl;
        partfile.close();
    }
    else
        std::cout << "Error opening partition file for writing" << endl;
}

// Write the partition given as parameters
void Optimizer::write_partition(vector<Rectangle*>& R_){
    get_objective(R_);
    ofstream partfile;
    partfile.open(partition_filename, ios::out);
    if (partfile.is_open())
    {   
        partfile << setprecision(15) << "{\"final\":{";
        partfile << "\"E\":" << E << ", ";
        partfile << "\"Ew\":" << Ew << ", ";
        partfile << "\"Rsize\":" << Rsize << ", ";
        partfile << "\"lambda\":" << lambda << ", ";
        partfile << "\"info\":" << info << ", ";
        partfile << "\"loss\":" << loss << ", ";
        partfile << "\"dkl\":" << dkl << ", ";
        partfile << "\"ldkl\":" << ldkl << ", ";
        partfile << "\"penalty\":" << penalty << ", ";
        partfile << "\"cost\":" << cost;
        partfile << "}," << endl;



        partfile << "\"R\":{" << endl;
        for(int r = 0; r < R_.size(); r++){
            data_tool->get_info(R_[r]);
            data_tool->get_non_zero(R_[r]);
            R_[r]->info = Brick::info;
            R_[r]->doLoss();
            R_[r]->doDKL();

            partfile << "\"" << r << "\":{\"geometry\":[";
            for(int d = 0; d < D; d++){
                partfile << "[";
                for(int e = 0; e < (*R_[r]->rectangle)[d]->size(); e++){
                    partfile << (*(*R_[r]->rectangle)[d])[e];
                    if(e < (*R_[r]->rectangle)[d]->size()-1)
                        partfile << ","; 
                }
                partfile << "]";
                if(d < D-1)
                    partfile << ",";
            }
            partfile << "], \"weight\" : " << R_[r]->weight << ", ";
            partfile << "\"info\" : " << R_[r]->info << ", ";
            partfile << "\"loss\" : " << R_[r]->loss << ", ";
            partfile << "\"dkl\" : " << R_[r]->dkl << ", ";
            partfile << "\"size\" : " << R_[r]->size << ", ";
            partfile << "\"non zero\" : " << Brick::non_zero;
            partfile << "}";
            if(r < R_.size()-1)
                partfile << ",";
            partfile << endl;
        }
        partfile << "}}" << endl;
        partfile.close();
    }
    else
        std::cout << "Error opening partition file for writing" << endl;
}

// Write the partition contained in a stroage
void Optimizer::write_partition(storage& S){
    ofstream partfile;
    partfile.open(partition_filename, ios::out);
    if (partfile.is_open())
    {   
        partfile << setprecision(15) << "{\"final\":{";
        partfile << "\"E\":" << E << ", ";
        partfile << "\"Ew\":" << Ew << ", ";
        partfile << "\"Rsize\":" << get<3>(S) << ", ";
        partfile << "\"lambda\":" << get<0>(S) << ", ";
        partfile << "\"info\":" << get<4>(S) << ", ";
        partfile << "\"loss\":" << get<5>(S) << ", ";
        partfile << "\"dkl\":" << get<6>(S) << ", ";
        partfile << "\"ldkl\":" << get<7>(S) << ", ";
        partfile << "\"penalty\":" << get<8>(S) << ", ";
        partfile << "\"cost\":" << get<1>(S);
        partfile << "}," << endl;



        partfile << "\"R\":{" << endl;
        for(int r = 0; r < get<2>(S)->size(); r++){
            data_tool->get_info((*get<2>(S))[r]);
            data_tool->get_non_zero((*get<2>(S))[r]);
            (*get<2>(S))[r]->info = Brick::info;
            (*get<2>(S))[r]->doLoss();
            (*get<2>(S))[r]->doDKL();

            partfile << "\"" << r << "\":{\"geometry\":[";
            for(int d = 0; d < D; d++){
                partfile << "[";
                for(int e = 0; e < ( *(((*get<2>(S))[r]) ->rectangle)) [d]->size(); e++){
                    partfile << (*(( *(((*get<2>(S))[r]) ->rectangle)) [d]))[e];
                    if(e < ( *(((*get<2>(S))[r]) ->rectangle)) [d]->size()-1)
                        partfile << ",";
                }
                partfile << "]";
                if(d < D-1)
                    partfile << ",";
            }
            partfile << "], \"weight\" : " << (*get<2>(S))[r]->weight << ", ";
            partfile << "\"info\" : " << (*get<2>(S))[r]->info << ", ";
            partfile << "\"loss\" : " << (*get<2>(S))[r]->loss << ", ";
            partfile << "\"dkl\" : " << (*get<2>(S))[r]->dkl << ", ";
            partfile << "\"size\" : " << (*get<2>(S))[r]->size << ", ";
            partfile << "\"non zero\" : " << Brick::non_zero;
            partfile << "}";
            if(r < get<2>(S)->size()-1)
                partfile << ",";
            partfile << endl;
        }
        partfile << "}}" << endl;
        partfile.close();
    }
    else
        std::cout << "Error opening partition file for writing" << endl;
}
