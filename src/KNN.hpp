//
//  KNN.hpp
//  UAV_WiFi_detection
//
//  Created by Alex Shchelochkov on 25.05.2022.
//

#ifndef KNN_hpp
#define KNN_hpp

#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <functional>
#include <tuple>

using namespace std;
 
const float PI = 3.14159265358979323846264338327950288;
const float EPS = 1e-8;

float dist(const vector<int> &a, const vector <int> &b, string distType);

float kernel(float x, const string kernelType);

float calcFscore(const vector<vector<u_int64_t>>& cm);

float nadarayWatson(const vector<vector<float>>& data,
                    const vector<vector<u_int32_t>> targets,
                    float h,
                    const vector<float>& obj,
                    function<float(vector<float>&, vector<float>&)> dist,
                    function<float(float)> kernel);

vector<vector<u_int64_t>> regression(const vector<vector<float>>& data,
                                    const vector<vector<u_int32_t>> targets,
                                    const function<float(vector<float>&, vector<float>&)> dist,
                                    function<float(float)> kernel,
                                    const string windType,
                                    const u_int64_t wind);

void normalize(vector<vector<float>> &df);
 
vector<vector<u_int32_t>> oneHotEncoding(const vector<u_int32_t> &classes, const u_int32_t classCnt);

class LeaveOneOut {
private:
    const vector<string> windowTypes {"variable", "fixed"};
    const vector<string> distTypes {"euclidean", "manhattan", "chebyshev"};
    const vector<string> kernelTypes {"uniform", "triangular", "epanechnikov", "quartic", "triweight", "tricube", "gaussian", "cosine", "logistic", "sigmoid"};
    vector<vector<float>> data;
    vector<vector<uint32_t>> targets;
    float bestFscore, bestWindowWidth;
    string bestDistType;
    string bestKernelType;
    string bestWindowType;
public:
    void fit(const vector<vector<float>>& data, const vector<vector<u_int32_t>>& targets);
    u_int32_t predict(const vector<float>& query);
};


#endif /* KNN_hpp */
