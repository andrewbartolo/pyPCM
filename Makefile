PCM_DIR = /file0/bartolo/build-new/pcm

all:
	#gcc -O pcm_test.cpp -o pcm_test --std=c++11 -lstdc++ -lpthread -L $(PCM_DIR)/cpucounters.o $(PCM_DIR)/msr.o $(PCM_DIR)/pci.o -I $(PCM_DIR)

	g++ -std=c++11 -I $(PCM_DIR) -L $(PCM_DIR)/cpucounters.o $(PCM_DIR)/pci.o $(PCM_DIR)/msr.o $(PCM_DIR)/client_bw.o $(PCM_DIR)/cpucounters.o -lpthread  pcm_test.cpp -o pcm_test

clean:
	rm pcm_test
