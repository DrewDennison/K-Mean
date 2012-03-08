import random

debug = False

class Point:
    def __init__(self, x, y):
        self.x = float(x)
        self.y = float(y)

    def getX(self):
        return self.x

    def getY(self):
        return self.y

    def getXY(self):
        return (self.x, self.y)

class Cluster:
    def __init__(self, points):
        self.points = points
        x = 0.0 # want floating point precision for divison
        y = 0.0

        # average x and y
        for p in self.points:
            x += p.getX()
            y += p.getY()
        x = x/len(points)
        y = y/len(points)
        self.center = Point(x, y)
        
    def getCenter(self):
        return self.center

    def updateCenter(self):
        x = 0.0 # want floating point precision for divison
        y = 0.0

        # average x and y
        for p in self.points:
            x += p.getX()
            y += p.getY()
        x = x/len(self.points)
        y = y/len(self.points)
        self.center = Point(x, y)
        return self.center
       
    def addPoint(self, point):
        self.points.extend([point])

    def distToCenter(self, point):
        return ((point.getX()-self.getCenter().getX())**2 + (point.getY()-self.getCenter().getY())**2)**0.5

    def getRemovePoints(self):
        points = self.points
        self.points = []
        return points

    def getPoints(self):
        return self.points
    

def kmeans(listOfPoints, k, numIter):    
    # randomly shuffle the elements
    random.shuffle(listOfPoints)

    if debug:
        print 'list of points', [p.getXY() for p in listOfPoints]

    # pick the first k elements to be the centroids of the k clusters

    clusters = [Cluster([listOfPoints[i]]) for i in range(k)] # k new clusters with 1 random point each

    if debug:
        prtClusters(clusters)

    # assign each remaining entry to the cluster that has the closest centroid
    for p in listOfPoints[k:]:
        addToClosest(clusters, p)

    if debug:
        prtClusters(clusters)
        
    # Now we run the main loop of the algorithm
    for i in range(numIter):
        points = [] # list of all points in all clusters
        for c in clusters:
            c.updateCenter() # recalculate centroid of each cluster
            points.extend(c.getRemovePoints()) # remove all points
        for p in points:
            addToClosest(clusters, p)

    # all done
    return clusters
            
        
    
def addToClosest(listOfClusters, point):
    # add point to the cluster with the closest centroid
    listOfClusters[min((c.distToCenter(point), i) for i, c in enumerate(listOfClusters))[1]].addPoint(point)


def prtClusters(listOfClusters):
    index = 0
    for x in listOfClusters:
        print 'cluster', index
        index += 1
        print 'points', [p.getXY() for p in x.getPoints()]
        print 'center', x.getCenter().getXY()
        
        
    

points = [Point(random.randint(0, 100), random.randint(0, 100)) for i in range(100)]

print [p.getXY() for p in points]

result = kmeans(points, 6, 15)

prtClusters(result)
