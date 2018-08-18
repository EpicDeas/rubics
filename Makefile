ifndef CPPC
	CPPC=g++
endif

CCFLAGS=-std=c++14 -O2

LIBS = -lOpenCL -lrt

rubics: src/rubics.cpp src/opencl_manager.cpp src/configuration.cpp
	$(CPPC) $^ $(CCFLAGS) $(LIBS) -I include -o $@

clean:
	rm -f rubics
