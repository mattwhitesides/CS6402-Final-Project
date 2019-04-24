# Parallel Apriori Based Frequent Subgraph Mining
## Matt Whitesides
### CS6402 Final Project

Requirements:
- gcc-g++ version 7.0 or later.
- make version 4.0 or later.
- Tested on rc02xcs213.managed.mst.edu

How to build:
- Download the repo or the zip file to your desired location.
- Open up a cmd or bash prompt and navigate to the base "CS6402-Final-Project" directory.
- To make the single threaded application run `$ make` or `$ make single` .
- To make the multi threaded application run `$ make multi`.
- To create the test project run `$ make test`.

How to use:
- To run the single or multi threaded project execute the following:
    - `$ ./single {Dataset Directory Path} {Optional Min Support Threshold} {Optional Output File Name}`
    - `$ ./multi {Dataset Directory Path} {Optional Min Support Threshold} {Optional Thread Count} {Optional Output File Name}`
    - Or do run with default parameters you can omit the optional arguments:
    - `$ ./single {Dataset Directory Path}`
    - `$ ./multi {Dataset Directory Path}`
    - For example:
        - Linux:
        - `$ ./single "Data/Test" 2 "output.txt"`
        - `$ ./multi "Data/Test" 2 2 "output.txt"`
        - Windows:
        - `$ single.exe "Data/Test" 2 "output.txt"`
        - `$ multi.exe "Data/Test" 2 2 "output.txt"`
