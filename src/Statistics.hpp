//
//  Statistics.hpp
//  UAV_WiFi_detection
//
//  Created by Alex Shchelochkov on 14.02.2022.
//

#ifndef Statistics_hpp
#define Statistics_hpp

#include <vector>
#include <string>
#include <sstream>
#include "LogFrame.hpp"

using namespace std;

/**
 * Get statistic of correct frames' number.
 *
 * Contains information about correctness of read frames.
 */
class Statistics {
private:
    /// Numer of correct frames.
    u_int64_t correct;
    /// Number of incorrect frames.
    u_int64_t incorrect;
    /// Total number of frames.
    u_int64_t whole;
public:
    /**
     * Basic constructor.
     *
     * @param whole_v total number of frames.
     * @param correct_v numer of correct frames.
     * @param incorrect_v number of incorrect frames.
     */
    Statistics(u_int64_t whole_v, u_int64_t correct_v, u_int64_t incorrect_v);
    /**
     * Alternative constructor.
     *
     * @param frames reference to vector of read frames.
     */
    Statistics(const vector<LogFrame> &frames);
    /**
     * Convert statistic to string value.
     *
     * @return string representation of statistic.
     */
    string toString();
};

#endif /* Statistics_hpp */
