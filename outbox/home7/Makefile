EXENAME = rk4
CXX = g++
CPPFLAGS = -std=c++11
# CPPFLAGS += -Isrc
CPPFLAGS += -O3
STRIP_COMMAND = true
# LDFLAGS  = -llapack -llapacke

$(EXENAME): clean main.o 
	$(CXX) $(CPPFLAGS) -o $(EXENAME) main.o $(LDFLAGS) 
	$(STRIP_COMMAND) $(EXENAME)

all: $(EXENAME)
	 
clean:
	rm -f $(EXENAME) *.o