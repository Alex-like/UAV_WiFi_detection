//
//  Classifier.cpp
//  UAV_WiFi_detection
//
//  Created by Alex Shchelochkov on 17.03.2022.
//

#include "Classifier.hpp"

using namespace std;

map<u_int64_t, string> Classifier::macToCompany = {
    {0x381D14000000, "Skydio Inc."},
    {0x00121C000000, "PARROT SA"},
    {0x00267E000000, "PARROT SA"},
    {0x9003B7000000, "PARROT SA"},
    {0x903AE6000000, "PARROT SA"},
    {0xA0143D000000, "PARROT SA"}
};
