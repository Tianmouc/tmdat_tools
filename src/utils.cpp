#include "utils.h"

using namespace std;

bool GreaterEqSort(std::string filePath1, std::string filePath2)
{
    int len1 = filePath1.length();
    int len2 = filePath2.length();
    if(len1 < len2)
    {
        return false;
    }
    else if(len1 > len2)
    {
        return true;
    }
    else
    {
        int iter = 0;
        while(iter < len1)
        {
            if(filePath1.at(iter) < filePath2.at(iter))
            {
                return false;
            }
            else if(filePath1.at(iter) > filePath2.at(iter))
            {
                return true;
            }
            ++iter;
        }
    }
    return true;
}

bool LessSort(std::string filePath1, std::string filePath2)
{
    return (!GreaterEqSort(filePath1, filePath2));
}


void pathSort(std::vector<std::string> &paths, int sortMode)
{
    if(sortMode == 1)
    {
        std::sort(paths.begin(), paths.end(), LessSort);
    }
}


void scanFilesUseStdFs(const std::string& rootPath,
                       std::vector<std::string>& container, std::string extension)
{
    std::filesystem::path fullpath(rootPath);

    if(!std::filesystem::exists(fullpath) || !std::filesystem::is_directory(fullpath))
    {
        std::cerr<<"File path not exist!" << std::endl;
        return;
    }

    std::filesystem::recursive_directory_iterator end_iter;
    for(std::filesystem::recursive_directory_iterator iter(fullpath); iter!=end_iter; iter++)
    {
        try
        {
            if(std::filesystem::is_directory( *iter ) ){
                std::cout<<*iter << "is dir" << std::endl;
            }
            else
            {
                if(std::filesystem::is_regular_file(*iter) && iter->path().extension() == extension)
                    container.push_back(iter->path().string());
            }
        } catch ( const std::exception & ex ){
            std::cerr << ex.what() << std::endl;
            continue;
        }
    }
}


void load_emul_fil(std::string filepath, int* pvalue, int size){
    std::ifstream fin(filepath, std::ios::in | std::ios::binary);
    fin.read((char *) pvalue, size * sizeof(int));
    fin.close();
}



void load_recorded_file(string filename, std::vector<uint64_t>& readStartPtrList,std::vector<uint64_t>& readEndPtrList){
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file." << std::endl;
        return;
    }
    uint64_t file_size;
    file_size = std::filesystem::file_size(filename);
    uint64_t readstart_ptr=0;
    uint64_t readend_ptr= 0;
    uint64_t frame_size = 0;

    while(readstart_ptr<file_size){
        file.seekg(readstart_ptr+6*4);
        file.read((char*)&frame_size, 4);
        frame_size = frame_size & 0xffffff;
        readStartPtrList.push_back(readstart_ptr);
        readend_ptr = (frame_size)*4 + readend_ptr;
        readEndPtrList.push_back(readend_ptr);
        readstart_ptr = frame_size*4 + readstart_ptr;
    }
    file.close();
    return;
}

bool safelyCreateFolder(const char* dir){
    using namespace std;
    if (access(dir, 0) == -1){
        return mkdir(dir,0755);
    }
    else{
        return true;
    }
}

bool isFileExists(std::string name) {
    ifstream f(name.c_str());
    return f.good();
}

uint64_t get_rod_timestamp(int* pvalue){
    uint64_t ts_h = static_cast<uint64_t>(pvalue[USB_ROD_TIMESTAMP_POS_H] & 0xffffff);
    uint64_t ts_m = static_cast<uint64_t> (pvalue[USB_ROD_TIMESTAMP_POS_M] & 0xffffff);
    uint64_t ts_l = static_cast<uint64_t>(pvalue[USB_ROD_TIMESTAMP_POS_L] & 0xffffff);
    uint64_t ts = static_cast<uint64_t> (ts_h << 48) + (ts_m << 24) + ts_l;
    return ts;
}

uint64_t get_cone_timestamp(int* pvalue){
    uint64_t ts_h = static_cast<uint64_t> (pvalue[USB_CONE_TIMESTAMP_POS_H] & 0xffffff);
    uint64_t ts_m = static_cast<uint64_t> (pvalue[USB_CONE_TIMESTAMP_POS_M] & 0xffffff);
    uint64_t ts_l = static_cast<uint64_t> (pvalue[USB_CONE_TIMESTAMP_POS_L] & 0xffffff);
    uint64_t ts = static_cast<uint64_t> ((ts_h) << 48) + (ts_m << 24) + ts_l;
    return ts;
}

uint32_t get_rod_counter(int* pvalue){
    uint32_t cnt_h = static_cast<uint32_t> (pvalue[USB_ROD_COUTNER_POS_H] & 0xffffff);
    uint32_t cnt_l = static_cast<uint32_t> (pvalue[USB_ROD_COUTNER_POS_L] & 0xffffff);
    uint32_t cnt = static_cast<uint32_t> (cnt_h << 24) + cnt_l;
    return cnt;
}

uint32_t get_cone_counter(int* pvalue){
    uint32_t cnt_h = static_cast<uint32_t> (pvalue[USB_CONE_COUTNER_POS_H] & 0xffffff);
    uint32_t cnt_l = static_cast<uint32_t> (pvalue[USB_CONE_COUTNER_POS_L] & 0xffffff);
    uint32_t cnt =   static_cast<uint32_t> (cnt_h << 24) + cnt_l;
    return cnt;
}
