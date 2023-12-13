#ifndef NE591_008_LINEARSOLVERPARAMS_H
#define NE591_008_LINEARSOLVERPARAMS_H

/**
* @file LinearSolverParams.h
* @brief This file contains the definition of the LinearSolverParams struct which encapsulates
* the parameters required for a solver with templated matrix and vector types.
*
* The LinearSolverParams struct is a template structure that holds parameters necessary for
* solving problems with templated matrix and vector types. It includes parameters such as the size
* of the matrix/vector, the convergence threshold, the relaxation factor, the maximum number of
* iterations, the coefficient matrix, and the vector of constants.
*
* The LinearSolverParams struct also provides methods to set and get these parameters, and
* includes default values for the parameters.
*
* This file is part of the MyBLAS::Solver namespace, which encapsulates functionality related to solving
* linear algebra problems.
*
* @author Arjun Earthperson
* @date 12/01/2023
 */
#include <cmath>
#include <cstddef>
#include <limits>

#include "matrix/LazyMatrix.h"
#include "matrix/Matrix.h"
#include "vector/LazyVector.h"
#include "vector/Vector.h"

/**
 * @brief Namespace for the Solver module
 * This namespace encapsulates functionality related to solving linear algebra problems.
 */
namespace MyBLAS::Solver {

/**
 * @brief Template struct to hold parameters for a solver with templated matrix and vector types.
 *
 * This struct contains parameters necessary for solving problems with templated matrix and vector types.
 *
 * @tparam MatrixType The matrix type used for storing coefficients.
 * @tparam VectorType The vector type used for storing constants.
 * @tparam T The data type used for storing parameters.
 */
template <template <typename> class MatrixType, template <typename> class VectorType, typename T>
struct TemplatedParameters {

  public:
    /**
     * @brief Size of the matrix/vector.
     *
     * This value represents the size of the matrix/vector.
     */
    size_t n = std::numeric_limits<size_t>::quiet_NaN();

    /**
     * @brief The convergence threshold.
     *
     * This value represents the convergence threshold for the solver.
     */
    T threshold = std::numeric_limits<T>::quiet_NaN();

    /**
     * @brief The relaxation factor.
     *
     * This value represents the relaxation factor for the solver.
     */
    T relaxation_factor = std::numeric_limits<T>::quiet_NaN();

    /**
     * @brief Maximum number of iterations to perform.
     *
     * This value represents the maximum number of iterations to perform.
     */
    size_t max_iterations = std::numeric_limits<size_t>::quiet_NaN();

    /**
     * @brief Coefficient matrix A.
     *
     * This matrix holds the coefficients for the problem.
     */
    MatrixType<T> coefficients;

    /**
     * @brief Vector of constants b.
     *
     * This vector holds the constants for the problem.
     */
    VectorType<T> constants;

    /**
     * @brief Default constructor for TemplatedParameters.
     */
    TemplatedParameters() = default;

    /**
     * @brief Constructor with initialization of parameters.
     *
     * @param n The size of the matrix/vector.
     * @param threshold The convergence threshold.
     * @param relaxation_factor The relaxation factor.
     * @param max_iterations The maximum number of iterations to perform.
     * @param coefficients The coefficient matrix.
     * @param constants The vector of constants.
     */
    TemplatedParameters(size_t _n, T _threshold, T _relaxation_factor, size_t _max_iterations,
                        const MatrixType<T>& _coefficients, const VectorType<T>& _constants)
        : n(_n), threshold(_threshold), relaxation_factor(_relaxation_factor),
          max_iterations(_max_iterations), coefficients(_coefficients), constants(_constants) {}

    /**
     * @brief Set the size of the matrix/vector.
     *
     * @param n The size of the matrix/vector.
     * @return A reference to the TemplatedParameters struct.
     *
     * @throw std::invalid_argument if the provided value is zero.
     */
    TemplatedParameters& setSize(size_t _n) {
        if (_n == 0) {
            throw std::invalid_argument("Invalid value for n. It should be a positive number.");
        }
        n = _n;
        return *this;
    }

    /**
     * @brief Set the convergence threshold.
     *
     * @param threshold The convergence threshold.
     * @return A reference to the TemplatedParameters struct.
     *
     * @throw std::invalid_argument if the provided value is non-positive or not finite.
     */
    TemplatedParameters& setThreshold(T _threshold) {
        if (_threshold <= 0 || !std::isfinite(_threshold)) {
            throw std::invalid_argument("Invalid value for threshold. It should be a positive finite number.");
        }
        threshold = _threshold;
        return *this;
    }

    /**
     * @brief Set the relaxation factor.
     *
     * @param relaxation_factor The relaxation factor.
     * @return A reference to the TemplatedParameters struct.
     *
     * @throw std::invalid_argument if the provided value is non-positive or not finite.
     */
    TemplatedParameters& setRelaxationFactor(T _relaxation_factor) {
        if (_relaxation_factor <= 0 || !std::isfinite(_relaxation_factor)) {
            throw std::invalid_argument("Invalid value for relaxation factor. It should be a positive finite number.");
        }
        relaxation_factor = _relaxation_factor;
        return *this;
    }

    /**
     * @brief Set the maximum number of iterations to perform.
     *
     * @param max_iterations The maximum number of iterations to perform.
     * @return A reference to the TemplatedParameters struct.
     *
     * @throw std::invalid_argument if the provided value is zero.
     */
    TemplatedParameters& setMaxIterations(size_t _max_iterations) {
        if (_max_iterations == 0) {
            throw std::invalid_argument("Invalid value for max_iterations. It should be a positive number.");
        }
        max_iterations = _max_iterations;
        return *this;
    }

    /**
     * @brief Set the coefficient matrix.
     *
     * @param coefficients The coefficient matrix.
     * @return A reference to the TemplatedParameters struct.
     *
     * @throw std::invalid_argument if the dimensions of the coefficients matrix do not match with the size.
     */
    TemplatedParameters& setCoefficients(const MatrixType<T>& _coefficients) {
        if (_coefficients.getRows() != n || _coefficients.getCols() != n) {
            throw std::invalid_argument("Invalid dimensions for the coefficients matrix. It should be square and match with the size.");
        }
        coefficients = _coefficients;
        return *this;
    }

    /**
     * @brief Set the vector of constants.
     *
     * @param constants The vector of constants.
     * @return A reference to the TemplatedParameters struct.
     *
     * @throw std::invalid_argument if the size of the constants vector does not match with the size.
     */
    TemplatedParameters& setConstants(const VectorType<T>& _constants) {
        if (_constants.getSize() != n) {
            throw std::invalid_argument("Invalid size for the constants vector. It should match with the size.");
        }
        constants = _constants;
        return *this;
    }

    /**
     * @brief Get the size of the matrix/vector.
     *
     * @return The size of the matrix/vector.
     */
    [[nodiscard]] size_t getSize() const { return n; }

    /**
     * @brief Get the convergence threshold.
     *
     * @return The convergence threshold.
     */
    [[nodiscard]] T getThreshold() const { return threshold; }

    /**
     * @brief Get the relaxation factor.
     *
     * @return The relaxation factor.
     */
    [[nodiscard]] T getRelaxationFactor() const { return relaxation_factor; }

    /**
     * @brief Get the maximum number of iterations to perform.
     *
     * @return The maximum number of iterations to perform.
     */
    [[nodiscard]] size_t getMaxIterations() const { return max_iterations; }

    /**
     * @brief Get the coefficient matrix.
     *
     * @return The coefficient matrix.
     */
    const MatrixType<T>& getCoefficients() const { return coefficients; }

    /**
     * @brief Get the vector of constants.
     *
     * @return The vector of constants.
     */
    const VectorType<T>& getConstants() const { return constants; }
};

/**
 * @brief Structure that represents the input to the linear solver.
 * @tparam T The type of the elements in the matrix/vector.
 */
template <typename T>
struct Parameters : public TemplatedParameters<MyBLAS::Matrix, MyBLAS::Vector, T> {};

/**
 * @brief Structure that represents the input to the linear solver.
 * @tparam T The type of the elements in the matrix/vector.
 */
template <typename T>
struct LazyParameters : public TemplatedParameters<MyBLAS::LazyMatrix, MyBLAS::LazyVector, T> {};

}  // namespace MyBLAS

#endif // NE591_008_LINEARSOLVERPARAMS_H