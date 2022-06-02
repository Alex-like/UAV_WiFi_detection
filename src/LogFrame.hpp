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
    /// More fragments flag.
    optional<bool> moreFragments;
    /// Sequence number.
    optional<u_int64_t> seqNum;
    /// Fragment number.
    optional<u_int64_t> fragNum;
public:
    /**
     * Default constuctor.
     */
    LogFrame();
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
    LogFrame(u_int64_t ind_v, float Offset_v, string BW_v, string MCS_v, int Size_v, string Frame_v, string info_v, bool FCS_v, optional<string> Type_v, optional<string> SSID_v,optional<u_int64_t> TA_v, optional<u_int64_t> RA_v, optional<bool> moreFragments_v, optional<u_int64_t> seqNum_v, optional<u_int64_t> fragNum_v);
    /**
     * Get frame's index.
     *
     * @return index.
     */
    u_int64_t getInd();
    /**
     * Get frame's BW.
     *
     * @return BW value.
     */
    string getBW();
    /**
     * Get frame's MCS.
     *
     * @return MCS value.
     */
    string getMCS();
    /**
     * Get hexadecimal frame's data.
     *
     * @return hexadecimal representation of frame.
     */
    string getFrame();
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
    /**
     * Get decoded data of frame.
     *
     * @return decoded frame.
     */
    string getData();
    /**
     * Get frame size.
     *
     * @return number of transmited bytes.
     */
    int getSize();
    /**
     * Get flag of existing more fragments of current frame.
     *
     * @return MoreFragments flag.
     */
    bool getMoreFrags();
    /**
     * Get sequence number of frame.
     *
     * @return sequence number.
     */
    u_int64_t getSeqNum();
    /**
     * Get fragment number of frame.
     *
     * @return fragment number.
     */
    u_int64_t getFragNum();
    /**
     * Get time offset of current frame.
     *
     * @return time offset.
     */
    float getOffset();
    /**
     * Sst offset value.
     *
     * @param offset value.
     */
    void setOffset(const float offset);
    /**
     * Set BW value.
     *
     * @param bw value.
     */
    void setBW(const string bw);
    /**
     * Set MCS value.
     *
     * @param mcs value.
     */
    void setMCS(const string mcs);
    /**
     * Set Size of frame.
     *
     * @param size value.
     */
    void setSize(const int size);
    /**
     * Set bits representation of decoded information.
     *
     * @param frame hexadecimal string.
     */
    void setFrame(const string frame);
    /**
     * Set decoded data.
     *
     * @param info decoded frame.
     */
    void setData(const string info);
    /**
     * Set frame type.
     *
     * @param type of frame.
     */
    void setType(const optional<string> type);
    /**
     * Set SSID of frame.
     *
     * @param ssid string.
     */
    void setSSID(const optional<string> ssid);
    /**
     * Set TA of frame.
     *
     * @param ta MAC-address.
     */
    void setTA(const optional<u_int64_t> ta);
    /**
     * Set RA of frame.
     *
     * @param ra MAC-address.
     */
    void setRA(const optional<u_int64_t> ra);
    /**
     * Set FCS flag of frame.
     *
     * @param fcs flag.
     */
    void setFCS(const bool fcs);
    /**
     * Set MoreFragments flag of frame.
     *
     * @param moreFrags flag.
     */
    void setMoreFrags(const optional<bool> moreFrags);
    /**
     * Set Seqnum of frame.
     *
     * @param seqNum value.
     */
    void setSeqNum(const optional<u_int64_t> seqNum);
    /**
     * Set Fragnum of frame.
     *
     * @param fragNum value.
     */
    void setFragNum(const optional<u_int64_t> fragNum);
};
/**
 * Get frame information from string.
 *
 * @param lines reference to separated lines which contain information.
 * @param hasHeader flag to indicate existence of header.
 * @param hasBody flag to indicate existence of decoded body.
 *
 * @return extracted information about frame.
 */
LogFrame parse(const vector<string> &lines, const bool hasHeader = true, const bool hasBody = true);

#endif /* LogFrame_hpp */
