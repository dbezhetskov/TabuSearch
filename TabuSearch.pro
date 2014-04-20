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
QMAKE_CFLAGS_RELEASE    = -O3

TEMPLATE = app


SOURCES += main.cpp \
    VectorSolution.cpp \
    TaskData.cpp \
    HashSetTabuList.cpp \
    BestEverAspirationCriteria.cpp \
    RandomMoveNeighborhood.cpp \
    MoveNeighborhood.cpp \
    SimpleMove.cpp \
    RandomAlignmentNeighborhood.cpp \
    CompoundMove.cpp \
    SwapNeighborhood.cpp \
    RandomSwapNeighborhood.cpp \
    UnionNeighborhood.cpp \
    RandomGetterServers.cpp \
    RandomGetterDisks.cpp \
    RandomOneDiskByServerGetter.cpp

HEADERS += \
    IMove.hpp \
    INeighborhood.hpp \
    ISolution.hpp \
    VectorSolution.hpp \
    SimpleTabuSearch.hpp \
    TaskData.hpp \
    ITabuList.hpp \
    HashSetTabuList.hpp \
    SimpleTabuSearch-inl.hpp \
    IAspirationCriteria.hpp \
    BestEverAspirationCriteria.hpp \
    RandomMoveNeighborhood.hpp \
    MoveNeighborhood.hpp \
    SimpleMove.hpp \
    RandomAlignmentNeighborhood.hpp \
    CompoundMove.hpp \
    AssignmentProblemRandomNeighborhood.hpp \
    SwapNeighborhood.hpp \
    RandomSwapNeighborhood.hpp \
    IChangeableSizeNeighborhood.hpp \
    MixTwoNeighborhoodOneTabuListTabuSearch.hpp \
    MixTwoNeighborhoodOneTabuListTabuSearch-inl.hpp \
    SimpleAssignmentProblemTabuSearch.hpp \
    SimpleAssignmentProblemTabuSearch-inl.hpp \
    UnionNeighborhood.hpp \
    AssignmentProblemRandomNeighborhood-inl.hpp \
    RandomGetterServers.hpp \
    IGetterServers.hpp \
    IGetterDisks.hpp \
    RandomGetterDisks.hpp \
    RandomOneDiskByServerGetter.hpp
