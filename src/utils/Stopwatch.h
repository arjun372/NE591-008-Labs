/**
 * @file Stopwatch.h
 * @author Arjun Earthperson
 * @date 08/30/2023
 * @brief This file contains the Stopwatch class which is a utility for measuring time durations in various units.
*/

/*
 * MIT License
 *
 * Copyright (c) 2023 Luigi Capogrosso, Luca Geretti,
 *                    Marco cristani, Franco Fummi, and Tiziano Villa.
 *
 * Copyright (c) 2023 Arjun Earthperson
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */


#ifndef NE591_008_STOPWATCH_H
#define NE591_008_STOPWATCH_H


#include <chrono>

// Define time units for convenience
using Seconds = std::chrono::seconds;
using Milliseconds = std::chrono::milliseconds;
using Microseconds = std::chrono::microseconds;
using Nanoseconds = std::chrono::nanoseconds;

/**
 * @brief Stopwatch class template for measuring time durations.
 * @tparam D The duration type (e.g., Seconds, Milliseconds, Microseconds).
 */
template<class D> class Stopwatch
{
public:
    // Define types for high resolution clock and time point
    using ResolutionType = std::chrono::high_resolution_clock;
    using TimePointType = std::chrono::time_point<ResolutionType>;

    /**
     * @brief Constructor that also restarts the stopwatch.
     */
    Stopwatch() { restart(); }

    /**
     * @brief Get the duration in the given type.
     * @return The duration from the initial time point to the last clicked time point, cast to the template duration
     * type.
     */
    D duration() const { return std::chrono::duration_cast<D>(_clicked-_initial); }

    /**
     * @brief Get the duration in seconds, in double precision.
     * @return The duration from the initial time point to the last clicked time point, cast to double precision
     * seconds.
     */
    [[nodiscard]] double elapsed_seconds() const { return std::chrono::duration_cast<std::chrono::duration<double>>(duration()).count(); }

    /**
     * @brief Restart the stopwatch by setting the initial and clicked time points to the current time.
     * @return A reference to the stopwatch object, allowing for method chaining.
     */
    Stopwatch& restart() { _initial = ResolutionType::now(); _clicked = _initial; return *this; }

    /**
     * @brief Save the current time as the clicked time point.
     * @return A reference to the stopwatch object, allowing for method chaining.
     */
    Stopwatch& click() { _clicked = ResolutionType::now(); return *this; }

private:
    TimePointType _initial; ///< The initial time point when the stopwatch was last restarted.
    TimePointType _clicked; ///< The last clicked time point, used to measure durations.
};

#endif //NE591_008_STOPWATCH_H
