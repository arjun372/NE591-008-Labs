/**
 * @file ProcessInputs.h
 * @author Arjun Earthperson
 * @date 09/01/2023
 * @brief Process inputs for inlab2
 */

#pragma once

#include <iostream>
#include <iomanip>

#include "utils/Helpers.h"
#include "utils/CheckBounds.h"
#include "utils/FileParser.h"


typedef struct InputParams {
    size_t n;
    size_t m;
    size_t x;
    std::vector<long double> &xData;
    std::vector<long double> &fxData;
} InputParams;

/**
 * @brief This function builds the input options for the program.
 *
 * @return A boost::program_options::options_description object containing the description of the input options.
 */
static boost::program_options::options_description buildInputs() {
    boost::program_options::options_description arguments("Parameters");
    arguments.add_options()
            ("num-points,n", boost::program_options::value<long double>(), "= (optional) number of interpolation points n")
            ("num-samples,m", boost::program_options::value<long double>(), "= number of Lagrange interpolation evaluation points")
            ("x-points,x", boost::program_options::value<std::vector<long double>>(), "= distinct and sorted (x) interpolation points if --input-csv is unset")
            ("fx-points,f", boost::program_options::value<std::vector<long double>>(), "= f(x=n) points if --use-fx-function and --input-csv are unset")
            ("input-csv,i", boost::program_options::value<std::string>(), "= path for input CSV file with two columns [x, f(x)]")
            ("output-csv,o", boost::program_options::value<std::string>(), "= path for output CSV file with five columns [i, x, f(x), L(x), E(x)]")
            ("use-fx-function,F", "= use bundled f(x=n) function");
    return arguments;
}

static bool isUnfilledDoubleLongVector(boost::program_options::variables_map &values, const std::string &key, const size_t expectedSize) {
    try {
        return values[key].as<std::vector<long double>>().size() < expectedSize;
    } catch (...) {
        return true;
    }
}

/**
 * @brief This function performs checks on the input parameters and prompts the user to enter valid inputs if the
 * current inputs are invalid.
 *
 * @param map A boost::program_options::variables_map object containing the input values to be checked.
 */
static void performInputChecks(boost::program_options::variables_map &map) {

    std::string input;

    if(!map.count("output-csv") || map["output-csv"].empty() || !isFileWritable(map["output-csv"].as<std::string>())) {
        while(!map.count("output-csv") || map["output-csv"].empty() || !isFileWritable(map["output-csv"].as<std::string>())) {
            std::cerr << "Error: No output CSV filepath provided." << std::endl;
            std::cout << "Enter output file path: ";
            std::cin >> input;
            try {
                replace(map, "output-csv", input);
            } catch (const std::exception &) {
                continue;
            }
        }
    }

    // if input-csv then read from a file.
    if(map.count("input-csv") && !map["input-csv"].empty()) {
        const std::string filename = map["input-csv"].as<std::string>();
        std::cout<<"Reading from file: "<<filename<<std::endl;
        try {
            // create a data map
            std::map<std::string, std::vector<long double>> dataMap;
            // fill data, mapping csv headers (keys) to csv columns (value vectors)
            readCSV(filename, dataMap);

            // read column x if provided and update the number of points
            if(dataMap.contains("x")) {
                replace(map, "x-points", dataMap["x"]);
                replace(map, "num-points", static_cast<long double>(dataMap["x"].size()));
            }
            // read column fx if provided
            if(dataMap.contains("f(x)")) {
                replace(map, "fx-points", dataMap["f(x)"]);
            }
        } catch (...) {
            std::cerr<<"..failed. Aborting."<<std::endl;
            exit(1);
        }
    }

    while (map["num-points"].empty() || failsNaturalNumberCheck(map["num-points"].as<long double>())) {
        std::cout << "Enter the number of interpolation points: ";
        std::cin >> input;
        try {
            replace(map, "num-points", asNumber(input));
        } catch (const std::exception &) {
            continue;
        }
    }

    while ((map["num-samples"].empty() || failsNaturalNumberCheck(map["num-samples"].as<long double>()))) {
        std::cout << "Enter the number of Lagrange interpolation evaluation points: ";
        std::cin >> input;
        try {
            replace(map, "num-samples", asNumber(input));
        } catch (const std::exception &) {
            continue;
        }
    }

    const auto n = static_cast<size_t>(map["num-points"].as<long double>());
    std::vector<long double> x_vec_inputs;
    bool messageShown = false;
    while (isUnfilledDoubleLongVector(map, "x-points", n)) {
        if (!messageShown) {
            std::cout << "Enter points for the interval x, sorted, and one at a time: "<<std::endl;
            messageShown = true;
        }
        std::cin >> input;
        try {
            x_vec_inputs.push_back(asNumber(input));
            replace(map, "x-points", x_vec_inputs);
        } catch (...){
            std::cerr<<"That wasn't a number, try again.\n";
            continue;
        }
    }

    messageShown = false;
    std::vector<long double> fx_vec_inputs;
    while (isUnfilledDoubleLongVector(map, "fx-points", n)) {
        if (!messageShown) {
            std::cout << "Enter points for f(x) for every x, one at a time: "<<std::endl;
            messageShown = true;
        }
        std::cin >> input;
        try {
            fx_vec_inputs.push_back(asNumber(input));
            replace(map, "fx-points", fx_vec_inputs);
        } catch (...){
            std::cerr<<"That wasn't a number, try again.\n";
            continue;
        }
    }
}

/**
 * @brief This function prints the input values.
 *
 * @param vm A boost::program_options::variables_map object containing the input values to be printed.
 *
 */
void printInputs(boost::program_options::variables_map &vm) {

    // retrieve the inputs
    const auto precision = vm["precision"].as<int>();
    const auto n  = static_cast<size_t>(vm["num-points"].as<long double>());
    const auto m  = static_cast<size_t>(vm["num-samples"].as<long double>());
    const auto x  = vm["x-points"].as<std::vector<long double>>();
    const auto fx = vm["fx-points"].as<std::vector<long double>>();

    // list the parameters
    CommandLine::printLine();
    std::cout << std::setw(44) << "Inputs\n";
    CommandLine::printLine();
    std::cout<<std::setw(16)<<"x"<<std::setw(34)<<"f(x)\n"<<std::scientific;
    for(size_t i = 0; i < x.size(); i++) {
        const auto x_sign = (x[i] >= 0) ? "+" : "";
        const auto fx_sign = (fx[i] >= 0) ? "+" : "";
        std::cout<<"\t"<<x_sign<<std::setprecision(precision)<<x[i];
        std::cout<<"\t\t\t"<<fx_sign<<std::setprecision(precision)<<fx[i]<<std::endl;
    }
    CommandLine::printLine();
    std::cout << "\tnum-points,     n: " << n << "\n";
    std::cout << "\tnum-samples, m: " << m << "\n";
    if (vm.count("input-csv") && !vm["input-csv"].empty()) {
        std::cout << "\tinput-csv,      i: " << vm["input-csv"].as<std::string>() << "\n";
    }
    std::cout << "\toutput-csv,     o: " << vm["output-csv"].as<std::string>() << "\n";
    CommandLine::printLine();
}