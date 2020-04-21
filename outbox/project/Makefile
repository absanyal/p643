EXENAME = sf
CXX = g++
CPPFLAGS = -std=c++11
CPPFLAGS += -O3
CPPFLAGS += -Isrc
STRIP_COMMAND = true
LDFLAGS  = -llapack -llapacke

$(EXENAME): clean sftriangle.o 
	$(CXX) $(CPPFLAGS) -o $(EXENAME)  sftriangle.o $(LDFLAGS) 
	$(STRIP_COMMAND) $(EXENAME)

all: $(EXENAME)
	 
clean:
	rm -f $(EXENAME) *.o