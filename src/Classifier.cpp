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
        case AccessPoint:   return "Access point";
        case Unknown:       return "Other device";
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

Classifier::Classifier() {}

template<class T, class E>
bool checkExist(T key, map<T, E> &dict) {
    return dict.find(key) != dict.end();
}

void checkExistAndAddDevice(u_int64_t key, map<u_int64_t, string> &dict, Object &obj) {
    if (checkExist(key, dict)) {
        addDevice(key, dict, obj);
    }
}

void addDevice(u_int64_t key, map<u_int64_t, string> &dict, Object &obj) {
    obj.setInfo(dict[key]);
    obj.setDevice(Drone);
    obj.addFeature(MAC);
}

optional<Object> Classifier::classify(u_int64_t mac, optional<string> ssid) {
    if (mac == BROADCAST) {
        return nullopt;
    }
    Object obj = Object(mac);
    checkExistAndAddDevice(mac & 0xffffff000000, macToCompany, obj);
    checkExistAndAddDevice(mac & 0xfffffff00000, macToCompany, obj);
    if (!ssid.has_value()) {
        return obj;
    }
    for (string word : stopWords) {
        if (ssid.value().find(word) != string::npos) {
            obj.setInfo(ssid.value());
            obj.setDevice(Drone);
            obj.addFeature(SSID);
            break;
        }
    }
    return obj;
}

set<string> Classifier::stopWords = {
    "skydio", "bebop", "drone"
};

map<u_int64_t, string> Classifier::macToCompany = {
    {0x381D14000000, "Skydio Inc."},
    
    {0x00121C000000, "PARROT SA"},
    {0x00267E000000, "PARROT SA"},
    {0x9003B7000000, "PARROT SA"},
    {0x903AE6000000, "PARROT SA"},
    {0xA0143D000000, "PARROT SA"},
    
    {0xEC5623000000, "HUAWEI TECHNOLOGIES CO.,LTD"},
    
    {0x88DC96000000, "EnGenius Technologies, Inc."},
    
    {0xE03E44000000, "Broadcom"},
    
    {0x6CDFFBE00000, "Beijing Fimi Technology Co., Ltd."},
    
    {0x98AAFC700000, "Shenzhen Hubsan Technology Co.，LTD."},
    
    {0x706582000000, "Suzhou Hanming Technologies Co., Ltd."},
    
    {0x10BD18000000, "Cisco Systems, Inc"},
    {0x5C5015000000, "Cisco Systems, Inc"},
    {0x6400F1000000, "Cisco Systems, Inc"},
    
    {0x1831BF000000, "ASUSTek COMPUTER INC."},
    {0x708BCD000000, "ASUSTek COMPUTER INC."},
    
    {0x28D244000000, "LCFC(HeFei) Electronics Technology co., ltd"},
    
    {0x3C970E000000, "Wistron InfoComm(Kunshan)Co.,Ltd."},
    
    {0x4C5E0C000000, "Routerboard.com"},
    
    {0x58D56E000000, "D-Link International"},
    
    {0x5CE0C5000000, "Intel Corporate"},
    {0x606720000000, "Intel Corporate"},
    {0x6C8814000000, "Intel Corporate"},
    
    {0x74C63B000000, "AzureWave Technology Inc."},
    
    {0x001882000000, "HUAWEI TECHNOLOGIES CO.,LTD"},
    {0x001E10000000, "HUAWEI TECHNOLOGIES CO.,LTD"},
    {0x002568000000, "HUAWEI TECHNOLOGIES CO.,LTD"},
    {0x00259E000000, "HUAWEI TECHNOLOGIES CO.,LTD"},
    {0x002EC7000000, "HUAWEI TECHNOLOGIES CO.,LTD"},
    {0x0034FE000000, "HUAWEI TECHNOLOGIES CO.,LTD"},
    {0x00464B000000, "HUAWEI TECHNOLOGIES CO.,LTD"},
    {0x004F1A000000, "HUAWEI TECHNOLOGIES CO.,LTD"},
    {0x005A13000000, "HUAWEI TECHNOLOGIES CO.,LTD"},
    {0x006151000000, "HUAWEI TECHNOLOGIES CO.,LTD"},

//    {0x000000, "HUAWEI TECHNOLOGIES CO.,LTD"},
//    {0x000000, "HUAWEI TECHNOLOGIES CO.,LTD"},
//    {0x000000, "HUAWEI TECHNOLOGIES CO.,LTD"},
//    {0x000000, "HUAWEI TECHNOLOGIES CO.,LTD"},
//    {0x000000, "HUAWEI TECHNOLOGIES CO.,LTD"},
//    {0x000000, "HUAWEI TECHNOLOGIES CO.,LTD"},
//    {0x000000, "HUAWEI TECHNOLOGIES CO.,LTD"},
//    {0x000000, "HUAWEI TECHNOLOGIES CO.,LTD"},
//    {0x000000, "HUAWEI TECHNOLOGIES CO.,LTD"},
//    {0x000000, "HUAWEI TECHNOLOGIES CO.,LTD"},
//    {0x000000, "HUAWEI TECHNOLOGIES CO.,LTD"},
//    {0x000000, "HUAWEI TECHNOLOGIES CO.,LTD"},
//    {0x000000, "HUAWEI TECHNOLOGIES CO.,LTD"},
//    {0x000000, "HUAWEI TECHNOLOGIES CO.,LTD"},
//    {0x000000, "HUAWEI TECHNOLOGIES CO.,LTD"},
//    {0x000000, "HUAWEI TECHNOLOGIES CO.,LTD"},
//    {0x000000, "HUAWEI TECHNOLOGIES CO.,LTD"},
//    {0x000000, "HUAWEI TECHNOLOGIES CO.,LTD"},
//    {0x000000, "HUAWEI TECHNOLOGIES CO.,LTD"},
};
