//
//  Classifier.hpp
//  UAV_WiFi_detection
//
//  Created by Alex Shchelochkov on 17.03.2022.
//

#ifndef Classifier_hpp
#define Classifier_hpp

#include <vector>
#include <set>
#include <map>
#include <string>
#include <sstream>
#include <optional>
#include "Utils.hpp"

using namespace std;

/**
 * Type of device which MAC-address belongs to.
 *
 * Use to define which type of device is that.
 */
enum Type {
    /// Drone.
    Drone = 1,
    /// Remote controller.
    Controller,
    /// Access point.
    AccessPoint,
    /// Other device.
    Unknown
};

/**
 * Convert type to string.
 *
 * @param dev device type.
 *
 * @return string representation of `dev`.
 */
string toString(Type dev);

/**
 * Features for detection.
 *
 * Kind of feature which helped to detect type of device
 */
enum Feature {
    /// MAC-address which is belong to specific company.
    MAC = 1,
    /// Name of frame which contains company's name.
    SSID
};

/**
 * Convert feature to string.
 *
 * @param ftr feature.
 *
 * @return string representation of `ftr`.
 */
string toString(Feature ftr);

class Object {
private:
    u_int64_t MAC;
    set<Feature> features;
    Type device;
    optional<string> info;
public:
    Object(u_int64_t mac_v, set<Feature> features_v = {}, Type device_v = Unknown);
    string toString();
    void setInfo(string data);
    void setDevice(Type dev);
    void addFeature(Feature ftr);
};

class Classifier {
private:
    static map<u_int64_t, string> macToCompany;
    static set<string> stopWords;
    static set<string> droneCompanies;
public:
    Classifier();
    static optional<Object> classify(u_int64_t mac, optional<string> ssid);
    map<u_int64_t, string> getCompanies();
    set<string> getStopWords();
};

template<class T, class E>
bool checkExist(T key, map<T, E> &dict);

void checkExistAndAddDevice(u_int64_t key, map<u_int64_t, string> &dict, Object &obj);

void addDevice(u_int64_t key, map<u_int64_t, string> &dict, Object &obj);

#endif /* Classifier_hpp */
