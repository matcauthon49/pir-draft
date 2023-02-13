# Building code
Install GF2X library and NTL library. While installing NTL library, make sure to use GF2X for polynomial multiplication which gives more speedup. To configure NTL to use GF2X, do `./configure NTL_GF2X_LIB=on TUNE=auto` during installation.
Run the `build.sh` file that builds executable `test, p0, p1, p2, c` in `src/build`. The code is available in the `src` folder.

# Running protocol

Run `run.sh` file to execute the protocol.

# Setting Database size and index to query
Set the `input_size` and `Bin` variable to log of desired database size in all files. By default, it has been set to $2^{24}$. Set the `ind` variable to index that is queried in c.cpp file. By default, it has been set to $2859$.