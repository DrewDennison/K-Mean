#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <vector>
#include <iostream>

using namespace std;

class Point2D {
public:
    Point2D() {};
    Point2D(double px, double py) { x = px; y = py; };
    double x;
    double y;
};

class Cluster {
public:
    Cluster() {};
    Cluster(Point2D initialPoints) { points.push_back(initialPoints); UpdateCenter(); };
    Cluster(vector<Point2D> initialPoints) { points = initialPoints; UpdateCenter(); };
    Point2D GetCenter() { return this->center; };
    void SetCenter(Point2D c) { this->center = c; };
    Point2D UpdateCenter() { 
	double x = 0; double y = 0; 
	for (int i = 0; i < this->points.size(); i++) { 
	    x += points[i].x;
	    y += points[i].y;
	}
	this->center = Point2D(x / points.size(), y / points.size()); 
    };
    void AddPoint(Point2D p) { points.push_back(p); };
    double DistToCenter(Point2D p) { 
	double a = p.x - this->GetCenter().x; 
	double b = p.y - this->GetCenter().y; 
	return sqrt(a * a + b * b);
    };
    vector<Point2D> RemovePoints() { this->points.clear(); };
    vector<Point2D> GetPoints() { return this->points; };
private:
    vector<Point2D> points;
    Point2D center;
};

void AddToClosestCentroid(vector<Cluster> & clusters, Point2D p) {
    int minD = -1;
    int minC = -1;
    for (int c = 0; c < clusters.size(); c++) {
	if (minD == -1 || clusters[c].DistToCenter(p) < minD) {
	    minD = clusters[c].DistToCenter(p);
	    minC = c;
	}
    }
    cout << "closest is " << " at " << minD << endl;
    clusters[minC].AddPoint(p);
}

vector<Cluster> kMeans(vector<Point2D> points, int k, int iters) { 
    vector<Cluster> clusters;
    for (int i = 0; i < k; i++) {
	clusters.push_back(Cluster(points[i]));
    }

    // for each point, add it to the cluster with the closest centroid
    for (int p = k; p < points.size(); p++) {	
	AddToClosestCentroid(clusters, points[p]);
    }
    
    for (int i = 0; i < iters; i++) {
	vector<Point2D> points;

	// recalculate the centroid of each cluster
	for (int j = 0; j < clusters.size(); j++) {
	    clusters[j].UpdateCenter();
	    vector<Point2D> tPoints = clusters[j].GetPoints();
	    cout << "cluster " << j << " has " << tPoints.size() << " points" << endl;
	    for (int t = 0; t < tPoints.size(); t++) {
		points.push_back(tPoints[t]);
	    }
	    Point2D ccc = clusters[j].GetCenter();
	    clusters[j] = Cluster();
	    clusters[j].SetCenter(ccc);
	}
		
        // for each point, add it to the cluster with the closest centroid
	for (int p = 0; p < points.size(); p++) {	
	    AddToClosestCentroid(clusters, points[p]);
	    cout << " added point " << p << " to cluster " << endl;
	}
	
    }
    return clusters;
}

int main(int argc, char *argv[]) {

    srand (time(NULL));

    vector<Point2D> ps;
    for (int i = 0; i < 100; i++) {
	ps.push_back(Point2D(rand() % 100, rand() % 100));
	cout << ps.back().x << endl;
    }

    vector<Cluster> cs = kMeans(ps, 5, 4);

    for (int c = 0; c < cs.size(); c++) {
	cout << "cluster " << c << endl;
	vector <Point2D> pts = cs[c].GetPoints();
	for (int p = 0; p < pts.size(); p++) {	
	    cout << "(" << pts[p].x << ", " << pts[p].y << ")";
	}
    }

    return 0;
}
