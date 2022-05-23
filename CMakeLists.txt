cmake_minimum_required(VERSION 3.13)
project(plume_experiments)

set(CMAKE_CXX_STANDARD 11)

find_package(Boost REQUIRED)

add_executable(test test.cpp Ellipse.cpp Ellipse.h Point.cpp Point.h PointUtil.cpp PointUtil.h Line.cpp Line.h LineSegment.cpp LineSegment.h PointStrategy.cpp PointStrategy.h SearchStrategy.h ChordStrategy.cpp ChordStrategy.h SpokeStrategy.cpp SpokeStrategy.h JohnSpokeStrategy.cpp JohnSpokeStrategy.h RandomUtil.cpp RandomUtil.h SearchStrategy.cpp SearchStrategyFactory.h TSP.cpp TSP.h KDTree.cpp KDTree.h)

add_executable(convergence convergence.cpp Ellipse.cpp Ellipse.h Point.cpp Point.h Line.cpp Line.h LineSegment.cpp LineSegment.h PointStrategy.cpp PointStrategy.h SearchStrategy.h ChordStrategy.cpp ChordStrategy.h SpokeStrategy.cpp SpokeStrategy.h JohnSpokeStrategy.cpp JohnSpokeStrategy.h RandomUtil.cpp RandomUtil.h SearchStrategy.cpp SearchStrategyFactory.h TSP.cpp TSP.h KDTree.cpp KDTree.h Plume.h CompositePlume.cpp CompositePlume.h)

add_executable(hits hits.cpp Ellipse.cpp Ellipse.h Point.cpp Point.h Line.cpp Line.h LineSegment.cpp LineSegment.h PointStrategy.cpp PointStrategy.h SearchStrategy.h ChordStrategy.cpp ChordStrategy.h SpokeStrategy.cpp SpokeStrategy.h JohnSpokeStrategy.cpp JohnSpokeStrategy.h RandomUtil.cpp RandomUtil.h SearchStrategy.cpp SearchStrategyFactory.h TSP.cpp TSP.h KDTree.cpp KDTree.h)

add_executable(manhattan manhattan.cpp Ellipse.cpp Ellipse.h Point.cpp Point.h Line.cpp Line.h LineSegment.cpp LineSegment.h Plume.h CompositePlume.h CompositePlume.cpp)

add_executable(sketch sketch.cpp Ellipse.cpp Ellipse.h Point.cpp Point.h PointUtil.cpp PointUtil.h Line.cpp Line.h LineSegment.cpp LineSegment.h Plume.h CompositePlume.h CompositePlume.cpp)
