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

float calcFscore(const vector<vector<uint64_t>>& cm) {
    uint64_t all = 0;
    vector<uint64_t> p(cm.size(), 0), c(cm.size(), 0);
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
                    const vector<uint32_t>& targets,
                    float h,
                    const vector<float>& query,
                    function<float(const vector<float>&, const vector<float>&)> dist,
                    function<float(float)> kernel) {
    vector<pair<float, uint64_t>> dists;
    for (uint64_t i = 0; i < data.size(); i++) {
        float d = dist(query, data[i]);
        dists.emplace_back(d, i);
    }
    if (abs(h) < EPS) {
        float sum = 0;
        uint64_t cnt = 0;
        if (data[dists[0].first] == query) {
            for (uint64_t i = 0; i < data.size(); i++)
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
        float kernel_v = kernel(p.first / h);
        kernel_sum_with_classes += kernel_v * targets[p.first];
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

vector<vector<uint64_t>> regression(const vector<vector<float>>& data,
                                    const vector<vector<uint32_t>>& targets,
                                    function<float(const vector<float>&, const vector<float>&)> distF,
                                    function<float(float)> kernelF,
                                    const string windType,
                                    const uint64_t wind) {
    vector<vector<uint64_t>> CM(targets.size(), vector<uint64_t>(targets.size(), 0));
    for (int i = 0; i < data.size(); i++) {
        vector<vector<float>> data_train(data);
        vector<float> data_test = data_train[i];
        data_train.erase(data_train.begin() + i);
        vector<float> predictions;
        for (int j = 0; j < targets.size(); j++) {
            vector<uint32_t> targets_train = targets[j];
            targets_train.erase(targets_train.begin() + i);
            float h;
            if (windType == "fixed")
                h = wind;
            else {
                vector<pair<float, int>> dists;
                for (int k = 0; k < targets_train.size(); k++)
                    dists.emplace_back(distF(data_test, data_train[k]), k);
                sort(dists.begin(), dists.end(), [](auto a, auto b){ return a.first < b.first; });
                h = dists[wind].first;
            }
            
            float pred = nadarayWatson(data_train, targets_train, h, data_test, distF, kernelF);
            predictions.push_back(pred);
        }
        uint32_t max = 0;
        uint64_t real = 0;
        for (int k = 0; k < targets.size(); k++)
            if (targets[k][i] > max) {
                max = targets[k][i];
                real = k;
            }
        float tmp_max = 0.0;
        uint64_t pred = 0;
        for (int k = 0; k < predictions.size(); k++)
            if (predictions[k] > tmp_max) {
                tmp_max = predictions[k];
                pred = k;
            }
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
        for (uint64_t i = 0; i < df.size(); i++) {
            min_j = min(min_j, df[i][j]);
            max_j = max(max_j, df[i][j]);
        }
        for (uint64_t i = 0; i < df.size(); i++)
            df[i][j] = (df[i][j] - min_j) / (max_j - min_j);
    }
}

vector<vector<uint32_t>> oneHotEncoding(const vector<uint32_t> &classes, const uint32_t classCnt) {
    vector<vector<uint32_t>> result(classCnt, vector<uint32_t>(classes.size(), 0));
    for (uint64_t i = 0; i < classes.size(); i++)
        result[classes[i] - 1][i] = 1;
    return result;
}

void LeaveOneOut::fit(const vector<vector<float>> &data, const vector<vector<uint32_t>> &targets) {
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
        vector<uint64_t> windows;
        if (windType == "fixed") {
            float R_D = 0.0;
            for (const auto &a : data)
                for (const auto &b : data) {
                    float d = dist(a, b, windType);
                    R_D = max(R_D, d);
                }
            float R_D_div = R_D / sqrtD;
            for (uint64_t i = 1; i < int(ceil(sqrtD)); i++)
                windows.emplace_back(i * uint64_t(ceil(R_D_div)));
        } else
            for (uint64_t i = 1; i < int(ceil(sqrtD)); i++)
                windows.emplace_back(i);
        for (const uint64_t wind : windows) {
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
                bestWindowWidth = wind;
                bestWindowType = windType;
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

uint32_t LeaveOneOut::predict(const vector<float>& query) {
    function<float(const vector<float>&, const vector<float>&)> distF = [dt = bestDistType](auto a, auto b) { return dist(a, b, dt); };
    function<float(float)> kernelF = [kt = bestKernelType](auto x) { return kernel(x, kt); };
    vector<float> predictions;
    for (int j = 0; j < targets.size(); j++) {
        vector<uint32_t> targets_train = targets[j];
        float h;
        if (bestWindowType == "fixed")
            h = bestWindowWidth;
        else {
            vector<pair<float, int>> dists;
            for (int k = 0; k < targets_train.size(); k++)
                dists.emplace_back(distF(query, data[k]), k);
            sort(dists.begin(), dists.end(), [](auto a, auto b){ return a.first < b.first; });
            h = dists[bestWindowWidth].first;
        }
        
        float pred = nadarayWatson(data, targets_train, h, query, distF, kernelF);
        predictions.push_back(pred);
    }
    float max = 0.0;
    uint32_t pred = 0;
    for (int k = 0; k < predictions.size(); k++)
        if (predictions[k] > max) {
            max = predictions[k];
            pred = k;
        }
    return pred + 1;
}
