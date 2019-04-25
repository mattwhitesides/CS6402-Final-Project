# Parallel Apriori Based Frequent Subgraph Mining
## Matt Whitesides
### CS6402 Final Project

Requirements:
- gcc-g++ version 7.0 or later Or a recent Visual Studio w/ Visual C++.
- make version 4.0 or later.
- Tested on rc02xcs213.managed.mst.edu

How to build:
- Download the repo or the zip file to your desired location.
- Open up a cmd or bash prompt and navigate to the base "CS6402-Final-Project" directory.
- To make the multi threaded application run `$ make` or `$ make multi`.
- To make the single threaded application run `$ make single` .
- To create the test project run `$ make test`.

Also included is a Visual Studio C++ solution for the multi threaded application, located in the VS_Multi folder.
- Open up VS_Multi.sln using Visual Studio.
- Build the solution preferably using x64.
- The resulting exe in the Release folder will behave the same as if made using the G++/Makefile.
- Note: For Windows this is preferable as there are issues w/ Cygwin and multi-threading, however I did not try running it outside of the Cygwin container so your milage may very.

How to use:
- To run the single or multi threaded project execute the following:
    - `$ ./single {Dataset Directory Path} {Optional Min Support Threshold} {Optional Output File Name}`
    - `$ ./multi {Dataset Directory Path} {Optional Min Support Threshold} {Optional Thread Count} {Optional Output File Name}`
    - Or to run with default parameters you can omit the optional arguments:
    - `$ ./single {Dataset Directory Path}`
    - `$ ./multi {Dataset Directory Path}`
    - For example:
        - Linux:
        - `$ ./single "Data/Test" 2 "output.txt"`
        - `$ ./multi "Data/Test" 2 2 "output.txt"`
        - Windows:
        - `$ single.exe "Data/Test" 2 "output.txt"`
        - `$ multi.exe "Data/Test" 2 2 "output.txt"`
        
Data from included some of the included datasets:
- [Social circles: Facebook](https://snap.stanford.edu/data/ego-Facebook.html)
- [Social circles: Twitter](https://snap.stanford.edu/data/ego-Twitter.html)
