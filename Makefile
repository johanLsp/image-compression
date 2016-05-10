CPPFLAGS =-g --std=c++11 -Wall -Wno-write-strings

BUILD_DIR = build


define build
    addprefix build/ $(1)
endef

#$(addprefix build/, compression.o Encoder/EZW.o Encoder/EBCOT.o haar.o)
all: bin/compression

bin/compression: $(addprefix build/, compression.o encoder/EZW.o encoder/EBCOT.o wavelet/Haar.o wavelet/DB97.o)
	g++ $(CPPFLAGS) $^ -o $@ 

#compression:
#	cd src;\
#	g++ $(CPPFLAGS) -o compression compression.cpp ezw.cpp haar.cpp db97.cpp ebcot.cpp;\
#	mv compression ../bin/compression;
		
#.SECONDEXPANSION

#$(BUILD_DIR)/compression.o: src/compression.cpp
#	g++ $(CXXFLAGS) -c  $^

#$(BUILD_DIR)/Encoder/EZW.o:  src/Encoder/EZW.cpp src/Encoder/EZW.h
#	g++ $(CXXFLAGS) -c  $^
	
#$(BUILD_DIR)/Encoder/EBCOT.o:  src/Encoder/EBCOT.cpp src/Encoder/EBCOT.h
#	g++ $(CXXFLAGS) -c  $^
	
#$(BUILD_DIR)/haar.o:  src/haar.cpp src/haar.h
#	g++ $(CXXFLAGS) -c  $^

build/%.o : src/%.cpp
	@mkdir -p $(dir $@)
	g++ $(CPPFLAGS) -c -o $@ $^
	
.PHONY clean:
	rm -rf $(BUILD_DIR)/*
	
