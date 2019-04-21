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
    - `$ ./single.exe {Dataset Directory Path} {Optional Min Support Threshold}`
    - For example:
        - `$ ./single.exe "Data/Random" 2`
        - `$ ./multi.exe "Data/Test" 3`
