#include "data_cutter.h"
#include <cassert>

using namespace std;

void parse_sample_dir(string readDir,
    int camera_idx,
    string& coneRawFileName,
    std::vector<uint64_t>& coneReadStartPtrList,
    std::vector<uint64_t>& coneReadEndPtrList,
    string& rodRawFileName,
    std::vector<uint64_t>& rodReadStartPtrList,
    std::vector<uint64_t>& rodReadEndPtrList){

    std::string rod_data_path;
    std::string cone_data_path;
    if(camera_idx==0){
        rod_data_path = readDir  + "/rod";
        cone_data_path = readDir  + "/cone";
    }else{
        string strNum = std::to_string(camera_idx);
        rod_data_path = readDir  + "/rod_" + strNum;
        cone_data_path = readDir  + "/cone_" + strNum;
    }
    int try_idx = 0;

    std::string extension1 = ".tmdat";
    std::string extension2 = ".bin";
    std::vector<std::string> pathRodVec;
    pathRodVec.clear();
    scanFilesUseStdFs(rod_data_path, pathRodVec, extension1);
    scanFilesUseStdFs(rod_data_path, pathRodVec, extension2);
    pathSort(pathRodVec, 1);

    std::vector<std::string> pathConeVec;
    pathConeVec.clear();
    scanFilesUseStdFs(cone_data_path, pathConeVec, extension1);
    scanFilesUseStdFs(cone_data_path, pathConeVec, extension2);
    pathSort(pathConeVec, 1);

    int rodFileNum = pathRodVec.size();
    int coneFileNum = pathConeVec.size();

    if(rodFileNum==0 || coneFileNum==0){
        std::cout<<"No files!\n";
        return;
    }
    if(rodFileNum>1 || coneFileNum>1){
        std::cout<<"Multiple raw files under selected folder, please choose only one file!\n";
        std::cout<<"No files!\n";
        return;
    }

    coneRawFileName = pathConeVec[0];
    load_recorded_file(coneRawFileName, coneReadStartPtrList,coneReadEndPtrList);
    rodRawFileName = pathRodVec[0];
    load_recorded_file(rodRawFileName, rodReadStartPtrList,rodReadEndPtrList);

    std::cout<<"file num compare:"<< coneReadStartPtrList.size()<<","<<rodReadStartPtrList.size()<<std::endl;

    return;
}


void cut_cop_data(const string& coneRawFileName,
    const std::vector<uint64_t>& coneReadStartPtrList,
    const std::vector<uint64_t>& coneReadEndPtrList,
    int start_idx,
    int end_idx,
    int& ctime1,
    int& ctime2,
    string outputFileName){

    int cone_size = CONE_ONE_FRM * 2;
    int* cone_pvalue = (int *)malloc(cone_size * 1 * sizeof(int));//one times mo //TODOre


    std::ifstream coneRawfile(coneRawFileName, std::ios::binary);
    if (!coneRawfile) {
        std::cerr << "Failed to open file." << std::endl;
        return;
    }
    assert (start_idx>=0 && start_idx <end_idx && end_idx < coneRawFileName.size());
    std::ofstream record_data_file;
    record_data_file.open(outputFileName, std::ios::binary | std::ios::out);//init record file

    uint64_t coneFileNem = coneReadStartPtrList.size()-1;

    for(int conePtr=start_idx; conePtr <= end_idx; conePtr ++){

        uint64_t fileSize = coneReadEndPtrList[conePtr]-coneReadStartPtrList[conePtr];
        coneRawfile.seekg(coneReadStartPtrList[conePtr]);
        coneRawfile.read((char*)cone_pvalue, fileSize);
        
        uint64_t cone_effDataLength = fileSize/4;
        assert(cone_effDataLength == 16 + 320 * 160);
        
        uint64_t ConeTimeStamp = get_cone_timestamp(cone_pvalue);
        uint64_t cone_now_cnt = get_cone_counter(cone_pvalue);

        if(conePtr<=end_idx){
            record_data_file.write(reinterpret_cast<const char*>(cone_pvalue),sizeof(int)*cone_effDataLength);
            std::cout <<"ConeTimeStamp:"<< ConeTimeStamp<<"\n";
        }
        if(conePtr==start_idx) ctime1 = ConeTimeStamp;
        if(conePtr==end_idx)   ctime2 = ConeTimeStamp;        
    }
    return;
}

void cut_aop_data(const string& rodRawFileName,
    const std::vector<uint64_t>& rodReadStartPtrList,
    const std::vector<uint64_t>& rodReadEndPtrList,
    int start_idx,
    int end_idx,
    int start_time,
    int end_time,
    string outputFileName)
{
    int rodFileCount = 0;
    uint64_t rod_size = ROD_8B_ONE_FRM * 2;
    int* rod_pvalue = (int *)malloc(rod_size * 1 * sizeof(int));//one times mo //TODOre

    std::ifstream rodRawfile(rodRawFileName, std::ios::binary);
    if (!rodRawfile) {
        std::cerr << "Failed to open file." << std::endl;
        return;
    }

    std::ofstream record_data_file;
    record_data_file.open(outputFileName, std::ios::binary | std::ios::out);//init record file
                
    if (start_time == -1){
        assert (start_idx>=0 && start_idx <end_idx && end_idx <rodReadStartPtrList.size());
        for(int rodPtr=start_idx; rodPtr <= end_idx; rodPtr ++){
            uint64_t fileSize = rodReadEndPtrList[rodPtr]-rodReadStartPtrList[rodPtr];
            rodRawfile.seekg(rodReadStartPtrList[rodPtr]);
            rodRawfile.read((char*)rod_pvalue, fileSize);

            uint64_t RodTimeStamp = get_rod_timestamp(rod_pvalue);
            uint64_t rod_now_cnt = get_rod_counter(rod_pvalue);

            record_data_file.write(reinterpret_cast<const char*>(rod_pvalue),sizeof(int)*fileSize/4);
        }
    }else{
        for(int rodPtr=0; rodPtr <rodReadStartPtrList.size(); rodPtr ++){
            uint64_t fileSize = rodReadEndPtrList[rodPtr]-rodReadStartPtrList[rodPtr];
            rodRawfile.seekg(rodReadStartPtrList[rodPtr]);
            rodRawfile.read((char*)rod_pvalue, fileSize);

            uint64_t RodTimeStamp = get_rod_timestamp(rod_pvalue);
            uint64_t rod_now_cnt = get_rod_counter(rod_pvalue);

            if (RodTimeStamp>= start_time-2 && RodTimeStamp<= end_time+2){
                record_data_file.write(reinterpret_cast<const char*>(rod_pvalue),sizeof(int)*fileSize/4);
            }

        }
    }
    record_data_file.close();
}


