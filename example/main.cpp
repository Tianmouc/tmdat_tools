#include "data_cutter.h"
#include <string>
#include <cassert>


int main(int argc, char* argv[]){
    string inputDir;
    string outputDir;
    int c1= 0;
    int c2= 0;
    int a1= 0;
    int a2= 0;
    int mode = 2;
    int camera_idx = 0;

    if (argc==1){
        std::cout<<"No argments given."<<std::endl;
        std::cout<<"--input tmdat sample dir path."<<std::endl;
        std::cout<<"--output tmdat sample dir path."<<std::endl;
        std::cout<<"--aop enable aop cut."<<std::endl;
        std::cout<<"--cop enable cop cut."<<std::endl;
        std::cout<<"--c1 cop start index."<<std::endl;
        std::cout<<"--c2 cop end index, [c1,c2)."<<std::endl;
        std::cout<<"--a1 aop start index."<<std::endl;
        std::cout<<"--a2 aop end index, [a1,a2)."<<std::endl;
        std::cout<<"--sync, use cop index range to cut bot cop and aop synchronisly."<<std::endl;
        std::cout<<"--camera_idx, default 0."<<std::endl;
        return -1;
    }else{
        for (int i =1 ; i<argc; i ++){

            if(std::string(argv[i])=="--input"){
                inputDir = std::string(argv[i+1]);
                i +=1;
                continue;
            }
            if(std::string(argv[i])=="--output"){
                outputDir = std::string(argv[i+1]);
                i +=1;
                continue;
            }
            if(std::string(argv[i])=="--camera_idx"){
                camera_idx = std::stoi(std::string(argv[i+1]));
                i +=1;
                continue;
            }
            if(std::string(argv[i])=="--c1"){
                c1 = std::stoi(std::string(argv[i+1]));
                i +=1;
                continue;
            }
            if(std::string(argv[i])=="--c2"){
                c2 = std::stoi(std::string(argv[i+1]));
                i +=1;
                continue;
            }
            if(std::string(argv[i])=="--a1"){
                a1 = std::stoi(std::string(argv[i+1]));
                i +=1;
                continue;
            }
            if(std::string(argv[i])=="--a2"){
                a2 = std::stoi(std::string(argv[i+1]));
                i +=1;
                continue;
            }
            if(std::string(argv[i])=="--aop"){
                mode = 0;
                continue;
            }
            if(std::string(argv[i])=="--cop"){
                mode = 1;
                continue;
            }
            if(std::string(argv[i])=="--sync"){
                mode = 2;
                continue;
            }
        
        std::cout<<"param not found:"<<std::string(argv[i])<<std::endl;
        return -2;

        }

    }
    string rodDir = outputDir + "/rod";
    string coneDir = outputDir + "/cone";
    if(!isFileExists(outputDir)){
        bool status = safelyCreateFolder(outputDir.c_str());
    }
    if(!isFileExists(rodDir)){
        bool status = safelyCreateFolder(rodDir.c_str());
    }
    if(!isFileExists(coneDir)){   
        bool status = safelyCreateFolder(coneDir.c_str());
    }

    string rodRawFileName,coneRawFileName;
    std::vector<uint64_t> coneReadStartPtrList;
    std::vector<uint64_t> coneReadEndPtrList;
    std::vector<uint64_t> rodReadStartPtrList;
    std::vector<uint64_t> rodReadEndPtrList;

    parse_sample_dir(inputDir,0,
                    coneRawFileName,coneReadStartPtrList,coneReadEndPtrList,
                    rodRawFileName,rodReadStartPtrList,rodReadEndPtrList);

    int ctime1=-1,ctime2=-1;
    if(mode ==1){
        string outputFileName = coneDir + "/cop_cutted.tmdat";
        cut_cop_data(coneRawFileName,coneReadStartPtrList,coneReadEndPtrList,c1,c2,ctime1,ctime2,outputFileName);
        return 1;
    }
    if(mode ==0){
        string outputFileName = rodDir + "/aop_cutted.tmdat";
        cut_aop_data(rodRawFileName,rodReadStartPtrList,rodReadEndPtrList,a1,a2,ctime1,ctime2,outputFileName);
        return 0;
    }
    if(mode ==2){
        string outputFileName1 = coneDir + "/cop_cutted.tmdat";
        cut_cop_data(coneRawFileName,coneReadStartPtrList,coneReadEndPtrList,c1,c2,ctime1,ctime2,outputFileName1);
        string outputFileName2 = rodDir + "/aop_cutted.tmdat";
        cut_aop_data(rodRawFileName,rodReadStartPtrList,rodReadEndPtrList,a1,a2,ctime1,ctime2,outputFileName2);
        return 2;
    }
    return -3;
}