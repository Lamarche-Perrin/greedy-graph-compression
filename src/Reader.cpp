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


#include "Reader.h"
using namespace std;

Reader::Reader(string input_filename): Data_Tree(), input_filename(input_filename){
	//cout << "Reader constructor" << endl;
	path_to_input = "input/";
	extract_extension();
	if(extension == "json")
		extract_datas_json();
	else if (extension == "csv")
		extract_datas_csv();
	else
		cout << "Extension of data file not known : " << extension << endl;
	make_brick_spaces();
}

Reader::Reader(){
	//cout << "Reader default constructor" << endl;
}

Reader::~Reader(){}

void Reader::extract_extension(){
	extension = input_filename.substr(input_filename.find_last_of(".") + 1);
	input_filename = path_to_input + input_filename;
}

void Reader::extract_datas_csv(){
	cout << input_filename << endl;
	ifstream data_file;
	data_file.open(input_filename);
    if (data_file.is_open())
    {
    	int nline = 0;
    	int ncol = 0;
    	vector<string> data_line;
    	while(getline(data_file, line)){
    		//cout << line << endl;
    		data_line.clear();
    		istringstream iss(line);
    		copy(istream_iterator<string>(iss),
    			 istream_iterator<string>(),
     			 back_inserter(data_line));

    		nodes.push_back(new Data_Brick(1));
    		
    		for(ncol = 0; ncol < data_line.size(); ncol++){
    			value = stod(data_line[ncol]);
    			if(value != 0){
    				//cout << "Inserting value" << endl;
	    			nodes[nline]->insert_value(ncol, value);
    				E++;
    				Ew += value;
    			}
    		}
    		nline++;
    	}
    	N.push_back(nline);
    	N.push_back(ncol);
    	D = 2;
	    data_file.close();
   	    cout << "Datas Extracted" << endl;
      	cout << "Ew : " << Ew << endl;
   	    cout << "E  : " << E  << endl;
   	    // if(Ew != 1.0)
   	    // 	normalize();
   	    cout << "Datas Normalized" << endl;
    }
    else
		cout << "Can't find the csv file " << input_filename << endl;	
}


void Reader::extract_datas_json(){
	cout << input_filename << endl;
	ifstream data_file;

	get_metadata(data_file);
	int dataPos = get_data_pos(data_file);

	data_file.open(input_filename);
    if (data_file.is_open())
    {
    	int end_of_data = 0;
		data_file.seekg(dataPos, ios::beg);
	    while (data_file >> word) {
	    	//cout << word << " - tokens : word[0] " << word[0] << ",  word[word.size()-2] : "
	    	//	<< word[word.size()-2] << " word[word.size()-3] :" << word[word.size()-3] << endl;
	    	if(word[word.size()-2] == ']'){
	    		if(word[word.size()-3] == ']')
	    			end_of_data = 1;
	    		clean_word(word);
	 			value = stod(word);
	    		make_bricks();
	    		hyperlinks.clear();
   				Ew += value;
	    	}
			else{
				clean_word(word);
				hyperlinks.push_back(stoi(word));
			}
			if(end_of_data == 1)
				break;
		}
	    data_file.close();
        cout << "Datas Extracted" << endl;
        //cout << "Ew : " << Ew << endl;
   	    //cout << "E  : " << E  << endl;

        // if(Ew != 1.0)
   	    // 	normalize();
  	    //cout << "Datas Normalized" << endl;

    }
    else
    	cout << "can't find the file " << input_filename << endl;
}



void Reader::get_metadata(std::ifstream& data_file){
	data_file.open(input_filename);
	string meta1 = "dimension";
	string meta2 = "E";
	string meta3 = "N";


    if (data_file.is_open())
    {
	    while (data_file >> word) {
	    	for(int c = word.size()-1; c >=0; c--)
	    		if(!isalpha(word[c]))
	    				word.erase(c,1);

	    	if(word == meta1)
	    	{
	    		data_file >> word;
	    		for(int c = word.size()-1; c >=0; c--)
	    			if(!isdigit(word[c]))
	    				word.erase(c,1);
				D = stoi(word);
				cout << "D : " << D << endl;
			}
	    	if(word == meta2)
	    	{
	    		data_file >> word;
	    		for(int c = word.size()-1; c >=0; c--)
	    			if(!isdigit(word[c]))
	    				word.erase(c,1);
				E = stoi(word);
				cout << "E : " << E << endl;
			}
			if(word == meta3)
	    	{
	    		bool noket = true;
	    		word.clear();
	    		while(noket){
		    		data_file >> word;
		    		noket = no_ket(word);
		    		for(int c = word.size()-1; c >=0; c--)
		    			if(!isdigit(word[c]))
		    				word.erase(c,1);
					N.push_back(stoi(word));
				}
				cout << "N : ";
				for(int e : N)
					cout << e << ", ";
				cout << endl;
			}
		}
		data_file.close();
		cout << "Metas done" << endl;
	}
    else
    	cout << "can't find the file " << input_filename << " for meta extraction" << endl;

    nodes.resize(N[0], nullptr);
}

int Reader::get_data_pos(std::ifstream& data_file){
	data_file.open(input_filename);
    if (data_file.is_open())
    {
    	int dataPos;
	    while (data_file >> word) {
	    	for(int c = word.size()-1; c >=0; c--)
	    		if(!isalpha(word[c]))
	    				word.erase(c,1);
			if(word == "datas")
				dataPos = data_file.tellg();
			//cout << dataPos << endl;
		}
	    data_file.close();	
    	return dataPos;
    }
    else
    	cout << "file not open" << endl;
    return -1;
}

int Reader::clean_word(string& word){
	for(int c = word.size()-1; c >=0; c--)
		if(word[c] == '"')
			word.erase(c,1);
		else if(word[c] == '[')
			word.erase(c,1);
		else if(word[c] == ']')
			word.erase(c,1);
		else if(word[c] == ',')
			word.erase(c,1);
}

bool Reader::no_ket(string& word){
	bool ket = true;
	for(int c = 0; c < word.size(); c++){
		if(word[c] == ']'){
			ket = false;
			break;
		}
	}
	return ket;
}

