#ifndef OPENCVTEST_HELPER_H
#define OPENCVTEST_HELPER_H

#include <string>
#include <vector>

namespace PR {

    struct Point {
        Point() {
            x = 0;
            y = 0;
        }

        Point(int _x, int _y) {
            x = _x;
            y = _y;
        }

        bool operator==(PR::Point _p2) {
            return x == _p2.x && y == _p2.y;
        }

        int x;
        int y;
    };

    class Object {
    public:
        Object() { }

        Object(std::string _id) {
            m_ID = _id;
        }

        Object(std::vector<PR::Point> _points){
            m_Points = _points;
        }

        bool contains(PR::Point _p) {
            for (int i = 0; i < m_Points.size(); i++) {
                if (m_Points[i] == _p)
                    return true;
            }
            return false;
        }

        std::vector<PR::Point> &points() {
            return m_Points;
        }

        cv::Mat img(){
            cv::Mat out(cv::Size(getWidth(),getHeight()),CV_8UC1);

            int lS = leftStart();
            int tS = topStart();

            out = cv::Scalar(255);

            for(int i = 0; i < m_Points.size(); i++){
                out.at<uchar>(cv::Point(m_Points[i].x-lS,m_Points[i].y-tS)) = 0;
            }

            return out;
        }

        std::string id() {
            return m_ID;
        }

        int leftStart(){
            int min = m_Points[0].x;

            for(int i = 1; i < m_Points.size(); i++){
                if(m_Points[i].x < min)
                    min = m_Points[i].x;
            }

            return min;
        }

        int topStart(){
            int min = m_Points[0].y;

            for(int i = 1; i < m_Points.size(); i++){
                if(m_Points[i].y < min)
                    min = m_Points[i].y;
            }

            return min;
        }

        int getWidth(){
            int min = m_Points[0].x, max = m_Points[0].x;

            for(int i = 1; i < m_Points.size(); i++){
                if(m_Points[i].x < min)
                    min = m_Points[i].x;
                if(m_Points[i].x > max)
                    max = m_Points[i].x;
            }

            return max - min + 1;
        }

        int getHeight(){
            int min = m_Points[0].y, max = m_Points[0].y;

            for(int i = 1; i < m_Points.size(); i++){
                if(m_Points[i].y < min)
                    min = m_Points[i].y;
                if(m_Points[i].y > max)
                    max = m_Points[i].y;
            }

            return max - min + 1;
        }

    private:
        std::string m_ID;
        std::vector <PR::Point> m_Points;
    };

    class Pattern {
    public:
        Pattern() {
            m_ID = "";
        }

        Pattern(std::string _id) {
            m_ID = _id;
            for(int y = 0; y < 100; y++) {
                std::vector<int> v;
                m_PointValues.push_back(v);
                for(int x = 0; x < 100; x++) {
                    m_PointValues[y].push_back(0);
                }
            }
        }

        bool operator==(std::string _id) {
            return m_ID == _id;
        }

        std::vector<std::vector<int>> &getPointValues(){
            return m_PointValues;
        }

        int getMaxValue(){
            int max = 0;
            for(int y = 0; y < 100; y++) {
                for(int x = 0; x < 100; x++) {
                    if(m_PointValues[y][x] > max)
                        max = m_PointValues[y][x];
                }
            }
            return max;
        }

        std::string getID() {
            return m_ID;
        }

        int getWidth(){
            return m_PointValues[0].size();
        }

        int getHeight(){
            return m_PointValues.size();
        }

    private:
        std::string m_ID;
        std::vector<std::vector<int>> m_PointValues;
    };
}
#endif