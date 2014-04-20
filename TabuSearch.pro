#-------------------------------------------------
#
# Project created by QtCreator 2014-03-01T02:39:21
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = TabuSearch
CONFIG   += console
CONFIG   -= app_bundle

QMAKE_CXXFLAGS += -std=c++11
QMAKE_CFLAGS_RELEASE    = -O2

TEMPLATE = app


SOURCES += main.cpp \
    Solution/VectorSolution.cpp \
    InitialStandardData/TaskData.cpp \
    TabuList/HashSetTabuList.cpp \
    AspirationCriteria/BestEverAspirationCriteria.cpp \
    Neighborhood/RandomMoveNeighborhood.cpp \
    Neighborhood/MoveNeighborhood.cpp \
    Move/SimpleMove.cpp \
    Neighborhood/RandomAlignmentNeighborhood.cpp \
    Move/CompoundMove.cpp \
    Neighborhood/SwapNeighborhood.cpp \
    Neighborhood/RandomSwapNeighborhood.cpp \
    Neighborhood/UnionNeighborhood.cpp \
    Neighborhood/AssignmentProblem/RandomGetterServers.cpp \
    Neighborhood/AssignmentProblem/RandomGetterDisks.cpp \
    Neighborhood/AssignmentProblem/RandomOneDiskByServerGetter.cpp

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
    Move/MoveNeighborhood.hpp \
    Move/SimpleMove.hpp \
    Neighborhood/RandomAlignmentNeighborhood.hpp \
    Move/CompoundMove.hpp \
    Neighborhood/AssignmentProblem/AssignmentProblemRandomNeighborhood.hpp \
    Neighborhood/SwapNeighborhood.hpp \
    Neighborhood/RandomSwapNeighborhood.hpp \
    Neighborhood/IChangeableSizeNeighborhood.hpp \
    TabuSearchStrategy/SimpleAssignmentProblemTabuSearch.hpp \
    TabuSearchStrategy/SimpleAssignmentProblemTabuSearch-inl.hpp \
    Neighborhood/UnionNeighborhood.hpp \
    Neighborhood/AssignmentProblem/AssignmentProblemRandomNeighborhood-inl.hpp \
    Neighborhood/AssignmentProblem/RandomGetterServers.hpp \
    Neighborhood/AssignmentProblem/IGetterServers.hpp \
    Neighborhood/AssignmentProblem/IGetterDisks.hpp \
    Neighborhood/AssignmentProblem/RandomGetterDisks.hpp \
    Neighborhood/AssignmentProblem/RandomOneDiskByServerGetter.hpp
