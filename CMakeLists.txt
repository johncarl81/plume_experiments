cmake_minimum_required(VERSION 3.13)
project(plume_experiments)

set(CMAKE_CXX_STANDARD 11)

add_executable(convergence convergence.cpp Ellipse.cpp Ellipse.h Point.cpp Point.h Line.cpp Line.h LineSegment.cpp LineSegment.h PointStrategy.cpp PointStrategy.h SearchStrategy.h ChordStrategy.cpp ChordStrategy.h SpokeStrategy.cpp SpokeStrategy.h JohnSpokeStrategy.cpp JohnSpokeStrategy.h RandomUtil.cpp RandomUtil.h SearchStrategy.cpp SearchStrategyFactory.h)

add_executable(hits hits.cpp Ellipse.cpp Ellipse.h Point.cpp Point.h Line.cpp Line.h LineSegment.cpp LineSegment.h PointStrategy.cpp PointStrategy.h SearchStrategy.h ChordStrategy.cpp ChordStrategy.h SpokeStrategy.cpp SpokeStrategy.h JohnSpokeStrategy.cpp JohnSpokeStrategy.h RandomUtil.cpp RandomUtil.h SearchStrategy.cpp SearchStrategyFactory.h)