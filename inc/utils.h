#ifndef _UTILS_H
#define _UTILS_H

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <filesystem>
#include <fstream>

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>


using namespace std;

#define ROD_8B_ONE_FRM 0x9e00
#define CONE_ONE_FRM 0x1A000 
#define USB_CONE_TIMESTAMP_POS_H 1
#define USB_CONE_TIMESTAMP_POS_M 2
#define USB_CONE_TIMESTAMP_POS_L 3
#define USB_CONE_COUTNER_POS_H 4
#define USB_CONE_COUTNER_POS_L 5
#define USB_ROD_TIMESTAMP_POS_H 1
#define USB_ROD_TIMESTAMP_POS_M 2
#define USB_ROD_TIMESTAMP_POS_L 3
#define USB_ROD_COUTNER_POS_H 4
#define USB_ROD_COUTNER_POS_L 5


bool GreaterEqSort(std::string filePath1, std::string filePath2);

bool LessSort(std::string filePath1, std::string filePath2);

void pathSort(std::vector<std::string> &paths, int sortMode);

void scanFilesUseStdFs(const std::string& rootPath,std::vector<std::string>& container, std::string extension);

void load_emul_fil(std::string filepath, int* pvalue, int size);

void load_recorded_file(string filename, std::vector<uint64_t>& readStartPtrList,std::vector<uint64_t>& readEndPtrList);

bool safelyCreateFolder(const char* dir);

bool isFileExists(std::string name);

uint64_t get_rod_timestamp(int* pvalue);

uint64_t get_cone_timestamp(int* pvalue);

uint32_t get_rod_counter(int* pvalue);

uint32_t get_cone_counter(int* pvalue);

#endif