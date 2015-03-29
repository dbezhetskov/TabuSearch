#-------------------------------------------------
#
# Project created by QtCreator 2014-03-01T02:39:21
#
#-------------------------------------------------

QMAKE_CXXFLAGS += -std=c++11
QMAKE_CFLAGS_RELEASE += -O2

LIBS += -pthread -Wl,--no-as-needed

SOURCES += main.cpp \
    Solution/VectorSolution.cpp \
    InitialStandardData/TaskData.cpp \
    TabuList/HashSetTabuList.cpp \
    AspirationCriteria/BestEverAspirationCriteria.cpp \
    Neighborhood/RandomMoveNeighborhood.cpp \
    Neighborhood/MoveNeighborhood.cpp \
    Move/SimpleMove.cpp \
    Move/CompoundMove.cpp \
    Neighborhood/SwapNeighborhood.cpp \
    Neighborhood/RandomSwapNeighborhood.cpp \
    TabuSearchStrategy/SimpleTabuSearchReverse.cpp \
    TabuSearchStrategy/SimpleTabuSearchReverse-impl.cpp

HEADERS += \
    Move/IMove.hpp \
    Neighborhood/INeighborhood.hpp \
    Solution/ISolution.hpp \
    Solution/VectorSolution.hpp \
    TabuSearchStrategy/SimpleTabuSearch.hpp \
    InitialStandardData/TaskData.hpp \
    TabuList/ITabuList.hpp \
    TabuList/HashSetTabuList.hpp \
    TabuSearchStrategy/SimpleTabuSearch-inl.hpp \
    AspirationCriteria/IAspirationCriteria.hpp \
    AspirationCriteria/BestEverAspirationCriteria.hpp \
    Neighborhood/RandomMoveNeighborhood.hpp \
    Neighborhood/MoveNeighborhood.hpp \
    Move/SimpleMove.hpp \
    Move/CompoundMove.hpp \
    Neighborhood/SwapNeighborhood.hpp \
    Neighborhood/RandomSwapNeighborhood.hpp \
    TabuSearchStrategy/ITabuSearch.hpp \
    TabuSearchStrategy/SimpleAssignmentProblemTabuSearch.hpp \
    TabuSearchStrategy/SimpleAssignmentProblemTabuSearch-inl.hpp \
    Neighborhood/UnionNeighborhood.hpp \
    TabuSearchStrategy/ParallelTabuSearch-inl.hpp \
    TabuSearchStrategy/ParallelTabuSearch.hpp \
    Scheduler/Scheduler.hpp \
    Scheduler/Scheduler-inl.hpp \
    Scheduler/FunctionWrapper.hpp \
    TabuSearchStrategy/LandingTabuSearch.hpp \
    TabuSearchStrategy/LandingTabuSearch-inl.hpp \
    Neighborhood/UnionNeighborhood-inl.hpp \
    Utils/TwoDimensionalMatrix.hpp \
    Utils/ThreeDimensionalMatrix.hpp \
    TabuSearchStrategy/SimpleTabuSearchReverse.hpp
