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


#pragma once
#include <sstream>
#include "Data_Tree.h"

class Reader : public Data_Tree{

	public:
		//for json and csv exploitation
		std::string path_to_input;
		std::string input_filename;
		std::string extension;
		
		std::string word;
		std::string line;
		std::vector<std::string> data_line;

	public:
		Reader(std::string input_filename);
		Reader();
		~Reader();

		void extract_extension();
		void extract_datas_csv();
		void extract_datas_json();

		//Read file utilities
		void get_metadata(std::ifstream&);
		int get_data_pos(std::ifstream&);
		int clean_word(std::string& word);
		bool no_ket(std::string& word);
};
