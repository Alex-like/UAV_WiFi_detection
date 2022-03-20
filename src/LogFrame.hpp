//
//  LogFrame.hpp
//  UAV_WiFi_detection
//
//  Created by Alex Shchelochkov on 13.02.2022.
//

#ifndef LogFrame_hpp
#define LogFrame_hpp

#include <vector>
#include <string>
#include <regex>
#include <iostream>
#include <sstream>
#include <optional>

using namespace std;

/**
 * Frame structure.
 *
 * Contains all inforfation about frame.
 */
class LogFrame {
private:
    // 1 Line
    /// Frame index.
    u_int64_t ind;
    /// Frame offset value.
    float Offset;
    /// Bandwidth.
    string BW;
    /// Modulation and Coding Scheme.
    string MCS;
    /// Frame size.
    int Size;
    // 2 Line
    /// Hexadecimal data representation.
    string Frame;
    // 3 Line
    /// Decoded data.
    string info;
    /// Frame type if frame is correct.
    optional<string> Type;
    /// Frame SSID if frame is correct. Like name.
    optional<string> SSID;
    // MAC addresses
    /// TA - transmitter address, if frame is correct.
    optional<u_int64_t> TA;
    /// RA - receiver address, if frame is correct.
    optional<u_int64_t> RA;
    /// Correctness of frame.
    bool FCS;
public:
    /**
     * Basic constructor.
     *
     * @param ind_v frame index.
     * @param Offset_v offset value.
     * @param BW_v bandwidth.
     * @param MCS_v modulation and coding scheme.
     * @param Size_v frame size.
     * @param Frame_v hexadecimal value of data.
     * @param info_v decoded data.
     * @param FCS_v frame correctness.
     * @param Type_v frame type if frame is correct.
     * @param SSID_v frame name if frame is correct.
     * @param TA_v transmitter address if frame is correct and has it.
     * @param RA_v receiver address if frame is correct and has it.
     */
    LogFrame(u_int64_t ind_v, float Offset_v, string BW_v, string MCS_v, int Size_v, string Frame_v, string info_v, bool FCS_v, optional<string> Type_v, optional<string> SSID_v,optional<u_int64_t> TA_v, optional<u_int64_t> RA_v);
    /**
     * Get correctness of frame.
     *
     * @return frame correctness.
     */
    bool isCorrect();
    /**
     * Convert information to string value.
     *
     * @return string representation of some information about frame.
     */
    string toString();
    /**
     * Convert TA and RA to string value.
     *
     * @return string representation of TA and RA.
     */
    string getTAAndRA();
    /**
     * Get frame type.
     *
     * @return frame type if frame is correct.
     */
    optional<string> getType();
    /**
     * Get transmitter addres.
     *
     * @return TA if frame is correct and has it.
     */
    optional<u_int64_t> getTA();
    /**
     * Get receiver address.
     *
     * @return RA if frame is correct and has it.
     */
    optional<u_int64_t> getRA();
    /**
     * Get frame SSID.
     *
     * @return SSID if frame is correct and has it.
     */
    optional<string> getSSID();
    
    string getData();
};

/**
 * Get frame information from string.
 *
 * @param lines reference to separated lines which contain information.
 *
 * @return extracted information about frame.
 */
LogFrame parse(const vector<string> &lines);

#endif /* LogFrame_hpp */
