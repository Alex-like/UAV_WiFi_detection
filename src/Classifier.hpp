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
#include <regex>
#include <iostream>
#include <sstream>
#include <optional>

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
    /// Other devices.
    Unknown
};

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

class Object {
private:
    u_int64_t MAC;
    set<Feature> features;
    Type device;
public:
    Object(u_int64_t mac_v);
    string toString();
};

class Classifier {
private:
    static map<u_int64_t, string> macToCompany;
public:
    Classifier();
    static Object classify(u_int64_t mac);
};

#endif /* Classifier_hpp */
