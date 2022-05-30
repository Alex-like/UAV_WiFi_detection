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
#include <set>
#include <map>
#include <fstream>
#include <regex>
#include <sstream>
#include <tuple>
#include "LogFrame.hpp"

/**
 * Fill vector with LogFrames from file.
 *
 * @param path the file to read;
 * @param to reference to filling vector.
 */
void readFromFile(const string path, vector<LogFrame> &to);
void printToFile(const string path, map<u_int64_t, vector<float>> &fom);
tuple<vector<uint64_t>,vector<vector<float>>, vector<uint32_t>> readDataForKNNFromFile(const string path);
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
/**
 * Print all MAC-addresses from file.
 *
 * @param path file path.
 */
void getAddressesFromFile(string path);
/**
 * Fill `macs` with MAC-addresses from string.
 *
 * @param str source srting.
 * @param macs set which we want to fill.
 */
void getAddressesFromStr(string str, set<string> &macs);
/**
 * Convert MAC-address to hex number.
 *
 * @param mac string with MAC-address.
 *
 * @return hexadecimal value of input MAC-address.
 */
u_int64_t macToHex(string mac);
/**
 * Convert all symbols in `s` to lower case.
 *
 * @param s source string.
 *
 * @return transformed string.
 */
string str_tolower(string s);
/**
 * Check existing of string by key.
 *
 * @param key specific key.
 * @param dict sorted group of pairs with unique keys and values.
 *
 * @return "True" if `dict` contains `key`else return "False".
 */
bool checkExist(u_int64_t key, map<u_int64_t, string> &dict);
/**
 * Filter vector of LogFrames by using definite predicate.
 *
 * @param vec reference to vector with LogFrames.
 * @param predicate boolean function for filtering.
 *
 * @return vector of LogFrames which was accepted.
 */
vector<LogFrame> filter(const vector<LogFrame>& vec, function<bool(LogFrame)> predicate);

string toString(const vector<u_int64_t> &vec);
string toString(const vector<float> &vec);
float fpow(float base, int exp);

#endif /* Utils_hpp */
