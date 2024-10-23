#!/bin/bash
#现在有一个工具，他的调用方法是：./tmdat_cut --input '/home/lyh/Downloads/origin' --output '/home/lyh/Downloads/output' --sync --c1 5 --c2 15，可以把输入路径的某个数据从c1切割至c2，输出到output中。现在给予一个txt文件，每行第一列是这个数据的名称，第二列是c1，第三列是c2，请写一个脚本，自动把input文件夹下的数据按照这个txt，切割完，输出到output文件夹

# 脚本使用的参数
TXT_FILE=file_list_interest.txt
INPUT_DIR='/data/lyh/tianmoucData/demo_test_Data/raw'
OUTPUT_DIR='/data/lyh/tianmoucData/demo_test_Data/cut'

# 检查输出目录是否存在，如果不存在则创建
mkdir -p $OUTPUT_DIR

# 读取文本文件中的每一行，并执行切割操作
while read -r file c1 c2; do
    echo "Processing $file from column $c1 to $c2"
    # 使用tmdat_cut工具进行切割操作，假设工具在环境变量路径中，或者当前目录下
    ./build/tmdat_cut --input "$INPUT_DIR/$file" --output "$OUTPUT_DIR/$file" --sync --c1 $c1 --c2 $c2
done < "$TXT_FILE"

echo "Data processing completed."