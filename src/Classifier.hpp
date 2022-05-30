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
#include "GroupedGraph.hpp"

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
/**
 * Contains information about specific MAC-address.
 */
class Object {
private:
    /// Specific MAC-address.
    u_int64_t MAC;
    /// Set with features which was detected.
    set<Feature> features;
    /// Type of device whose MAC-address equals to this.
    Type device;
    /// Extra information.
    optional<string> info;
public:
    /**
     * Basic constructor.
     *
     * @param mac_v device's MAC-address.
     * @param features_v set for detected featers.
     * @param device_v result of classification.
     */
    Object(u_int64_t mac_v, set<Feature> features_v = {}, Type device_v = Unknown);
    /**
     * Convert device definition to string.
     *
     * @return string representation of device.
     */
    string toString();
    /**
     * Set extra information about device.
     *
     * @param data extra information.
     */
    void setInfo(string data);
    /**
     * Set device's type.
     *
     * @param dev device's type.
     */
    void setDevice(Type dev);
    /**
     * Add detected feature.
     *
     * @param ftr detected feature.
     */
    void addFeature(Feature ftr);
};
/**
 * Classify device by MAC-address.
 */
class Classifier {
private:
    /// MACS which belongs to definite company.
    static map<u_int64_t, string> macToCompany;
    /// Words which help to identify device by SSID.
    static set<string> stopWords;
    /// Companies which makes drones.
    static set<string> droneCompanies;
    /// Current network.
    GroupedGraph network;
    /**
     * Classify device by MAC.
     *
     * @param mac MAC-address of device.
     * @param obj reference to device definition.
     *
     * @return if device has specific MAC-address return "True", else return "False".
     */
    bool classifyByMac(u_int64_t mac, Object& obj);
    /**
     * Classify device by SSID.
     *
     * @param ssid SSID of device.
     * @param obj reference to device definition.
     *
     * @return if device has specific SSID return "True", else return "False".
     */
    bool classifyBySSID(optional<string> ssid, Object& obj);
public:
    /**
     * Basic default constructor.
     *
     * @param network_v current network.
     */
    Classifier(GroupedGraph network_v = GroupedGraph());
    /**
     * Set current network.
     *
     * @param network_v current network.
     */
    void setNetwork(GroupedGraph network_v);
    /**
     * Get current network.
     *
     * @return current network.
     */
    GroupedGraph getNetwork();
    /**
     * Classify device by MAC-address or SSID.
     *
     * @param mac device's MAC-address.
     * @param ssid device's SSID if it exists.
     *
     * @return device definition if MAC-address not equals to broadcast.
     */
    optional<Object> classify(u_int64_t mac, optional<string> ssid);
    /**
     * Get map of MACs and companies which match them.
     *
     * @return map with MACs and companies.
     */
    map<u_int64_t, string> getCompanies();
    /**
     * Get special words which can occur in SSIDs.
     *
     * @return set with special words.
     */
    set<string> getStopWords();
};

#endif /* Classifier_hpp */
