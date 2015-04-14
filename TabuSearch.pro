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
    TabuSearchStrategy/SimpleTabuSearchReverse-impl.cpp \
    TabuSearchStrategy/LandingTabuSearch.cpp \
    TabuSearchStrategy/LandingTabuSearch-impl.cpp \
    Scheduler/Scheduler.cpp \
    TabuSearchStrategy/ParallelTabuSearch.cpp \
    TabuSearchStrategy/ParallelTabuSearch-impl.cpp \
    TabuSearchStrategy/SimpleTabuSearch.cpp \
    Improvements/LinKernighan.cpp \
    TabuSearchStrategy/SimpleTabuSearch-impl.cpp \
    Neighborhood/UnionNeighborhood.cpp \
    Neighborhood/UnionNeighborhood-impl.cpp \
    Improvements/linkernighan-impl.cpp \
    Reduction/RandomReduction.cpp \
    Reduction/RandomReduction-impl.cpp

HEADERS += \
    Move/IMove.hpp \
    Neighborhood/INeighborhood.hpp \
    Solution/ISolution.hpp \
    Solution/VectorSolution.hpp \
    TabuSearchStrategy/SimpleTabuSearch.hpp \
    InitialStandardData/TaskData.hpp \
    TabuList/ITabuList.hpp \
    TabuList/HashSetTabuList.hpp \
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
    TabuSearchStrategy/ParallelTabuSearch.hpp \
    Scheduler/Scheduler.hpp \
    Scheduler/FunctionWrapper.hpp \
    TabuSearchStrategy/LandingTabuSearch.hpp \
    Utils/TwoDimensionalMatrix.hpp \
    Utils/ThreeDimensionalMatrix.hpp \
    TabuSearchStrategy/SimpleTabuSearchReverse.hpp \
    Improvements/IImprovement.hpp \
    Improvements/LinKernighan.hpp \
    Reduction/IReduction.hpp \
    Reduction/RandomReduction.hpp
