//
//  MainUtils.hpp
//  UAV_WiFi_detection
//
//  Created by Alex Shchelochkov on 25.04.2022.
//

#ifndef MainUtils_hpp
#define MainUtils_hpp

#include <iostream>
#include <fstream>
#include <functional>
#include <algorithm>
#include <numeric>
#include <tuple>
#include <cassert>
#include "LogFrame.hpp"
#include "Statistics.hpp"
#include "Utils.hpp"
#include "Graph.hpp"
#include "GroupedGraph.hpp"
#include "Classifier.hpp"
#include "KNN.hpp"

using namespace std;

/**
 * Get statistic of processed frames.
 *
 * @param frames reference to stream of frames.
 */
void getStatistics(vector<LogFrame> &frames);
/**
 * Get amout of frames with different types.
 *
 * @param frames reference to stream of frames.
 */
void countFrameTypes(vector<LogFrame> &frames);
/**
 * Make graph with dependences of MAC-addresses.
 *
 * @param frames reference to stream of frames.
 */
void makeSimpleGraph(vector<LogFrame> &frames);
/**
 * Grouped MAC-dependences by networks.
 *
 * @param frames reference to stream of frames.
 */
void getNetworks(vector<LogFrame> &frames);
/**
 * Get list of MAC-addresses from processed frames.
 *
 * @param frames reference to stream of frames.
 */
void getAddressesFromFrames(vector<LogFrame> &frames);
/**
 * Get list of SSIDs from processed frames.
 *
 * @param frames reference to stream of frames.
 */
void getSSIDFromFrames(vector<LogFrame> &frames);
/**
 * Get MAC-address classification.
 *
 * @param frames reference to stream of frames.
 */
void classifyAllMacFromFrames(vector<LogFrame> &frames);
/**
 * Output types of processed frames.
 *
 * @param frames reference to stream of frames.
 */
void printFramesTypes(vector<LogFrame> &frames);
/**
 * Filter data-frames, exclude features from them and make dataset with features.
 *
 * @param frames reference to stream of frames.
 */
void excludeDataForTrainingSet(vector<LogFrame> &frames);
/**
 * Filter data-frames and process them.
 *
 * @param frames reference to stream of frames.
 */
void workWithDataFrames(vector<LogFrame> &frames);
/**
 * Process single file with frames.
 *
 * @param action worker with frames.
 */
void workWithDefiniteFile(function<void(vector<LogFrame> &)> action);
/**
 * Process all files from source.
 *
 * @param action worker with frames for each file.
 */
void workWithAllFiles(function<void(vector<LogFrame> &)> action);
/**
 * Get pair of flags which shows existance of header and body in frames from file.
 *
 * @param path file's path.
 *
 * @return pair with boolean flags of existance.
 */
pair<bool, bool> getFlagsOfExistance(const string &path);
/**
 * Process all files with separated frames.
 *
 * @param action worker with frames for each file.
 */
void workWithSeparatedFiles(function<void(vector<LogFrame> &)> action);
/**
 * Train model with training data.
 *
 * @return LeaveOneOut model.
 */
LeaveOneOut trainModel();
/**
 * Train model and predict something.
 *
 * @param query of features for test.
 */
void workWithModel(vector<float> &query);
/**
 * Test our trained model.
 */
void testModel();

class Packet {
private:
    u_int64_t number;
    u_int64_t size;
    float arrivalTime;
    vector<pair<u_int64_t, u_int64_t>> fragments;
public:
    Packet(u_int64_t id = 0, u_int64_t size_v = 0, float time = 0, vector<pair<u_int64_t, u_int64_t>> frags = {});
    void addFragment(LogFrame* frame);
    u_int64_t getID();
    u_int64_t getSize();
    float getArrivalTime();
};
/**
 * Collect transmitions into packets and group them by TA.
 *
 * @param frames reference to vector of Logframes.
 *
 * @return map with packets
 * key: TA (MAC-address);
 * value: sorted vector of Packets.
 */
map<u_int64_t, vector<Packet>> collectPacketsByTA(const vector<LogFrame> &frames);
/**
 * Make vector of features.
 *
 * @param packets reference to vector with packets.
 *
 * @return vector of features.
 */
vector<float> excludeFeaturesFromPackets(const vector<Packet> &packets);
/**
 * Make a report for work.
 */
void reportTaskWriter();

class StandardFeatures {
public:
    float standartDeviation;
    float variance;
    float rootMeanSquare;
    float m_square;
    float p_skewness;
    float kurtosys;
    float skewness;
    float min;
    float max;
    float mean;
    float median;
    float medianAD;
    StandardFeatures(vector<float> xs = {});
    string toString();
    vector<float> toVector();
};
class UniqueFeatures {
public:
    float pivotSize;
    float PM;
    float PT;
    UniqueFeatures(vector<Packet> packets = {});
    string toString();
    vector<float> toVector();
};

#endif /* MainUtils_hpp */
