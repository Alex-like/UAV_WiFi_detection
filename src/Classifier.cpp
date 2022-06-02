//
//  Classifier.cpp
//  UAV_WiFi_detection
//
//  Created by Alex Shchelochkov on 17.03.2022.
//

#include "Classifier.hpp"

#define BROADCAST 0xffffffffffff

using namespace std;

string toString(Type dev) {
    switch (dev) {
        
        case Drone:         return "Drone";
        case Controller:    return "Remote controller";
        case AccessPoint:   return "Access point / Client / Something else";
        case Unknown:       return "Unknown device";
    }
}

string toString(Feature ftr) {
    switch (ftr) {
        case MAC:   return "specified MAC";
        case SSID:  return "specified SSID";
    }
}

Object::Object(u_int64_t mac_v, set<Feature> features_v, Type device_v) {
    MAC = mac_v;
    features = features_v;
    device = device_v;
}

string Object::toString() {
    stringstream ss;
    ss << "MAC: " << hexToMAC(decToHex(MAC)) << "\tDevice: " << ::toString(device) << '\n';
    if (device < 3) {
        ss << "Info: " << info.value() << '\n';
        ss << "Features:\n";
        for (Feature feature : features) {
            ss << '\t' << ::toString(feature) << '\n';
        }
    }
    return ss.str();
}

void Object::setInfo(string data) {
    info = data;
}

void Object::setDevice(Type dev) {
    device = dev;
}

void Object::addFeature(Feature ftr) {
    features.insert(ftr);
}

Classifier::Classifier(GroupedGraph network_v) {
    network = network_v;
}

bool Classifier::classifyByMac(u_int64_t mac, Object& obj) {
    u_int64_t mac_mask = 0x0;
    if (!checkExist(mac & 0xffffff000000, macToCompany)) {
        if (!checkExist(mac & 0xfffffff00000, macToCompany)) {
            return false;
        }
        mac_mask = mac & 0xfffffff00000;
    } else {
        mac_mask = mac & 0xffffff000000;
    }
    string company = macToCompany[mac_mask];
    obj.setInfo(company);
    if (droneCompanies.find(company) != droneCompanies.end()) {
        obj.setDevice(Drone);
        obj.addFeature(MAC);
        return true;
    }
    if (network.checkExistAsHost(mac)) {
        obj.setDevice(Drone);
        obj.addFeature(MAC);
        return true;
    }
    if (network.checkExistAsClient(mac)) {
        obj.setDevice(Controller);
        obj.addFeature(MAC);
        return true;
    }
    obj.setDevice(AccessPoint);
    obj.addFeature(MAC);
    return true;
}

bool Classifier::classifyBySSID(optional<string> ssid, Object& obj) {
    if (!ssid.has_value()) {
        return false;
    }
    for (string word : stopWords) {
        if (strToLower(ssid.value()).find(word) != string::npos) {
            obj.setInfo(ssid.value());
            obj.setDevice(Drone);
            obj.addFeature(SSID);
            return true;
        }
    }
    return false;
}

void Classifier::setNetwork(GroupedGraph network_v) {
    network = network_v;
}

GroupedGraph Classifier::getNetwork() {
    return network;
}

optional<Object> Classifier::classify(u_int64_t mac, optional<string> ssid) {
    if (mac == BROADCAST) {
        return nullopt;
    }
    
    Object obj = Object(mac);
    
    if (!classifyByMac(mac, obj) && !classifyBySSID(ssid, obj)) {
        obj.setDevice(Unknown);
    }
    return obj;
}

map<u_int64_t, string> Classifier::getCompanies() {
    return macToCompany;
}

set<string> Classifier::getStopWords() {
    return stopWords;
}

set<string> Classifier::stopWords = {
    "skydio", "bebop", "drone", "sololink",
};

map<u_int64_t, string> Classifier::macToCompany = {
    {0x1831BF000000, "ASUSTek COMPUTER INC."}, // 3rd type
    {0x708BCD000000, "ASUSTek COMPUTER INC."},
    
    {0x74C63B000000, "AzureWave Technology Inc."}, // 3rd type
    
    {0x6CDFFBE00000, "Beijing Fimi Technology Co., Ltd."}, // 1st type
    
    {0xE03E44000000, "Broadcom"}, // 3rd type
    
    {0x10BD18000000, "Cisco Systems, Inc"}, // 3rd type
    {0x5C5015000000, "Cisco Systems, Inc"},
    {0x6400F1000000, "Cisco Systems, Inc"},
    
    {0xF07D68000000, "D-Link Corporation"}, // 3rd type
    
    {0x58D56E000000, "D-Link International"}, // 3rd type
    {0xD8FEE3000000, "D-Link International"},
    
    {0x88DC96000000, "EnGenius Technologies, Inc."}, // 3rd type
    
    {0xDAA119000000, "Google, Inc."}, // 3rd type
    
    {0x001882000000, "HUAWEI TECHNOLOGIES CO.,LTD"}, // 3rd type
    {0x001E10000000, "HUAWEI TECHNOLOGIES CO.,LTD"},
    {0x002568000000, "HUAWEI TECHNOLOGIES CO.,LTD"},
    {0x00259E000000, "HUAWEI TECHNOLOGIES CO.,LTD"},
    {0x002EC7000000, "HUAWEI TECHNOLOGIES CO.,LTD"},
    {0x0034FE000000, "HUAWEI TECHNOLOGIES CO.,LTD"},
    {0x00464B000000, "HUAWEI TECHNOLOGIES CO.,LTD"},
    {0x004F1A000000, "HUAWEI TECHNOLOGIES CO.,LTD"},
    {0x005A13000000, "HUAWEI TECHNOLOGIES CO.,LTD"},
    {0x006151000000, "HUAWEI TECHNOLOGIES CO.,LTD"},
    {0xEC5623000000, "HUAWEI TECHNOLOGIES CO.,LTD"},
    
    {0x48A472000000, "Intel Corporate"}, // 3rd
    {0x48F17F000000, "Intel Corporate"},
    {0x5CE0C5000000, "Intel Corporate"},
    {0x606720000000, "Intel Corporate"},
    {0x6C8814000000, "Intel Corporate"},
    {0x7C7A91000000, "Intel Corporate"},
    {0x7CB27D000000, "Intel Corporate"},
    {0x94659C000000, "Intel Corporate"},
    {0x98AF65000000, "Intel Corporate"},
    {0xBCA8A6000000, "Intel Corporate"},
    
    {0x28D244000000, "LCFC(HeFei) Electronics Technology co., ltd"}, // 3rd type
    
    {0x9822EF000000, "Liteon Technology Corporation"}, // 3rd type
    
    {0x00121C000000, "PARROT SA"}, // 1st type
    {0x00267E000000, "PARROT SA"},
    {0x9003B7000000, "PARROT SA"},
    {0x903AE6000000, "PARROT SA"},
    {0xA0143D000000, "PARROT SA"},
    
    {0x88A73C000000, "Ragentek Technology Group"}, // 3rd type
    
    {0x4C5E0C000000, "Routerboard.com"}, // 3rd type
    
    {0x98AAFC700000, "Shenzhen Hubsan Technology Co.，LTD."}, // 1st type
    
    {0x381D14000000, "Skydio Inc."}, // 1st type
    
    {0x706582000000, "Suzhou Hanming Technologies Co., Ltd."}, // 3rd type
    
    {0xC46E1F000000, "TP-LINK TECHNOLOGIES CO.,LTD."}, // 3rd type
    
    {0x3C970E000000, "Wistron InfoComm(Kunshan)Co.,Ltd."}, // 3rd type
    
    {0x80AD16000000, "Xiaomi Communications Co Ltd"},  // 3st type

//    {0x000000, "HUAWEI TECHNOLOGIES CO.,LTD"},
//    {0x000000, "HUAWEI TECHNOLOGIES CO.,LTD"},
//    {0x000000, "HUAWEI TECHNOLOGIES CO.,LTD"},
//    {0x000000, "HUAWEI TECHNOLOGIES CO.,LTD"},
//    {0x000000, "HUAWEI TECHNOLOGIES CO.,LTD"},
};

set<string> Classifier::droneCompanies = {
    "Beijing Fimi Technology Co., Ltd.",
    "PARROT SA",
    "Skydio Inc.",
};
