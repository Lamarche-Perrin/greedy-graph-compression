# Greedy Graph Compression

This program is a C++ implementation of a greedy algorithm solving the graph compression problem (GCP). Several snippets are provided in order to introduce the different classes of the project.

For details regarding the GCP, see <https://arxiv.org/abs/1807.06874>:  
Robin Lamarche-Perrin. An Information-theoretic Framework
for the Lossy Compression of Link Streams. arXiv:1807.06874


## Clone and Compile

You need to have **git**, **gcc** and **make** installed and working on your machine.

Then, run:
```
git clone https://github.com/Lamarche-Perrin/greedy-graph-compression.git
cd greedy-graph-compression
make
```

## Project Directories

* Sources: `src`
* Headers: `include`
* Tutorials: `tutorials`
* Binaries: `bin`
* Building files: `build`
* Input data: `input`
* Output data: `output`

## Tutorials
There are six files in the `tutorials` directory that contain different `main()` functions introducing the six core classes of the program. Only one of these six files can be in the `src` directory when compiling the project. We present these classes in order of inheritance.

##### `test/Create_Builder.cpp` 
The Builder class described in `include/Builder.h` is used to initialize the required data structures. It contains methods to build an initial partition, to build registers, and a few methods to print the content of these structures and of the graph data.

##### `test/Create_Checker.cpp`
The Checker class described in `include/Checker.h` contains many methods to perform various checks on the partitions and the registers. It includes checks of the covering and packing constraints.

##### `test/Create_Operator.cpp`
The Operator class described in `include/Operator.h` implements operations on partitions. Here are methods to randomly draw rectangles and to insert them in a partition.

##### `test/Create_Objective.cpp`
The Objective class described in `include/Objective.h` re-implements the Operator class by adding the calculation of the objective function. It also provides a method to check the computation of this objective function.

##### `test/Create_Optimizer.cpp`
The Optimizer class described in `include/Optimizer.h` provides methods to search for an optimized partition.

##### `test/Create_Compressor.cpp`
The Compressor class described in `include/Compressor.h` uses a dichotomic search algorithm to allow graph compression at various scales.


## Author and Founding

This program has been developed by Léonard Panichi while working as a research engineer at the [Complex Networks](http://www.complexnetworks.fr/) team of the [Computer Science Laboratory of Paris 6](https://www.lip6.fr/). This work has been founded by the [European Commission FETPROACT 2016-2017 program](https://ec.europa.eu/research/participants/portal/desktop/en/opportunities/h2020/calls/h2020-fetproact-2016-2017.html) under grant 732942 ([ODYCCEUS](https://www.odycceus.eu/)).


## License

Copyright © 2018 Léonard Panichi
(<Leonard.Panichi@gmail.com>)

This program is free software: you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the Free
Software Foundation, either version 3 of the License, or (at your option)
any later version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
more details.

You should have received a copy of the GNU General Public License along
with this program. If not, see <http://www.gnu.org/licenses/>.
