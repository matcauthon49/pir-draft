# Running tests

Run the `script.sh` file that builds executable `test` in `src/build`. To test any functionalities, use the `main.cpp` file in src. All the files are moved to `src` folder with a `CMakeLists.txt` file that builds the project.

# Setting number of threads
In `script.sh` set `export OMP_NUM_THREADS` in line 46 and set the global variable in dpf.cpp   `int nt` to same number that you want to use in parallelization.