//
//  Utils.hpp
//  UAV_WiFi_detection
//
//  Created by Alex Shchelochkov on 14.02.2022.
//

#ifndef Utils_hpp
#define Utils_hpp

#include <string>
#include <vector>
#include <fstream>
#include <regex>
#include <sstream>
#include "LogFrame.hpp"

/**
 * Fill vector with LogFrames from file.
 *
 * @param path the file to read;
 * @param to reference to filling vector.
 */
void readFromFile(const string path, vector<LogFrame> &to);

/**
 * Convert decimal to hexadecimal number.
 *
 * @param dec decimal number.
 *
 * @return hexadecimal number equals `dec`.
 */
string decToHex(const u_int64_t dec);

/**
 * Convert hexadecimal number to MAC-address.
 *
 * @param hex hexadecimal number.
 *
 * @return string equals MAC-address constructed from `hex`.
 */
string hexToMAC(const string hex);

#endif /* Utils_hpp */
