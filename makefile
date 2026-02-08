# makefile for ConvolVer Chugin

CHUGIN_NAME=ConvolVer

# source files
CXXS+=ConvolVer.cpp FFTConvolver.cpp AudioFFT.cpp Utilities.cpp

# flags
FLAGS+=-DFFTCONVOLVER_USE_SSE

# OS X: build universal binary
ARCHS?=x86_64 arm64

top_srcdir=../chugins_repo
CK_SRC_PATH?=$(top_srcdir)/chuck/include

include makefile.mac

default:
	@echo "Type 'make osx', 'make linux', or 'make win32'"

osx: 
	$(CXX) $(FLAGS) $(LDFLAGS) -o $(CHUGIN_NAME).chug $(CXXS)
	
install:
	@echo "Installing to global chugins..."
	mkdir -p /usr/local/lib/chuck/
	cp $(CHUGIN_NAME).chug /usr/local/lib/chuck/
	@echo "Done!"
