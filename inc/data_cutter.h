#include "utils.h"
#include <string>

using namespace std;


void parse_sample_dir(string readDir,
    int camera_idx,
    string& coneRawFileName,
    std::vector<uint64_t>& coneReadStartPtrList,
    std::vector<uint64_t>& coneReadEndPtrList,
    string& rodRawFileName,
    std::vector<uint64_t>& rodReadStartPtrList,
    std::vector<uint64_t>& rodReadEndPtrList);


void cut_aop_data(const string& rodRawFileName,
    const std::vector<uint64_t>& rodReadStartPtrList,
    const std::vector<uint64_t>& rodReadEndPtrList,
    int start_idx,
    int end_idx,
    int start_time,
    int end_time,
    string outputFileName);


void cut_cop_data(const string& coneRawFileName,
    const std::vector<uint64_t>& coneReadStartPtrList,
    const std::vector<uint64_t>& coneReadEndPtrList,
    int start_idx,
    int end_idx,
    int& start_time,
    int& end_time,
    string outputFileName);