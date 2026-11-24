#include"Subway.h"
Subway::Subway()
{
    int ID=1;
    for(int i=0;i<=27;i++)
    {
        Line NewLine;
        NewLine.LineID=ID;
        ID++;
        Lines.push_back(NewLine);
    }
    QColor deepRed(204, 0, 0);
    Lines[0].name=QStringLiteral("1号线");
    Lines[0].color=deepRed;
    Lines[1].name=QStringLiteral("2号线");
    Lines[1].color=Qt::blue;
    Lines[2].name=QStringLiteral("3号线");
    Lines[2].color=Qt::red;
    QColor CyanGreen(0, 170, 153);
    Lines[3].name=QStringLiteral("4号线（含大兴线）");
    Lines[3].color=CyanGreen;
    QColor Violet(166, 33, 127);
    Lines[4].name=QStringLiteral("5号线");
    Lines[4].color=Violet;
    QColor khaki(209, 151, 8);
    Lines[5].name=QStringLiteral("6号线");
    Lines[5].color=khaki;
    QColor Lightyellow(255, 223, 179);
    Lines[6].name=QStringLiteral("7号线");
    Lines[6].color=Lightyellow;
    QColor HunterGreen(22,154,108);
    Lines[7].name=QStringLiteral("8号线");
    Lines[7].color=HunterGreen;
    QColor LightGreen(143,195,31);
    Lines[8].name=QStringLiteral("9号线");
    Lines[8].color=LightGreen;
    QColor HunterBlue(0,155,192);
    Lines[9].name=QStringLiteral("10号线");
    Lines[9].color=HunterBlue;
    QColor Orange(237,121,107);
    Lines[10].name=QStringLiteral("11号线");
    Lines[10].color=Orange;
    QColor Coffee(189,111,22);
    Lines[11].name=QStringLiteral("12号线");
    Lines[11].color=Coffee;
    QColor Yellow(249,231,0);
    Lines[12].name=QStringLiteral("13号线");
    Lines[12].color=Yellow;
    QColor Pink(213,167,161);
    Lines[13].name=QStringLiteral("14号线");
    Lines[13].color=Pink;
    QColor Purple(91,44,104);
    Lines[14].name=QStringLiteral("15号线");
    Lines[14].color=Purple;
    QColor Green(118,163,46);
    Lines[15].name=QStringLiteral("16号线");
    Lines[15].color=Green;
    QColor SkyBlue(118,209,209);
    Lines[16].name=QStringLiteral("17号线北段");
    Lines[16].color=SkyBlue;
    Lines[17].name=QStringLiteral("17号线南段");
    Lines[17].color=SkyBlue;
    QColor LightPink(214,171,193);
    Lines[18].name=QStringLiteral("19号线");
    Lines[18].color=LightPink;
    QColor HunterOrange(228,96,34);
    Lines[19].name=QStringLiteral("房山线");
    Lines[19].color=HunterOrange;
    Lines[20].name=QStringLiteral("燕房线");
    Lines[20].color=HunterOrange;
    QColor Gray(162,155,187);
    Lines[21].name=QStringLiteral("首都机场线");
    Lines[21].color=Gray;
    QColor RedPink(222,130,178);
    Lines[22].name=QStringLiteral("昌平线");
    Lines[22].color=RedPink;
    QColor DaXingBLUE(0,74,159);
    Lines[23].name=QStringLiteral("大兴机场线");
    Lines[23].color=DaXingBLUE;
    QColor YiZhuangPink(228,0,119);
    Lines[24].name=QStringLiteral("亦庄线");
    Lines[24].color=YiZhuangPink;
    Lines[25].name=QStringLiteral("亦庄 T1 线");
    Lines[25].color=YiZhuangPink;
    Lines[26].name=QStringLiteral("西郊线");
    Lines[26].color=YiZhuangPink;
    QColor Brown(214,167,135);
    Lines[27].name=QStringLiteral("S1 线");
    Lines[27].color=Brown;

    Position=
    {
        ":/data/Line1.txt",
        ":/data/Line2.txt",
        ":/data/Line3.txt",
        ":/data/Line4.txt",
        ":/data/Line5.txt",
        ":/data/Line6.txt",
        ":/data/Line7.txt",
        ":/data/Line8.txt",
        ":/data/Line9.txt",
        ":/data/Line10.txt",
        ":/data/Line11.txt",
        ":/data/Line12.txt",
        ":/data/Line13.txt",
        ":/data/Line14.txt",
        ":/data/Line15.txt",
        ":/data/Line16.txt",
        ":/data/Line17North.txt",
        ":/data/Line17South.txt",
        ":/data/Line19.txt",
        ":/data/FangShan.txt",
        ":/data/YanFang.txt",
        ":/data/CapitalAirportLine.txt",
        ":/data/ChangPing.txt",
        ":/data/DaXingAirportLine.txt",
        ":/data/YiZhuang.txt",
        ":/data/YiZhuangT1.txt",
        ":/data/XiJiao.txt",
        ":/data/S1.txt",

    };
}





void Subway::GetLines()
{
    for (auto& line : Lines) {
        line.Stations.clear();
        line.Turns.clear();
        line.Description.clear();
    }
    for (int i = 0; i < Position.size(); ++i) {
        QFile file(Position.at(i));
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qWarning() << "Cannot open embedded line data:" << file.fileName();
            continue;
        }
        QTextStream in(&file);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        in.setCodec("UTF-8");
#endif
        while (!in.atEnd()) {
            const QString text = in.readLine().simplified();
            if (text.isEmpty()) continue;
            const QStringList parts = text.split(' ');
            if (parts.size() < 3) continue;
            bool okX = false, okY = false;
            const int x = parts[parts.size() - 2].toInt(&okX);
            const int y = parts.last().toInt(&okY);
            if (!okX || !okY) continue;
            const QString name = parts.mid(0, parts.size() - 2).join(' ');
            if (name == QStringLiteral("Turn")) {
                if (Lines[i].Stations.isEmpty()) continue;
                Turn turn;
                turn.points = QPointF(x, y);
                turn.PreStation = Lines[i].Stations.last();
                Lines[i].Turns.push_back(turn);
            } else {
                Station station;
                station.Name = name;
                station.points = QPointF(x, y);
                station.Messages.insert(Lines[i].LineID, Lines[i].Stations.size() + 1);
                Lines[i].Stations.push_back(station);
            }
        }
    }
}


void Subway::DataProcessing()
{
    // 使用QMultiMap按站名分组站点
    QMultiMap<QString, Station*> stationMap;

    // 第一次遍历：将所有站点按名称分组
    for (auto& line : Lines)
    {
        for (auto& station : line.Stations)
        {
            stationMap.insert(station.Name, &station);
        }
    }


    QList<QString> stationNames = stationMap.uniqueKeys();
    for (const QString& name : stationNames)
    {

        QList<Station*> stations = stationMap.values(name);


        if (stations.size() > 1)
        {

            Station* baseStation = stations.first();


            for (int i = 1; i < stations.size(); i++)
            {
                for (auto it = stations[i]->Messages.cbegin(); it != stations[i]->Messages.cend(); ++it)
                    baseStation->Messages.insert(it.key(), it.value());
            }


            for (Station* station : stations)
            {
                station->Messages = baseStation->Messages;
            }
        }
    }
    QFile file(":/data/LineData.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return ;
    }

    QTextStream in(&file);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    in.setCodec("UTF-8");
#endif
    int lineIndex = 0;
    QString currentBlock;
    const auto saveBlock = [&]() {
        if (!currentBlock.trimmed().isEmpty() && lineIndex < Lines.size())
            Lines[lineIndex++].Description = currentBlock.trimmed();
        currentBlock.clear();
    };
    while (!in.atEnd()) {
        const QString text = in.readLine().trimmed();
        if (text == QStringLiteral("NEXT")) saveBlock();
        else if (!text.isEmpty()) currentBlock += text + QLatin1Char('\n');
    }
    saveBlock(); // The last line description has no trailing NEXT delimiter.
}


QList<Station> Subway::FindBestWay()
{
    QList<Station>Path;
    for(int i=0; i<SelectedStation.size()-1; i++)
    {
        for(auto it=SelectedStation[i].Messages.begin(); it!=SelectedStation[i].Messages.end(); ++it)
        {
            int lineIndex = it.key() - 1;



            Line& currentLine = Lines[lineIndex];


            int nextStationIndex = -1;
            for(int j=0; j<currentLine.Stations.size(); j++)
            {
                if(currentLine.Stations[j].Name == SelectedStation[i+1].Name)
                {
                    nextStationIndex = j;
                    break;
                }
            }

            if(nextStationIndex == -1)
                continue;


            int currentStationIndex = SelectedStation[i].Messages.value(it.key());
            if(currentStationIndex < nextStationIndex)
            {
                for(int s=currentStationIndex+1; s<=nextStationIndex; s++)
                {
                    if(s >= 0 && s < currentLine.Stations.size())
                        Path.push_back(currentLine.Stations[s]);
                }
            }
            else
            {
                for(int s=currentStationIndex-1; s>=nextStationIndex; s--)
                {
                    if(s >= 0 && s < currentLine.Stations.size())
                        Path.push_back(currentLine.Stations[s]);
                }
            }
            break;
        }
    }
    if(Path.size()!=0)
    {
      return Path;
    }
    return findShortestPath();

}
void Subway::initializeFloyd() {

    StationIndex.clear();
    StationNames.clear();

    int index = 0;
    for (const auto& line : Lines) {
        for (const auto& station : line.Stations) {
            if (!StationIndex.contains(station.Name)) {


                StationIndex[station.Name] = index;
                StationNames.append(station.Name);
                index++;



            }
        }
    }

    int n = StationIndex.size();


    dist.resize(n);
    next.resize(n);
    lineUsed.resize(n);

    for (int i = 0; i < n; ++i) {
        dist[i].resize(n);
        next[i].resize(n);
        lineUsed[i].resize(n);

        for (int j = 0; j < n; ++j) {
            dist[i][j] = INT_MAX;
            next[i][j] = -1;
            lineUsed[i][j] = -1;
        }
    }

    // 自己到自己的距离为0
    for (int i = 0; i < n; i++) {
        dist[i][i] = 0;
        next[i][i] = i;
    }


    for (int lineIdx = 0; lineIdx < Lines.size(); lineIdx++) {
        const Line& line = Lines[lineIdx];
        for (int i = 0; i < line.Stations.size() - 1; i++) {
            const Station& station1 = line.Stations[i];
            const Station& station2 = line.Stations[i + 1];


            if (!StationIndex.contains(station1.Name) || !StationIndex.contains(station2.Name))
                continue;

            int u = StationIndex[station1.Name];
            int v = StationIndex[station2.Name];


            if (u < 0 || u >= n || v < 0 || v >= n)
                continue;
            //处理相邻站点
            dist[u][v] = 1;//相邻的站点距离为1
            dist[v][u] = 1;
            next[u][v] = v;
            next[v][u] = u;
            lineUsed[u][v] = lineIdx;
            lineUsed[v][u] = lineIdx;
        }
    }

//更新最短的路径
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {

                if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX &&
                    dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    next[i][j] = next[i][k];
                    lineUsed[i][j] = lineUsed[i][k];
                }
            }
        }
    }
}
QList<Station> Subway::findShortestPath()
{
    QList<Station> path;
    for(int i=0; i<SelectedStation.size()-1; i++)
    {QString startStation=SelectedStation[i].Name;
    QString endStation=SelectedStation[i+1].Name;
    // 检查站点是否存在
    if (!StationIndex.contains(startStation) || !StationIndex.contains(endStation)) {
        return path;
    }

    int startIdx = StationIndex[startStation];
    int endIdx = StationIndex[endStation];

    // 如果没有路径，返回空列表
    if (next[startIdx][endIdx] == -1) {
        return path;
    }

    // 重建路径
    int at = startIdx;
    while (at != endIdx) {
        path.append(getStationByName(StationNames[at]));
        at = next[at][endIdx];
    }
    path.append(getStationByName(StationNames[endIdx])); // 添加终点
    }
    return path;
}

Station Subway::getStationByName(const QString& name) const {


    for (const auto& line : Lines) {
        for (const auto& station : line.Stations) {
            if (station.Name == name) {
                return station;
            }
        }
    }


    return Station();
}
