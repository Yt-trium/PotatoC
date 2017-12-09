**PotatoC** : The StenC Compiler for MIPS R2000

```
    .-'''-.     
   '        \   
 /   O   O   \  
.      .     '  
|             ' 
\   \____    /  
 `.         /   
    '-...-'`  
 ____   __  ____  __  ____  __    ___ 
(  _ \ /  \(_  _)/ _\(_  _)/  \  / __)
 ) __/(  O ) )( /    \ )( (  O )( (__ 
(__)   \__/ (__)\_/\_/(__) \__/  \___)

```

## Compiling

    make

Works by default on Linux and OSX.
The potatoc program will be available in the `bin` directory.

## Usage 

    potatoc [input_file [output_file]]

By default, potatoc will read user inputs and save the MIPS output in `out.asm`.

## Tests

The folder tests provide a bunch of working examples. And the script `run_scripts.sh` will run all test files and save the resulting MIPS code and debug lines in `test.log`. MIPS files would also be saved in the tests directory.

    ./run_tests.sh
