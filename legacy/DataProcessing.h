
#include <fstream>
#include <sstream>
#include<iostream>
#include<vector>
#include<string>
#include <utility>
struct PointF
{
    float X;
    float Y;
};
using namespace std;
class DataProcessing
{
public:
    DataProcessing();
    void GetData(const std::string& sourcePath);
    void GetPoints();

private:
    vector<std::string>StationName;
    vector<int>Distance;
    vector<PointF>points;
    vector<PointF>FirstPoints;
    vector<std::pair<std::string,vector<int>>>Turn;

};
