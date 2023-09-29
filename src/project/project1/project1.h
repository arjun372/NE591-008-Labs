/**
 * @file project1.cpp
 * @author Arjun Earthperson
 * @date 09/29/2023
 * @brief This file contains the declaration for the Project1 class.
 *
 */

#ifndef NE591_008_PROJECT1_PROJECT1_H
#define NE591_008_PROJECT1_PROJECT1_H

#include <variant>
#include <numeric>
#include <iostream>
#include <iomanip>
#include <boost/program_options.hpp>

#include "InputsOutputs.h"
#include "Parser.h"

#include "Project.h"
#include "CommandLine.h"

#include "math/blas/MyBLAS.h"
#include "math/blas/Matrix.h"
#include "math/blas/LU.h"

#include "json.hpp"
#include "Compute.h"


/**
 * @class Project1
 * @brief This class is a child of the Project class and is used to solve a system of linear equations using forward and back substitution.
 * @details The class takes in command line arguments and uses them to solve the system of equations.
 */
class Project1 : public Project<SolverInputs , Parser, SolverOutputs > {

public:
    /**
     * @brief Constructor for the project1 class
     * @param args Command line arguments
     */
    explicit Project1(CommandLineArgs args) : Project(args) {}

protected:
    /**
     * @brief This function builds the header information for the project.
     * @return HeaderInfo object containing project information
     */
    HeaderInfo buildHeaderInfo() override {
        Canvas canvas;
        auto x = -0.172;
        auto y = -0.66;
        auto iterations = 2000;
        canvas.x_start = -0.007514104707;
        canvas.x_stop = 0.075446744304;
        canvas.y_start = 0.825578589953;
        canvas.y_stop = 0.883651184261;
        printJuliaSet<__float128>(canvas, x, y, iterations); //"o█■"
        std::cout<<"Julia set at ("<<x<<","<<y<<"), "<<iterations<<" iterations\n";
        return {
                .ProjectName = "NE591: Project Milestone 1",
                .ProjectDescription = "Serial Neutron Diffusion Code",
                .SubmissionDate = "09/29/2023",
                .StudentName = "Arjun Earthperson",
                .HeaderArt = " ",
        };
    }

    /**
     * @brief This function runs the project.
     * @details It solves the system of linear equations using forward and back substitution.
     * @param outputs The output vector
     * @param inputs The input matrices
     * @param values The variable map
     */
    void run(SolverOutputs &outputs, SolverInputs &inputs, boost::program_options::variables_map &values) override {

        /**
            1. Given the matrix A = LU and vector b, solve Ax = b, which is LUx = b.
            2. Perform row pivoting on A to obtain a permuted matrix PA = LU, where P is a permutation matrix.
            3. Let y = Ux. Now, we have two systems of linear equations: Ly = Pb and Ux = y.
            4. Solve the first system Ly = Pb using forward substitution.
            5. Solve the second system Ux = y using backward substitution.
        **/

//        nlohmann::json results;
//        inputs.toJSON(results["inputs"]);
//
//       // const auto A = inputs.coefficients;
//
//        auto L = MyBLAS::Matrix(A.getRows(), A.getCols(), static_cast<long double>(0));
//        auto U = MyBLAS::Matrix(A.getRows(), A.getCols(), static_cast<long double>(0));
//
//        const auto pivot = !values.count("no-pivoting");
//        const bool alternateMethod = values.count("alternate-method") != 0;
//        MyBLAS::Matrix<long double> P;
//
//        if (pivot) {
//            if (alternateMethod) {
//                P = MyBLAS::cast<long double, bool>(MyBLAS::cast<bool, long double>(dooLittleFactorizeLUP(L, U, A)));
//            } else {
//                P = MyBLAS::cast<long double, bool>(MyBLAS::cast<bool, long double>(factorizeLUwithPartialPivoting(L, U, A)));
//            }
//        } else {
//           MyBLAS::LU::factorize(L, U, A);
//        }
//
//        if(!MyBLAS::isUnitLowerTriangularMatrix(L)) {
//            std::cerr << "Warning: Factorized matrix L is not unit lower triangular, expect undefined behavior.\n";
//        }
//
//        if(!MyBLAS::isUpperTriangularMatrix(U)) {
//            std::cerr << "Warning: Factorized matrix U is not upper triangular, expect undefined behavior.\n";
//        }
//
//        if (pivot) {
//            if (!MyBLAS::isPermutationMatrix(P)) {
//                std::cerr << "Warning: Generated matrix P is not a permutation matrix, expect undefined behavior.\n";
//            }
//        }
//
//        IntermediateResults intermediates;
//        intermediates.L = L;
//        intermediates.U = U;
//        if(pivot) {
//            intermediates.P = P;
//        }
//        intermediates.toJSON(results["intermediates"]);
//
//        const auto b = inputs.constants;
//        const auto Pb = pivot ? (P * b) : b;
//
//        const MyBLAS::Vector y = MyBLAS::forwardSubstitution<long double>(L, Pb);
//        const MyBLAS::Vector x = MyBLAS::backwardSubstitution<long double>(U, y);
//
//        outputs.solution = x;
//
//        const auto b_prime = A * x;
//        const auto r = b - b_prime;
//        outputs.residual = r;
//
//        const auto maxResidual = MyBLAS::max<long double>(MyBLAS::abs(r));
//        outputs.max_residual = maxResidual;
//
//        outputs.toJSON(results["outputs"]);
//
//        if(!values.count("quiet")) {
//            const auto precision = getTerminal().getCurrentPrecision();
//
//            Parser::printLine();
//            std::cout << "Lower Triangular Matrix (L):\n";
//            Parser::printLine();
//            std::cout << std::scientific << std::setprecision(precision) << L;
//            Parser::printLine();
//            std::cout << "Upper Triangular Matrix (U):\n";
//            Parser::printLine();
//            std::cout << std::setprecision(precision) << U;
//            Parser::printLine();
//            if (pivot) {
//                std::cout << "Permutation Matrix (P):\n";
//                Parser::printLine();
//                std::cout << std::setprecision(precision) << P;
//                Parser::printLine();
//                std::cout << "Permuted constants (Pb = P * b):\n";
//                Parser::printLine();
//                std::cout << std::setprecision(precision) << Pb;
//                Parser::printLine();
//            }
//            std::cout << "Intermediate vector (y), where (Ly = "<< (pivot ? "Pb" : "b") << "):\n";
//            Parser::printLine();
//            std::cout << std::setprecision(precision) << y;
//            Parser::printLine();
//            std::cout << "Solution vector (x), where (Ux = y):\n";
//            Parser::printLine();
//            std::cout << std::setprecision(precision) << x;
//            Parser::printLine();
//            std::cout << "Residual vector (r = b - Ax) :\n";
//            Parser::printLine();
//            std::cout << std::setprecision(precision) << r;
//            Parser::printLine();
//            std::cout << "Max Residual abs(r): ";
//            std::cout << std::setprecision(max_precision) << maxResidual << std::endl;
//            Parser::printLine();
//        }
//
//        writeJSON(values["output-json"].as<std::string>(), results);
    }

};

#endif // NE591_008_PROJECT1_PROJECT1_H
