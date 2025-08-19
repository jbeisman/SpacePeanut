
#include <iostream>
#include <string>
#include <sstream>
#include "get_input.hh"


std::tuple<int, int, int, float, float> get_input()
{
    const int   default_nbods = 2097152;
    const int   default_ngrid = 256;
    const int default_nsteps  = 1000;
    const float default_gmax  = 128.0f;
    const float default_rshft = 50.0f;

    auto return_default = [=](){
        return std::make_tuple(default_nbods, default_ngrid, default_nsteps, default_gmax, default_rshft);
    };

    std::string user_choice;
    std::cout << "Do you want to customize the input parameters? (y/n): ";
    std::getline(std::cin, user_choice);

    for (char &c : user_choice) {
        c = std::tolower(c);
    }

    if (user_choice == "y") {

        std::string input_line;
        int nbods, ngrid, nsteps;
        float gmax, redshift;

        std::cout << "\n Enter: NBODS NGRID LENGTH RSHIFT NSTEPS\n";
        std::cout << "NBODS <int>       number of particles\n";
        std::cout << "NGRID <int>       N from the NxNxN grid\n";
        std::cout << "NSTEPS <int>      number of timesteps\n";
        std::cout << "LENGTH <float>    grid length\n";
        std::cout << "RSHIFT <float>    cosmological redshift parameter\n";
        std::cout << "Enter on a single line separated by spaces, eg. 2097152 256 1000 128.0 50.0 \n\n";
        std::getline(std::cin, input_line);

        std::stringstream ss(input_line);

        if (ss >> nbods >> ngrid >> nsteps >> gmax >> redshift) {
            std::cout << "User input parsing success" << std::endl;
            return std::make_tuple(nbods, ngrid, nsteps, gmax, redshift);
        } else {
            std::cerr << "Error: Could not parse input. Returning default values." << std::endl;
            return return_default();
        }
    } else if (user_choice == "n") {
        std::cout << "Returning default values" << std::endl;
        return return_default();
    }
    
    std::cerr << "Invalid choice. Please enter 'y' or 'n'. Returning default values." << std::endl;
    return return_default();
}
