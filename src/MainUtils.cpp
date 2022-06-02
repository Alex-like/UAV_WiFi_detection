//
//  MainUtils.cpp
//  UAV_WiFi_detection
//
//  Created by Alex Shchelochkov on 25.04.2022.
//

#include "MainUtils.hpp"

using namespace std;

void getStatistics(vector<LogFrame> &frames) {
    Statistics stat(frames);
    cout << stat.toString() << '\n';
}

void countFrameTypes(vector<LogFrame> &frames) {
    map<string, u_int64_t> types;
    for (LogFrame frame : frames) {
        if (frame.getType().has_value()) {
            if (types.find(frame.getType().value()) == types.end()) {
                types[frame.getType().value()] = 0;
            }
            types[frame.getType().value()]++;
        }
    }
    for (auto &type : types) {
        cout << type.first << " : " << type.second << '\n';
    }
}

void makeSimpleGraph(vector<LogFrame> &frames) {
    Graph graph = Graph();
    for (LogFrame frame : frames) {
        graph.addEdge(frame.getTA(), frame.getRA());
    }
    cout << graph.toString() << "\n";
}

void getNetworks(vector<LogFrame> &frames) {
    GroupedGraph graph = GroupedGraph();
    for (LogFrame frame : frames) {
        if (frame.getType() == "Management/Beacon") {
            if (frame.getSSID().has_value() && frame.getTA().has_value()) {
                graph.addGroup(frame.getSSID().value(), frame.getTA().value());
            }
        }
        if (frame.getTA().has_value() && frame.getRA().has_value()) {
            graph.addEdge(frame.getTA().value(), frame.getRA().value());
        }
    }
    cout << graph.toString() << '\n';
}

void getAddressesFromFrames(vector<LogFrame> &frames) {
    set<string> macs;
    for (LogFrame frame : frames) {
        getAddressesFromStr(frame.getData(), macs);
    }
    for (string mac : macs) {
        cout << mac << '\n';
    }
    cout << '\n';
}

void getSSIDFromFrames(vector<LogFrame> &frames) {
    set<string> ssids;
    for (LogFrame frame : frames) {
        optional<string> ssid = frame.getSSID();
        if (ssid.has_value()) {
            ssids.insert(ssid.value());
        }
    }
    for (string ssid : ssids) {
        cout << ssid << '\n';
    }
    cout << '\n';
}

void classifyAllMacFromFrames(vector<LogFrame> &frames) {
    GroupedGraph graph = GroupedGraph();
    for (LogFrame frame : frames) {
        if (frame.getType() == "Management/Beacon") {
            if (frame.getSSID().has_value() && frame.getTA().has_value()) {
                graph.addGroup(frame.getSSID().value(), frame.getTA().value());
            }
        }
        if (frame.getTA().has_value() && frame.getRA().has_value()) {
            graph.addEdge(frame.getTA().value(), frame.getRA().value());
        }
    }
    cout << graph.toString() << '\n';
    
    map<u_int64_t, string> macAndSSID;
    for (LogFrame frame : frames) {
        if (frame.getTA().has_value() && !checkExist(frame.getTA().value(), macAndSSID)) {
            string ssid = "";
            if (frame.getSSID().has_value()) {
                ssid = frame.getSSID().value();
            }
            macAndSSID[frame.getTA().value()] = ssid;
        }
    }
    
    set<string> macs;
    for (LogFrame frame : frames) {
        getAddressesFromStr(frame.getData(), macs);
    }
    
    Classifier classifier = Classifier(graph);
    vector<Object> objs;
    for (string mac : macs) {
        u_int64_t mac_v = macToHex(mac);
        optional<Object> obj = classifier.classify(mac_v, macAndSSID[mac_v]);
        if (obj.has_value()) {
            objs.push_back(obj.value());
        }
    }
    for (Object obj : objs) {
        cout << obj.toString() << '\n';
    }
}

void printFramesTypes(vector<LogFrame> &frames) {
    map<u_int64_t, map<u_int64_t, vector<string>>> types;
    for (LogFrame frame : frames) {
        optional<u_int64_t> tmp_num = frame.getTA();
        if (!tmp_num.has_value())
            continue;
        u_int64_t TA = tmp_num.value();
        tmp_num = frame.getRA();
        if (!tmp_num.has_value())
            continue;
        u_int64_t RA = tmp_num.value();
        optional<string> tmp_str = frame.getType();
        if (!tmp_str.has_value())
            continue;
        string type = tmp_str.value();
        types[TA][RA].emplace_back(type);
    }
    for (auto &p1 : types)
        for (auto &p2 : p1.second) {
            cout << p1.first << " -> " << p2.first << " : ";
            for (string type : p2.second)
                cout << type << ' ';
            cout << '\n';
        }
    
}

Packet::Packet(u_int64_t id, u_int64_t size_v, float time, vector<pair<u_int64_t, u_int64_t>> frags) {
    number = id;
    size = size_v;
    arrivalTime = time;
    fragments = frags;
}

void Packet::addFragment(LogFrame* frame) {
    if (number != frame->getSeqNum() || fragments.size() - 1 == frame->getFragNum())
        return;
    fragments.emplace_back(frame->getSize(), frame->getOffset());
    size += frame->getSize();
    arrivalTime = frame->getOffset();
}

u_int64_t Packet::getID() {
    return number;
}

u_int64_t Packet::getSize() {
    return size;
}

float Packet::getArrivalTime() {
    return arrivalTime;
}

float calcMedian(vector<float> xs, function<bool(float, float)> cmp) {
    sort(xs.begin(), xs.end(), cmp);
    return xs.size() % 2 == 0 ? xs[xs.size()/ 2 + 1] : xs[(xs.size() + 1)/ 2];
}

StandardFeatures::StandardFeatures(vector<float> xs) {
    float n = xs.size();
    // min, max
    min = *min_element(xs.begin(), xs.end());
    max = *max_element(xs.begin(), xs.end());
    // median, medianAD
    median = calcMedian(xs, [](float a, float b) { return a < b; });
    medianAD = abs(calcMedian(xs, [m = median](float a, float b) { return abs(a - m) < abs(b - m); }) - median);
    // mean
    mean = accumulate(xs.begin(), xs.end(), 0.0) / n;
    // variance
    variance = accumulate(xs.begin(), xs.end(), 0.0, [m = mean](float acc, float x) { return acc + (x - m) * (x - m); }) / (xs.size() - 1);
    // standart deviation
    standartDeviation = sqrt(variance);
    // m_square
    m_square = accumulate(xs.begin(), xs.end(), 0.0, [](float acc, float x) { return acc + x * x; }) / n;
    // root mean square
    rootMeanSquare = sqrt(m_square);
    // p_skewness
    p_skewness = 3.0 * (mean - median) / variance;
    // kurtosys
    kurtosys = accumulate(xs.begin(), xs.end(), 0.0, [m = mean, d = variance](float acc, float x) { return acc + fpow((x - m) / d, 4); }) / n;
    // skewness
    skewness = accumulate(xs.begin(), xs.end(), 0.0, [m = mean, d = variance](float acc, float x) { return acc + fpow((x - m) / d, 3); }) / n;
}

string StandardFeatures::toString() {
    stringstream ss;
    ss.precision(5);
    ss << standartDeviation << ' ' << variance << ' ' << rootMeanSquare << ' ' << m_square << ' ' << p_skewness << ' ' << kurtosys << ' ' << skewness << ' ' << min << ' ' << max << ' ' << mean << ' ' << median << ' ' << medianAD;
    return ss.str();
}

vector<float> StandardFeatures::toVector() {
    return {
        standartDeviation, variance, rootMeanSquare,
        m_square, p_skewness, kurtosys,
        skewness, min, max,
        mean, median, medianAD
    };
}

UniqueFeatures::UniqueFeatures(vector<Packet> packets) {
    // find pivot
    map<u_int64_t, u_int16_t> sizeAmounts;
    u_int64_t totalSize = 0, MTU = 0;
    for (Packet packet: packets) {
        u_int64_t size = packet.getSize();
        sizeAmounts[size]++;
        totalSize += size;
        MTU = max(MTU, size);
    }
    for (Packet p : packets)
        if (p.getSize() != MTU) {
            pivotSize = p.getSize();
            break;
        }
    for (auto &p : sizeAmounts)
        if (p.first != MTU && p.second > sizeAmounts[pivotSize])
            pivotSize = p.first;
    // pivot size / MTU size
    PM = float(pivotSize) / MTU;
    // pivot size / total sample size
    PT = float(pivotSize) / totalSize;
}

string UniqueFeatures::toString() {
    stringstream ss;
    ss.precision(5);
    ss << pivotSize << ' ' << PM << ' ' << PT;
    return ss.str();
}

vector<float> UniqueFeatures::toVector() {
    return {pivotSize, PM, PT};
}

void workWithDataFrames(vector<LogFrame> &frames) {
    // filter data frames
    vector<LogFrame> dataFrames = filter(frames, [](LogFrame f) { return f.getType().has_value() && f.getType().value().starts_with("Data") && f.getType().value().ends_with("Data"); });
    // grouped frames by TA
    map<u_int64_t, vector<LogFrame*>> transmitions;
    for (int i = 0; i < dataFrames.size(); i++) {
        optional<u_int64_t> tmp_num = dataFrames[i].getTA();
        if (!tmp_num.has_value())
            continue;
        u_int64_t TA = tmp_num.value();
        transmitions[TA].emplace_back(&dataFrames[i]);
    }
    // sort groupes by seqnum and fragnum
    for (auto &p : transmitions) {
        sort(p.second.begin(), p.second.end(), [](LogFrame* a, LogFrame* b) { return a->getSeqNum() != b->getSeqNum() ? a->getSeqNum() < b->getSeqNum() : a->getFragNum() < b->getFragNum(); });
    }
//    for (auto &p : transmitions) {
//        cout << hexToMAC(decToHex(p.first)) << " : ";
//        for (LogFrame* frame : p.second)
//            cout << frame->getSeqNum() << '(' << frame->getSize() << ") ";
//        cout << '\n';
//    }
    // collect transmitions to packets grouped by TA
    map<u_int64_t, vector<Packet>> D;
    for (auto &p : transmitions) {
        D[p.first] = vector<Packet>();
        for (int i = 0; i < p.second.size(); i++) {
            if (i == 0 || p.second[i]->getSeqNum() > p.second[i - 1]->getSeqNum()) {
                Packet new_p(p.second[i]->getSeqNum(), p.second[i]->getSize(), p.second[i]->getOffset(), {{p.second[i]->getSize(), p.second[i]->getOffset()}});
                D[p.first].emplace_back(new_p);
                continue;
            }
            u_int64_t last = D[p.first].size() - 1;
            D[p.first][last].addFragment(p.second[i]);
        }
        sort(D[p.first].begin(), D[p.first].end(), [](Packet a, Packet b) { return a.getArrivalTime() < b.getArrivalTime(); });
    }
//    for (auto &p : D) {
//        cout << hexToMAC(decToHex(p.first)) << " : ";
//        for (Packet packet : p.second)
//            cout << '(' << packet.getSize() << ", " << packet.getArrivalTime() << ") ";
//        cout << '\n';
//    }
    // cut first "MTU" packets from begin and save not more than 20 packets
    map<u_int64_t, vector<Packet>> SM;
    for (auto &p : D) {
        if (p.second.size() < 20)
            SM[p.first] = p.second;
        else {
            int left = 0;
            for (; left < p.second.size() - 1 && p.second[left].getSize() <= p.second[left + 1].getSize() && p.second.size() - left > 20; left++);
            int right = min(left + 20, int(p.second.size()));
            SM[p.first] = vector<Packet>();
            for (int i = left; i < right; i++)
                SM[p.first].emplace_back(p.second[i]);
        }
    };
//    for (auto &p : SM) {
//        cout << hexToMAC(decToHex(p.first)) << " : ";
//        for (Packet packet : p.second)
//            cout << '(' << packet.getSize() << ", " << packet.getArrivalTime() << ") ";
//        cout << '\n';
//    }
    // create Features DataSet
    map<u_int64_t, vector<float>> DS;
    for (auto &p : SM) {
        if (p.second.size() < 8)
            continue;
        vector<float> tmp;
        DS[p.first] = {};
        tmp = UniqueFeatures(p.second).toVector();
        DS[p.first].insert(DS[p.first].end(), tmp.begin(), tmp.end());
        tmp.clear();
        transform(p.second.begin(), p.second.end(), back_inserter(tmp), [](Packet p) { return p.getSize(); });
        tmp = StandardFeatures(tmp).toVector();
        DS[p.first].insert(DS[p.first].end(), tmp.begin(), tmp.end());
        tmp.clear();
        transform(p.second.begin(), p.second.end(), back_inserter(tmp), [](Packet p) { return p.getArrivalTime(); });
        tmp = StandardFeatures(tmp).toVector();
        DS[p.first].insert(DS[p.first].end(), tmp.begin(), tmp.end());
    }
    
    for (auto &p : DS)
        cout << hexToMAC(decToHex(p.first)) << " : " << toString(p.second) << '\n';
    
//    printToFile("/Users/alexshchelochkov/Desktop/STC/UAV_WiFi_detection/data/data.log", DS);
    
}

void workWithModel() {
    auto [macs, data, classes] = readDataForKNNFromFile("/Users/alexshchelochkov/Desktop/STC/UAV_WiFi_detection/data/data.log");
    normalize(data);
    u_int32_t classCnt = 0;
    for (const u_int32_t cl : classes)
        classCnt = max(classCnt, cl);
    vector<vector<u_int32_t>> targets = oneHotEncoding(classes, classCnt);
    LeaveOneOut lvoModel;
    lvoModel.fit(data, targets);
    
    // TEST
    vector<float> query = {601, 0.967794, 0.113525, 251.097, 63049.8, 502.418, 252424, -0.0076051, 3.18499e-10, -3.91268e-08, 82, 621, 441.167, 601, 20, 22.6925, 514.949, 34.301, 1176.56, 0.00741055, 4.72455e-06, 2.66138e-05, 1.2609, 57.3001, 26.5428, 25.2708, 24.0099,};
    u_int32_t expectedClass = 1;
    cout << "Test : actual(" << lvoModel.predict(query) << "), expected(" << expectedClass << ")\n";
}

void workWithDefiniteFile(function<void(vector<LogFrame> &)> action) {
    string path;
    cout << "Введите путь до файла:\n";
    cin >> path;
    cout << '\n';
    vector<LogFrame> frames;
    readFromFile(path, frames);
    action(frames);
}

void workWithAllFiles(function<void(vector<LogFrame> &)> action) {
    vector<string> paths {
        "/Users/alexshchelochkov/Desktop/STC/UAV_WiFi_detection/data/drones/3dr_solo/dsss/frames.log",
        "/Users/alexshchelochkov/Desktop/STC/UAV_WiFi_detection/data/drones/3dr_solo/wifi-ofdm-20/frames.log",
        "/Users/alexshchelochkov/Desktop/STC/UAV_WiFi_detection/data/drones/fimi_x8_me_2020/1wifi_fc_5825000000_fs_12000000.pcm.result/frames.log",
        "/Users/alexshchelochkov/Desktop/STC/UAV_WiFi_detection/data/drones/fimi_x8_me_2020/2wifi_fc_5825000000_fs_12000000.pcm.result/frames.log",
        "/Users/alexshchelochkov/Desktop/STC/UAV_WiFi_detection/data/drones/fimi_x8_me_2020/wifi_fc_5825000000_fs_12000000.pcm.result/frames.log",
        "/Users/alexshchelochkov/Desktop/STC/UAV_WiFi_detection/data/drones/hubsan_zino_2/Vega_2021-03-30_15-13-49-781_1_5785000000_10000000_11764706.log",
        "/Users/alexshchelochkov/Desktop/STC/UAV_WiFi_detection/data/drones/nelk_b6/NELK_B6_Downlink_5220.213483MHz_46625.000000KHz.pcm.log/frames.log",
        "/Users/alexshchelochkov/Desktop/STC/UAV_WiFi_detection/data/drones/parrot_bebop2/on_the_ground_gps-dsss/frames.log",
        "/Users/alexshchelochkov/Desktop/STC/UAV_WiFi_detection/data/drones/parrot_bebop2/on_the_ground_gps-ofdm-20/frames.log",
        "/Users/alexshchelochkov/Desktop/STC/UAV_WiFi_detection/data/drones/skydio2/3_Подключение_App,_калибровка,_попытка_полета-Unsafe_space/frames.log",
        "/Users/alexshchelochkov/Desktop/STC/UAV_WiFi_detection/data/drones/skydio2/4_Взлет_и_посадка_в_офисе/frames.log"
    };
    vector<LogFrame> frames;
    for (const string &path : paths) {
//        cout << path << '\n';
        readFromFile(path, frames);
        action(frames);
        frames.clear();
    }
}


pair<bool, bool> getFlagsOfExistance(const string &path) {
    bool hasHeader = path.ends_with("_phy.log");
    bool hasBody = path.ends_with("_parser.log");
    return {hasHeader, hasBody};
}

void workWithSeparatedFiles(function<void(vector<LogFrame> &)> action) {
    vector<string> paths {
        "/Users/alexshchelochkov/Desktop/STC/UAV_WiFi_detection/data/drones/part2/dji_mavic_air/handshake-work-goodbye.pcm/frames_parser.log",
        "/Users/alexshchelochkov/Desktop/STC/UAV_WiFi_detection/data/drones/part2/dji_mavic_air/handshake-work-goodbye.pcm/frames_phy.log",
        "/Users/alexshchelochkov/Desktop/STC/UAV_WiFi_detection/data/drones/part2/dji_tello/18_01_45 2427MHz 23312.5KHz.pcm/frames_parser.log",
        "/Users/alexshchelochkov/Desktop/STC/UAV_WiFi_detection/data/drones/part2/dji_tello/18_01_45 2427MHz 23312.5KHz.pcm/frames_phy.log",
        "/Users/alexshchelochkov/Desktop/STC/UAV_WiFi_detection/data/drones/part2/syma_x5_sw/10_24_27 2439.346405MHz 93250.000000KHz.pcm/frames_parser.log",
        "/Users/alexshchelochkov/Desktop/STC/UAV_WiFi_detection/data/drones/part2/syma_x5_sw/10_24_27 2439.346405MHz 93250.000000KHz.pcm/frames_phy.log",
        "/Users/alexshchelochkov/Desktop/STC/UAV_WiFi_detection/data/drones/part2/syma_x5_uw/10_31_11 2439.346405MHz 93250.000000KHz.pcm/frames_parser.log",
        "/Users/alexshchelochkov/Desktop/STC/UAV_WiFi_detection/data/drones/part2/syma_x5_uw/10_31_11 2439.346405MHz 93250.000000KHz.pcm/frames_phy.log",
        "/Users/alexshchelochkov/Desktop/STC/UAV_WiFi_detection/data/drones/part2/wltoys_q242/10_39_38 2439.346405MHz 93250.000000KHz(Video).pcm/frames_parser.log",
        "/Users/alexshchelochkov/Desktop/STC/UAV_WiFi_detection/data/drones/part2/wltoys_q242/10_39_38 2439.346405MHz 93250.000000KHz(Video).pcm/frames_phy.log",
        "/Users/alexshchelochkov/Desktop/STC/UAV_WiFi_detection/data/drones/part2/xiaomi_mi_drone_4k/13_40_03 5765.117978MHz 13321.428571KHz.pcm/frames_parser.log",
        "/Users/alexshchelochkov/Desktop/STC/UAV_WiFi_detection/data/drones/part2/xiaomi_mi_drone_4k/13_40_03 5765.117978MHz 13321.428571KHz.pcm/frames_phy.log"
    };
    vector<LogFrame> frames;
    map<u_int64_t, LogFrame> frameByInd;
    for (int i = 0; i < paths.size(); i += 2) {
        cout << paths[i] << '\n';
        auto [hasHeader, hasBody] = getFlagsOfExistance(paths[i]);
        readFromFile(paths[i], frames, hasHeader, hasBody);
        for (LogFrame frame : frames)
            frameByInd[frame.getInd()] = frame;
        frames.clear();
        tie(hasHeader, hasBody) = getFlagsOfExistance(paths[i + 1]);
        readFromFile(paths[i + 1], frames, hasHeader, hasBody);
        for (LogFrame &frame : frames) {
            LogFrame &oFrame = frameByInd[frame.getInd()];
            if (hasHeader && !hasBody) {
                if (!oFrame.isCorrect()) {
                    frame.setFCS(oFrame.isCorrect());
                    continue;
                }
                frame.setFCS(oFrame.isCorrect());
                frame.setType(oFrame.getType());
                frame.setSSID(oFrame.getSSID());
                frame.setTA(oFrame.getTA());
                frame.setRA(oFrame.getRA());
                frame.setMoreFrags(oFrame.getMoreFrags());
                frame.setSeqNum(oFrame.getSeqNum());
                frame.setFragNum(oFrame.getFragNum());
            } else {
                frame.setOffset(oFrame.getOffset());
                frame.setBW(oFrame.getBW());
                frame.setMCS(oFrame.getMCS());
                frame.setSize(oFrame.getSize());
                frame.setFrame(oFrame.getFrame());
            }
        }
        action(frames);
        frames.clear();
    }
}

