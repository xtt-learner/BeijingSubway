#include"DataProcessing.h"
DataProcessing::DataProcessing()
{


    Turn =
    {
        {"古城", {1, 0}},
        {"八桥",{1,-1}},
        {"土桥", {0, -1}},
        {"花庄", {-1, 0}},
    };
}



void DataProcessing::GetData(const std::string& sourcePath)
{
    std::ifstream file(sourcePath);

    if (!file.is_open()) {
        std::cerr << "无法打开文件" << std::endl;
        return ;
    }

    std::string line;
    int i=0;
    while (std::getline(file, line)) {
        // 忽略空行
        if (line.empty()) continue;

        // 处理行中的制表符和空格
        for (auto& c : line) {
            if (c == '\t' || c == ' ') {
                c = ' '; // 统一替换为空格
            }
        }

        // 使用stringstream分割字符串
        std::istringstream iss(line);
        std::string first, second;
        int distance;


        if (!(iss >> first)) continue;

        // 读取第二个站（可能包含多个单词）
        second = "";
        std::string word;
        while (iss >> word) {
            // 尝试将当前单词转换为数字
            std::istringstream numCheck(word);
            int temp;
            if (numCheck >> temp && numCheck.eof()) {
                // 转换成功且没有剩余字符，说明是距离
                distance = temp;
                break;
            }
            // 否则是第二个站的一部分
            if (!second.empty()) second += " ";
            second += word;
        }
        StationName.push_back(first);
        Distance.push_back(distance);
    }
}


void DataProcessing::GetPoints()
{
    int j = 0;
    for (int i = 0; i < Distance.size(); i++)
    {
        if (j < 3 && StationName[i] == Turn[j+1].first&&j<3)
        {
            j++;
        }
        PointF Point(FirstPointX)


    }






}
