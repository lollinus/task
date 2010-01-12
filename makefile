CXX=g++
INCLUDE=-I$(HOME)/include -I/usr/local/include -I/opt/sysincludes/linux64/ix86/sysincludes_1.3/usr/include -I../logger
LIBS_PATH=-L/opt/sysincludes/linux64/ix86/sysincludes_1.3/usr/lib64 -L/usr/local/lib
LIBS=-lboost_thread 
OBJECTS=task_manager.o
FLAGS=
CXXFLAGS=
LDDFLAGS=

task_manager_test: test.o $(OBJECTS)
	$(CXX) -o $@ $(LIBS_PATH) $(LIBS) $(FLAGS) $(LDDFLAGS) $< $(OBJECTS)

%_run: %
	@./$^

.PHONY: clean
clean:
	@rm -rf *.core *.o task_manager_test

%.o: %.cpp %.hpp
	$(CXX) -c -o $@ $(INCLUDE) $(CXXFLAGS) $(FLAGS) $< 

%.o: %.cpp
	$(CXX) -c -o $@ $(INCLUDE) $(CXXFLAGS) $(FLAGS) $< 

