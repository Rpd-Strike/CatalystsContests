#include <bits/stdc++.h>

using namespace std;

ifstream in("text.in");
ofstream out("text.out");

const double EPS = 1e-9;

struct Point
{
    double x, y;

    Point(double x = 0, double y = 0) : x(x), y(y) {}
};

struct Line
{
    Point a, b;
};

namespace Geometry
{
    double distance(Point a, Point b)
    {
        return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
    }

    Point getXIntersect(Line d, double x)
    {
        double ratie = (x - d.a.x) / (d.b.x - d.a.x);
        return Point(d.a.x + (d.b.x - d.a.x) * ratie, d.a.y + (d.b.y - d.a.y) * ratie);
    }

    Point getYIntersect(Line d, double y)
    {
        double ratie = (y - d.a.y) / (d.b.y - d.a.y);
        return Point(d.a.x + (d.b.x - d.a.x) * ratie, d.a.y + (d.b.y - d.a.y) * ratie);
    }
}

pair<int,int> getCell(Point a)
{
    return {floor(a.x), floor(a.y)};
}

Point getCenter(Point pct)
{
    return Point(pct.x + 0.5, pct.y + 0.5);
}

class Matrice
{
public:
    int N, M;
    vector<vector<int>> data;

    void readMatrice()
    {
        data = vector<vector<int>>(N, vector<int>(M, 0));
        cout << data.size() << ' ' << data[0].size() << '\n';
        for(int i = 0; i < N;  ++i)
            for( int j = 0; j < M;  ++j)
                in >> data[i][j];
    }
};

struct Hotspot
{

};

const int INF  = (1 << 30);

int dx[4] = {-1, 0, 1, 0};
int dy[4] = {0, 1, 0, -1};

Matrice tabla;
int Slimit;
int upX, upY, downX, downY;
int hotUpX, hotDownX, hotUpY, hotDownY;
vector<vector<bool>> viz, cur, mark;
vector<vector<int>> buildingID;
set<pair<int,int>> buildingPoints;
vector<pair<int,int>> hotspotList;  /// coordonata coltul stanga-sus al hotspotului
vector<pair<int,int>> goodRelations;

bool inBounds(int x, int y, int N, int M)
{
    if( x < 0 || y < 0 ) return 0;
    if( x >= N || y >= M ) return 0;
    return 1;
}

void dfs(int x, int y, int building_number)
{
    buildingPoints.insert({x, y});
    buildingID[x][y] = building_number;
    viz[x][y] = 1;
    cur[x][y] = 1;
    upX = min(upX, x);
    upY = min(upY, y);
    downX = max(downX, x);
    downY = max(downY, y);
    for( int i = 0; i < 4;  ++i ) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        if( inBounds(nx, ny, tabla.N, tabla.M) ) {
            if( !viz[nx][ny] && tabla.data[nx][ny] == tabla.data[x][y] )
                dfs(nx, ny, building_number);
        }
    }
}

void checkAndMark(int x, int y, int S)
{
    bool ok = 1;
    for( int i = x; i < x + S; ++i ) {
        for( int j = y; j < y + S; ++j ) {
            if( !inBounds(i, j, tabla.N, tabla.M) ) {
                ok = 0;
                return ;
            }
            if( !cur[i][j] ) ok = 0;
        }
    }
    if( !ok ) return;

    for( int i = x; i < x + S; ++i ) {
        for( int j = y; j < y + S; ++j ) {
            mark[i][j] = 1;
        }
    }
}

void dfs_mark(int x, int y)
{
    mark[x][y] = 0;

    hotUpX = min(hotUpX, x);
    hotDownX = max(hotDownX, x);
    hotUpY = min(hotUpY, y);
    hotDownY = max(hotDownY, y);

    for( int i = 0; i < 4;  ++i ) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        if( inBounds(nx, ny, tabla.N, tabla.M) ) {
            if( mark[nx][ny] )
                dfs_mark(nx, ny);
        }
    }
}


void dfs_clear(int x, int y)
{
    cur[x][y] = 0;
    upX = min(upX, x);
    upY = min(upY, y);
    downX = max(downX, x);
    downY = max(downY, y);
    for( int i = 0; i < 4;  ++i ) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        if( inBounds(nx, ny, tabla.N, tabla.M) ) {
            if( cur[nx][ny] )
                dfs_clear(nx, ny);
        }
    }
}

vector<pair<int,int>> hotspotPath(Point a, Point b)
{
    Line d;
    map<pair<int,int>, int> pointsOnLine;
    d.a = a;  d.b = b;

    d.a = getCenter(d.a);
    d.b = getCenter(d.b);
    if( d.a.x > d.b.x )
        swap(d.a, d.b);
    for( int xCord = ceil(d.a.x); xCord <= floor(d.b.x); xCord += 1 ) {
        Point intersected = Geometry:: getXIntersect(d, xCord);
        pair<int,int> cellA = getCell(intersected);
        intersected.x -= 1;
        pair<int,int> cellB = getCell(intersected);

        pointsOnLine[cellA] = pointsOnLine[cellB] = 1;
    }

    if(d.a.y > d.b.y)
        swap(d.a, d.b);
    for( int yCord = ceil(d.a.y); yCord <= floor(d.b.y); yCord += 1 ) {
        Point intersected = Geometry:: getYIntersect(d, yCord);
        pair<int,int> cellA = getCell(intersected);
        intersected.y -= 1;
        pair<int,int> cellB = getCell(intersected);

        pointsOnLine[cellA] = pointsOnLine[cellB] = 1;
    }

    vector<pair<int,int>> solution;
    for(auto el : pointsOnLine)
        solution.push_back(el.first);
    return solution;
}

bool outsideOfCell(Point p, pair<int,int> cell)
{
    if( p.x + EPS < cell.first ) return 1;
    if( cell.first + 1 < p.x - EPS ) return 1;
    if( p.y + EPS < cell.second ) return 1;
    if( cell.second + 1 < p.y - EPS ) return 1;
    return 0;
}

double getHeightOfPoint(Line d, Point pct, pair<int,int> hotspotHeights)
{
    int startH = hotspotHeights.first;
    int endH = hotspotHeights.second;

    double ratie = Geometry::distance(d.a, pct) / Geometry::distance(d.a, d.b);
    double height = startH + (endH - startH) * ratie;
    return height;
}

bool checkRelation(pair<int,int> pa, pair<int,int> pb)
{
    int colA = buildingID[pa.first][pa.second];
    int colB = buildingID[pb.first][pb.second];

    Line dd;
    dd.a = getCenter( Point(pa.first, pa.second) );
    dd.b = getCenter( Point(pb.first, pb.second) );

    pair<int,int> hotspotHeights = {tabla.data[pa.first][pa.second],
                                    tabla.data[pb.first][pb.second]};

    vector<pair<int,int>> betweenPoints = hotspotPath(Point(pa.first, pa.second), Point(pb.first, pb.second));
   /// int nr_points = betweenPoints.size();
    for( pair<int,int> pct : betweenPoints ) {
        if( buildingID[pct.first][pct.second] == colA )
            continue;
        if( buildingID[pct.first][pct.second] == colB )
            continue;
        if( buildingID[pct.first][pct.second] == 0 )
            continue;

       /// int middleHeight = tabla.data[pct.first][pct.second];

        Point intersected = Geometry::getXIntersect(dd, pct.first);
        if( !outsideOfCell(intersected, pct) ) {
            double height = getHeightOfPoint(dd, intersected, hotspotHeights);
            if( height - EPS <= tabla.data[pct.first][pct.second] )
                return 0;
        }

        intersected = Geometry::getXIntersect(dd, pct.first + 1);
        if( !outsideOfCell(intersected, pct) ) {
            double height = getHeightOfPoint(dd, intersected, hotspotHeights);
            if( height - EPS <= tabla.data[pct.first][pct.second] )
                return 0;
        }

        /// Y intersect
        intersected = Geometry::getYIntersect(dd, pct.second);
        if( !outsideOfCell(intersected, pct) ) {
            double height = getHeightOfPoint(dd, intersected, hotspotHeights);
            if( height - EPS <= tabla.data[pct.first][pct.second] )
                return 0;
        }

        intersected = Geometry::getYIntersect(dd, pct.second + 1);
        if( !outsideOfCell(intersected, pct) ) {
            double height = getHeightOfPoint(dd, intersected, hotspotHeights);
            if( height - EPS <= tabla.data[pct.first][pct.second] )
                return 0;
        }
    }
    return 1;
}
const int NMAX = 1e5;
vector<int> G[NMAX+2];
vector<int> order;

int main()
{
    int maxCount;
    in >> tabla.N >> tabla.M >> Slimit >> maxCount;
    tabla.readMatrice();

    buildingID = vector<vector<int>>(tabla.N, vector<int>(tabla.M, 0));

    viz = vector<vector<bool>>(tabla.N, vector<bool>(tabla.M, 0));
    cur = vector<vector<bool>>(tabla.N, vector<bool>(tabla.M, 0));
    mark = vector<vector<bool>>(tabla.N, vector<bool>(tabla.M, 0));
    int building_number = 0;
    for( int i = 0; i < tabla.N; ++i ) {
        for( int j = 0; j < tabla.M;  ++j ) {
            if( !viz[i][j] && tabla.data[i][j] > 0 ) {
                upX = upY = INF;
                downX = downY = -INF;

                buildingPoints.clear();
                ++building_number;
                dfs(i,j, building_number);

                for( auto bPoint : buildingPoints )
                    checkAndMark(bPoint.first, bPoint.second, Slimit);

                for( auto bPoint : buildingPoints ) {
                    if( mark[bPoint.first][bPoint.second] ) {
                        hotUpX = hotUpY = INF;
                        hotDownX = hotDownY = -INF;

                        dfs_mark(bPoint.first, bPoint.second);

                        pair<int,int> hotspot = {(hotUpX + hotDownX) / 2, (hotUpY + hotDownY) / 2};

                        if( cur[hotspot.first][hotspot.second] ) {
                            hotspotList.push_back(hotspot);
                        }
                        else {
                            cout << "Am gasit un bad hotspot: " << hotspot.first << ' ' << hotspot.second << '\n';
                        }

                    }
                }

                dfs_clear(i,j);
            }
        }
    }

    /// afis hotspot list
    sort(hotspotList.begin(), hotspotList.end());
    int indice = 0;
    for( auto el : hotspotList ) {
      ///  out << indice << ' ';
      ///  out << el.first << ' ' << el.second << ' ';
        ++indice;
    }
   /// out << '\n';



    /// calculate relations
    for( int ind_a = 0;  ind_a < (int)hotspotList.size();   ++ind_a ) {
        order.push_back(ind_a);
        for( int ind_b = ind_a + 1;  ind_b < (int)hotspotList.size();  ++ind_b ) {
            if( ind_a == 3 && ind_b == 8 ) {
                cout << "meh";
            }
            if( checkRelation(hotspotList[ind_a], hotspotList[ind_b]) ) {
                goodRelations.push_back({ind_a, ind_b});
                G[ind_a].push_back(ind_b);
                G[ind_b].push_back(ind_a);
            }

        }
    }

    /// afis relations
    for( auto &el : goodRelations )
        if( el.first > el.second )
            swap(el.first, el.second);
    sort(goodRelations.begin(), goodRelations.end());
 /*   for( auto el : goodRelations )
      ///  out << el.first << ' ' << el.second << ' ';*/

    int apare[NMAX+2];
    bool found_solution = 0;
  ///  srand(time(0));

    while( !found_solution ) {
        std::srand ( unsigned ( std::time(0) ) );
        random_shuffle(order.begin(), order.end());
        memset(apare, 0, sizeof(apare));
        int left_out = 0;
        for( int i = 0; i < min(maxCount, (int)order.size());  ++i ) {
            apare[ order[i] ] = 1;
        }
        for( int i = 0; i < (int)order.size();  ++i ) {
            if( apare[i] ) continue;
            int has_team = 0;
            for( auto x : G[i] )
                if(apare[x] )
                    has_team = 1;
            if( has_team == 0 )
                ++left_out;
        }
        cout << "I managed to left out of: " << left_out << '\n';
        if( left_out == 0 ) {
            break;
        }
    }

    for( int i = 0; i < min(maxCount, (int)order.size());  ++i )
        out << order[i] << ' ';

    return 0;
}
