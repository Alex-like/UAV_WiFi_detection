//
//  KNN.cpp
//  UAV_WiFi_detection
//
//  Created by Alex Shchelochkov on 25.05.2022.
//

#include "KNN.hpp"

using namespace std;

float dist(const vector<float> &a, const vector <float> &b, const string distType) {
    float dist = 0;
    if (distType == "euclidean") {
        for (int i = 0; i < a.size(); i++)
            dist += (a[i] - b[i]) * (a[i] - b[i]);
        dist = sqrt(dist);
    }
    if (distType == "manhattan")
        for (int i = 0; i < a.size(); i++)
            dist += abs(a[i] - b[i]);
    if (distType == "chebyshev")
        for (int i = 0; i < a.size(); i++)
            dist = max(dist, float(abs(a[i] - b[i])));
    return dist;
}

float kernel(float x, const string kernelType) {
    if (kernelType == "uniform")
        return abs(x) < 1 ? 0.5 : 0;
    if (kernelType == "triangular")
        return abs(x) < 1 ? 1 - x : 0;
    if (kernelType == "epanechnikov")
        return abs(x) < 1 ? 0.75 * (1 - x * x) : 0;
    if (kernelType == "quartic")
        return abs(x) < 1 ? (15.0 / 16.0) * (1.0 - (x * x)) * (1.0 - (x * x)) : 0;
    if (kernelType == "triweight")
        return abs(x) < 1 ? (35.0 / 32.0) * (1.0 - (x * x)) * (1.0 - (x * x)) * (1.0 - (x * x)) : 0;
    if (kernelType == "tricube")
        return abs(x) < 1 ? (70.0 / 81.0) * (1.0 - (x * x * x)) * (1.0 - (x * x * x)) * (1.0 - (x * x * x)) : 0;
    if (kernelType == "gaussian")
        return exp((-0.5) * x * x) / (sqrt(2 * PI));
    if (kernelType == "cosine")
        return abs(x) < 1 ? (PI / 4.0) * cos(PI * x / 2.0) : 0;
    if (kernelType == "logistic")
        return 1.0 / (exp(x) + 2.0 + exp(-x));
    if (kernelType == "sigmoid")
        return (2.0 / PI) / (exp(x) + exp(-x));
    return 0;
}

float calcFscore(const vector<vector<u_int64_t>>& cm) {
    u_int64_t all = 0;
    vector<u_int64_t> p(cm.size(), 0), c(cm.size(), 0);
    for (int i = 0; i < cm.size(); i++)
        for (int j = 0; j < cm.size(); j++) {
            all += cm[i][j];
            p[i] += cm[i][j];
            c[j] += cm[i][j];
        }
    float fscore = 0;
    for (int i = 0; i < cm.size(); i++) {
        float tp = cm[i][i];
        float fp = c[i] - cm[i][i];
        float fn = p[i] - cm[i][i];
        float prec = tp + fp != 0 ? float(tp) / (tp + fp) : 0;
        float recl = tp + fn != 0 ? float(tp) / (tp + fn) : 0;
        if (prec + recl != 0)
            fscore += p[i] * (2 * prec * recl) / (prec + recl);
    }
    return fscore / all;
}

float nadarayWatson(const vector<vector<float>>& data,
                    const vector<u_int32_t>& targets,
                    const u_int64_t h,
                    const vector<float>& query,
                    function<float(const vector<float>&, const vector<float>&)> distF,
                    function<float(float)> kernelF,
                    const string windType) {
    vector<pair<float, u_int64_t>> dists;
    for (u_int64_t i = 0; i < data.size(); i++)
        dists.emplace_back(distF(query, data[i]), i);
    sort(dists.begin(), dists.end(), [](auto a, auto b){ return a.first < b.first; });
    float windWidth = windType == "fixed" ? float(h) : dists[h].first;
    if (abs(windWidth) < EPS) {
        float sum = 0;
        u_int64_t cnt = 0;
        if (data[dists[0].second] == query) {
            for (u_int64_t i = 0; i < data.size(); i++)
                if (data[i] == query) {
                    sum += targets[i];
                    cnt++;
                }
        } else {
            for (const auto &t : targets)
                sum += t;
            cnt = data.size();
        }
        return sum / cnt;
    }
    float kernel_sum_with_classes = 0, kernel_sum = 0;
    for (const auto &p : dists) {
        float kernel_v = kernelF(p.first / windWidth);
        kernel_sum_with_classes += kernel_v * targets[p.second];
        kernel_sum += kernel_v;
    }
    if (isnan(kernel_sum_with_classes) || isnan(kernel_sum) || kernel_sum < EPS) {
        float sum = 0;
        for (const auto &t : targets)
            sum += t;
        return sum / data.size();
    }
    return kernel_sum_with_classes / kernel_sum;
}

vector<vector<u_int64_t>> regression(const vector<vector<float>>& data,
                                    const vector<u_int32_t>& targets,
                                    function<float(const vector<float>&, const vector<float>&)> distF,
                                    function<float(float)> kernelF,
                                    const string windType,
                                    const u_int64_t wind) {
    u_int32_t targets_cnt = *max_element(targets.begin(), targets.end()) + 1;
    vector<vector<u_int64_t>> CM(targets_cnt, vector<u_int64_t>(targets_cnt, 0));
    for (int i = 0; i < data.size(); i++) {
        vector<vector<float>> data_train(data);
        vector<float> data_test = data_train[i];
        data_train.erase(data_train.begin() + i);
        vector<u_int32_t> targets_train(targets);
        u_int32_t real = targets_train[i];
        targets_train.erase(targets_train.begin() + i);
        u_int32_t pred = round(nadarayWatson(data_train, targets_train, wind, data_test, distF, kernelF, windType));
        CM[pred][real]++;
    }
    return CM;
}

void normalize(vector<vector<float>> &df) {
    if (df.empty())
        return;
    if (df[0].empty())
        return;
    for (int j = 0; j < df[0].size(); j++) {
        float min_j = MAXFLOAT, max_j = MAXFLOAT + 1;
        for (u_int64_t i = 0; i < df.size(); i++) {
            min_j = min(min_j, df[i][j]);
            max_j = max(max_j, df[i][j]);
        }
        for (u_int64_t i = 0; i < df.size(); i++)
            df[i][j] = (df[i][j] - min_j) / (max_j - min_j);
    }
}

vector<vector<u_int32_t>> oneHotEncoding(const vector<u_int32_t> &classes, const u_int32_t classCnt) {
    vector<vector<u_int32_t>> result(classCnt, vector<u_int32_t>(classes.size(), 0));
    for (u_int64_t i = 0; i < classes.size(); i++)
        result[classes[i] - 1][i] = 1;
    return result;
}

u_int32_t predictWithoutFit(const vector<vector<float>> &data,
                            const vector<uint32_t> &targets,
                            const vector<float> &query,
                            const string bestDistType,
                            const string bestKernelType,
                            const string bestWindowType,
                            const u_int64_t bestWindowWidth) {
    function<float(const vector<float>&, const vector<float>&)> distF = [dt = bestDistType](auto a, auto b) { return dist(a, b, dt); };
    function<float(float)> kernelF = [kt = bestKernelType](auto x) { return kernel(x, kt); };
    return round(nadarayWatson(data, targets, bestWindowWidth, query, distF, kernelF, bestWindowType));;
}


void LeaveOneOut::fit(const vector<vector<float>> &data, const vector<u_int32_t> &targets) {
    this->data = data;
    this->targets = targets;
    bestFscore = 0.0;
    float sqrtD = sqrt(data.size());
    vector<tuple<string, string, string>> combs;
    for (const string &w : windowTypes)
        for (const string &d : distTypes)
            for (const string &k : kernelTypes)
                combs.emplace_back(w, d, k);
    for (const auto &[windType, distType, kernelType] : combs) {
        vector<u_int64_t> windows;
        if (windType == "fixed") {
            float R_D = 0.0;
            for (const auto &a : data)
                for (const auto &b : data) {
                    float d = dist(a, b, distType);
                    R_D = max(R_D, d);
                }
            float R_D_div = R_D / sqrtD;
            for (u_int64_t i = 1; i < int(ceil(sqrtD)); i++)
                windows.emplace_back(i * u_int64_t(ceil(R_D_div)));
        } else
            for (u_int64_t i = 2; i < int(ceil(sqrtD)); i++)
                windows.emplace_back(i);
        for (const u_int64_t wind : windows) {
            function<float(const vector<float>&, const vector<float>&)> distF = [dt = distType](auto a, auto b) { return dist(a, b, dt); };
            function<float(float)> kernelF = [kt = kernelType](auto x) { return kernel(x, kt); };
            float fscore = calcFscore(regression(data,
                                                 targets,
                                                 distF,
                                                 kernelF,
                                                 windType,
                                                 wind));
            if (fscore > bestFscore) {
                bestFscore = fscore;
                bestDistType = distType;
                bestKernelType = kernelType;
                bestWindowType = windType;
                bestWindowWidth = wind;
            }
        }
    }
    cout << "---- Results: ----" << '\n';
    cout << "Distance: " << bestDistType << '\n';
    cout << "Kernel: " << bestKernelType << '\n';
    cout << "Window Type: " << bestWindowType << '\n';
    if (bestWindowType == "variable")
        cout << "Neibhours: " << bestWindowWidth << '\n';
    else
        cout << "Window Width: " << bestWindowWidth << '\n';
    cout << "max F score: " << bestFscore << '\n';
}

u_int32_t LeaveOneOut::predict(const vector<float>& query) {
    return predictWithoutFit(data, targets, query, bestDistType, bestKernelType, bestWindowType, bestWindowWidth);
}
