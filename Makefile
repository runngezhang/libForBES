# LIBFORBES MAKEFILE
	
# Licence note:
#
# ForBES is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#   
# ForBES is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU Lesser General Public License for more details.
# 
# You should have received a copy of the GNU Lesser General Public License
# along with ForBES. If not, see <http://www.gnu.org/licenses/>.


#						 #
# Do not modify this file			 #
#						 #


include config.mk

# Set this to 1 in order to generate a coverage report
DO_PROFILE := 0
DO_PARALLEL := 1
NPROCS := 1
# Enable parallel make on N-1 processors	
ifeq (1, $(DO_PARALLEL))
	NPROCS := 1
	OS:=$(shell uname -s)
	ifeq ($(OS),Linux)
	    NPROCS:=$(shell grep -c ^processor /proc/cpuinfo)
	endif
	ifeq ($(OS),Darwin) # Assume Mac OS X
	    NPROCS:=$(shell sysctl -n hw.ncpu)
	endif
	NPROCS:=$$(($(NPROCS)-1))	
	MAKEFLAGS += -j $(NPROCS)
	MAKEFLAGS += --no-print-directory
endif

# C++ compiler
ifeq (1, $(DO_PROFILE))
    CXX = g++-4.9
else
    CXX = g++-4.8
endif

# Enable CCACHE
ifneq (, $(shell which ccache))
	CXX := ccache $(CXX)
endif

# Additional compiler flags (e.g., -O2 or -O3 optimization flags, etc)
# To create a test coverage report add: -fprofile-arcs -ftest-coverage
CFLAGS_ADDITIONAL = -O0	
ifeq (1, $(DO_PROFILE))
	CFLAGS_ADDITIONAL += -fprofile-arcs 
	CFLAGS_ADDITIONAL += -ftest-coverage
endif

CFLAGS_WARNINGS = \
	-pedantic \
	-Wall \
	-Wextra \
	-Wcast-align \
	-Wcast-qual \
	-Wdisabled-optimization \
	-Wformat=2 \
	-Winit-self \
	-Wlogical-op \
	-Wmissing-declarations \
	-Wmissing-include-dirs \
	-Wnoexcept \
	-Wold-style-cast \
	-Woverloaded-virtual \
	-Wredundant-decls \
	-Wshadow \
	-Wsign-promo \
	-Wstrict-null-sentinel \
	-Wstrict-overflow=5 \
	-Wswitch-default \
	-Wundef \
	-Wno-unused \
	-Wno-sign-compare
	
#CFLAGS_WARNINGS += -Werror
	
CFLAGS_ADDITIONAL += ${CFLAGS_WARNINGS}

# Additional link flags
# To create a test coverage report add: -fprofile-arcs
ifeq (1, $(DO_PROFILE))
    LFLAGS_ADDITIONAL = -fprofile-arcs
endif


OBJ_DIR = build/Debug
BIN_DIR = dist/Debug

OBJ_TEST_DIR = ${OBJ_DIR}/Test
BIN_TEST_DIR = ${BIN_DIR}/Test

TEST_DIR = source/tests

ARCHIVE = ${BIN_DIR}/libforbes.a

CFLAGS = -c -DUSE_LIBS $(CFLAGS_ADDITIONAL) $(CEXTRA)

IFLAGS = \
	-I. \
	-I./source \
	-I$(SS_DIR)/CHOLMOD/Include \
	-I$(SS_DIR)/LDL/Include \
	-I$(SS_DIR)/SuiteSparse_config \
	-I/usr/local/include \
	-I$(IEXTRA)

UNAME = $(shell uname -s)
ifeq ($(UNAME),Linux)
	# From https://github.com/xianyi/OpenBLAS/wiki/faq
	# On Linux, if OpenBLAS was compiled with threading support 
	# (USE_THREAD=1 by default), custom programs statically linked against 
	# libopenblas.a should also link to the pthread library
	LFLAGS_ADDITIONAL += -lrt -lpthread
endif

lFLAGS = \
	-lldl \
	-lcholmod \
	-lamd \
	-lcolamd \
	-lsuitesparseconfig \
	-lccolamd \
	-lcamd \
	-llapacke \
	-llapack \
	-lopenblas \
	-lm \
	-lcppunit \
	$(LFLAGS_ADDITIONAL)

ifeq ($(DO_PROFILE),0)
    #lFLAGS += -lgfortran
endif

LFLAGS = \
	-L$(SS_DIR)/CHOLMOD/Lib \
	-L$(SS_DIR)/AMD/Lib \
	-L$(SS_DIR)/COLAMD/Lib \
	-L$(SS_DIR)/SuiteSparse_config \
	-L$(SS_DIR)/CCOLAMD/Lib \
	-L$(SS_DIR)/CAMD/Lib \
	-L$(SS_DIR)/LDL/Lib \
	-L/usr/local/lib \
	-L$(LEXTRA)

# LINEAR OPERATORS
SOURCES = LinearOperator.cpp \
	MatrixOperator.cpp \
	OpAdjoint.cpp \
	OpComposition.cpp \
	OpDCT2.cpp \
	OpDCT3.cpp \
	OpReverseVector.cpp \
	OpGradient.cpp \
	OpLTI.cpp
		
	
# SOLVERS FOR LINEAR SYTEMS Ax=b AND T(x) = b
SOURCES += CGSolver.cpp \
	LinOpSolver.cpp \
	MatrixSolver.cpp \
	LinSysSolver.cpp \
	S_LDLFactorization.cpp \
	CholeskyFactorization.cpp \
	FactoredSolver.cpp \
	LDLFactorization.cpp \
	Properties.cpp

# FORBES UTILITIES	
SOURCES += ForBESUtils.cpp \
	SVDHelper.cpp \
	FunctionOntologicalClass.cpp \
	FunctionOntologyRegistry.cpp


# MATRIX & MATRIX UTILITIES
SOURCES += Matrix.cpp \
	MatrixWriter.cpp \
	MatrixFactory.cpp

# FUNCTIONS
SOURCES += Function.cpp \
	IndBox.cpp \
	IndSOC.cpp \
	IndPos.cpp \
	IndProbSimplex.cpp \
	QuadOverAffine.cpp \
	QuadraticOperator.cpp \
	Quadratic.cpp \
	DistanceToBox.cpp \
	DistanceToBall2.cpp \
	ElasticNet.cpp \
	LogLogisticLoss.cpp \
	QuadraticLoss.cpp \
	HingeLoss.cpp \
	HuberLoss.cpp \
	ConjugateFunction.cpp \
	Norm.cpp \
	Norm1.cpp \
	Norm2.cpp \
	SeparableSum.cpp \
	IndBall2.cpp \
	QuadraticLossOverAffine.cpp \
	SumOfNorm2.cpp \
	LQCost.cpp
	
# CORE
SOURCES += FBProblem.cpp \
	FBCache.cpp \
	FBSplitting.cpp \
	FBSplittingFast.cpp \
	FBStopping.cpp \
	FBStoppingRelative.cpp \
	LBFGSBuffer.cpp

OBJECTS = $(SOURCES:%.cpp=$(OBJ_DIR)/%.o)

TESTS = \
	TestSVDHelper.test \
	TestSLDL.test \
	TestCholesky.test \
	TestIndBox.test \
	TestIndPos.test \
	TestIndSOC.test \
	TestIndProbSimplex.test \
	TestCGSolver.test \
	TestLDL.test \
	TestMatrix.test \
	TestMatrixFactory.test \
	TestMatrixOperator.test \
	TestOpAdjoint.test \
	TestOpComposition.test \
	TestOpDCT2.test \
	TestOpDCT3.test \
	TestOpGradient.test \
	TestOpReverseVector.test \
	TestQuadOverAffine.test \
	TestQuadratic.test \
	TestQuadraticOperator.test \
	TestOntRegistry.test \
	TestDistanceToBox.test \
	TestDistanceToBall2.test \
	TestElasticNet.test \
	TestQuadraticLoss.test \
	TestLogLogisticLoss.test \
	TestHingeLoss.test \
	TestHuber.test \
	TestNorm1.test \
	TestNorm2.test \
	TestFunctionOntologicalClass.test \
	TestFunctionOntologyRegistry.test \
	TestIndBall2.test \
	TestSeparableSum.test \
	TestConjugateFunction.test \
	TestMatrixExtras.test \
	TestFBCache.test \
	TestFBSplitting.test \
	TestFBSplittingFast.test \
	TestLasso.test \
	TestSumOfNorm2.test \
	TestProperties.test \
	TestLBFGSBuffer.test \
	TestFBProblem.test

TEST_BINS = $(TESTS:%.test=$(BIN_TEST_DIR)/%)

$(ARCHIVE): prop dirs $(OBJECTS)
	@echo "\nArchiving..."
	ar rcs $(ARCHIVE) $(OBJECTS)

all: $(ARCHIVE)

prop-short:
	@echo "--- General ---"
	@echo "OS           :"  $(OS)
	@echo "UNAME        :"  $(UNAME)
	@echo "Processors   :"  $(NPROCS)
	@echo "Profiling    :"  $(DO_PROFILE)
	@echo "Do parallel  :"  $(DO_PARALLEL)
	@echo "C++ compiler :"  $(CXX)
	@echo "\n--- Directories ---"
	@echo "Obj.         :"  ${OBJ_DIR}
	@echo "Bin.         :"  ${BIN_DIR}
	@echo "Test Obj.    :"  ${OBJ_TEST_DIR}
	@echo "Test Bin .   :"  ${BIN_TEST_DIR}
prop: prop-short	
	@echo "\n--- C flags  ---\n" $(CFLAGS)
	@echo "\n--- Includes ---\n" $(IFLAGS)
	@echo "\n--- l flags  ---\n" $(lFLAGS)
	@echo "\n--- L flags  ---\n" $(LFLAGS)
	@echo "\n"
build-tests: $(ARCHIVE) $(TEST_BINS)

test: build-tests
	@echo "\n*** FACTORIZERS/SOLVERS ***"
	${BIN_TEST_DIR}/TestCholesky
	${BIN_TEST_DIR}/TestLDL
	${BIN_TEST_DIR}/TestSLDL
	${BIN_TEST_DIR}/TestCGSolver
	@echo "\n*** FUNCTIONS ***"
	${BIN_TEST_DIR}/TestConjugateFunction
	${BIN_TEST_DIR}/TestQuadOverAffine
	${BIN_TEST_DIR}/TestQuadratic
	${BIN_TEST_DIR}/TestQuadraticOperator
	${BIN_TEST_DIR}/TestIndBox
	${BIN_TEST_DIR}/TestIndPos
	${BIN_TEST_DIR}/TestIndSOC
	${BIN_TEST_DIR}/TestIndProbSimplex
	${BIN_TEST_DIR}/TestNorm1
	${BIN_TEST_DIR}/TestNorm2
	${BIN_TEST_DIR}/TestIndBall2
	${BIN_TEST_DIR}/TestDistanceToBox
	${BIN_TEST_DIR}/TestDistanceToBall2
	${BIN_TEST_DIR}/TestElasticNet
	${BIN_TEST_DIR}/TestQuadraticLoss
	${BIN_TEST_DIR}/TestLogLogisticLoss
	${BIN_TEST_DIR}/TestHingeLoss
	${BIN_TEST_DIR}/TestHuber
	${BIN_TEST_DIR}/TestSeparableSum
	${BIN_TEST_DIR}/TestSumOfNorm2
	@echo "\n*** UTILITIES ***"
	${BIN_TEST_DIR}/TestMatrixFactory
	${BIN_TEST_DIR}/TestMatrixExtras
	${BIN_TEST_DIR}/TestMatrix
	${BIN_TEST_DIR}/TestOntRegistry
	${BIN_TEST_DIR}/TestFunctionOntologicalClass
	${BIN_TEST_DIR}/TestFunctionOntologyRegistry
	${BIN_TEST_DIR}/TestProperties
	${BIN_TEST_DIR}/TestSVDHelper
	@echo "\n*** LINEAR OPERATORS ***"
	${BIN_TEST_DIR}/TestMatrixOperator
	${BIN_TEST_DIR}/TestOpAdjoint
	${BIN_TEST_DIR}/TestOpComposition
	${BIN_TEST_DIR}/TestOpDCT2
	${BIN_TEST_DIR}/TestOpDCT3
	${BIN_TEST_DIR}/TestOpReverseVector	
	${BIN_TEST_DIR}/TestOpGradient
	@echo "\n*** ALGORITHMS ***"
	${BIN_TEST_DIR}/TestFBCache
	${BIN_TEST_DIR}/TestFBProblem
	${BIN_TEST_DIR}/TestLBFGSBuffer
	${BIN_TEST_DIR}/TestFBSplitting
	${BIN_TEST_DIR}/TestFBSplittingFast
	${BIN_TEST_DIR}/TestLasso

$(BIN_TEST_DIR)/%: $(OBJECTS) $(TEST_DIR)/%.cpp $(TEST_DIR)/%Runner.cpp $(TEST_DIR)/%.h
	@echo
	@echo [Compiling $*]
	$(CXX) $(CFLAGS) $(IFLAGS) -o $(OBJ_TEST_DIR)/$*.o $(TEST_DIR)/$*.cpp
	$(CXX) $(CFLAGS) $(IFLAGS) -o $(OBJ_TEST_DIR)/$*Runner.o $(TEST_DIR)/$*Runner.cpp
	@echo
	@echo [Linking $*]
	$(CXX) $(LFLAGS) -L./dist/Debug -o $(BIN_TEST_DIR)/$* $(OBJ_TEST_DIR)/$*.o $(OBJ_TEST_DIR)/$*Runner.o -lforbes $(lFLAGS) `cppunit-config --libs`
	@echo "\n\n\n"

main:
	$(CXX) $(CFLAGS) $(IFLAGS) source/main.cpp 
	$(CXX) $(LFLAGS) -L./dist/Debug main.o -lforbes $(lFLAGS) -o main_run

run: main
	./main_run

$(OBJ_DIR)/%.o: source/%.cpp
	@echo 
	@echo Compiling $*
	$(CXX) $(CFLAGS) $(IFLAGS) $< -o $@
	@echo "\n\n\n"

dirs: $(OBJ_DIR) $(OBJ_TEST_DIR) $(BIN_DIR) $(BIN_TEST_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_TEST_DIR):
	mkdir -p $(OBJ_TEST_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(BIN_TEST_DIR):
	mkdir -p $(BIN_TEST_DIR)

clean:
	@echo Cleaning...
	rm -rf $(OBJ_DIR)/*.o ;
	rm -rf $(OBJ_TEST_DIR)/*.o;
	rm -rf $(BIN_DIR)/*;	


help:
	@echo "Makefile targets for libforbes:\n"
	@echo "make                        - Compiles, links and archives [creates libforbes.a]"
	@echo "make clean                  - Cleans all previously built files"
	@echo "make al                     - Same as make (tests are not built)"
	@echo "make build-tests            - Compiles and links the tests"
	@echo "make test                   - Compiles [if necessary] and runs all tests"
	@echo "make docs                   - Used doxygen to build documentation"
	@echo "make main                   - Compiles and links source/main.cpp (for testing only)"
	@echo "make prop                   - Prints properties of the makefile"
	@echo "make help                   - This help message\n"

docs:
	doxygen forbes.doxygen

.SECONDARY:

.PHONY: clean

