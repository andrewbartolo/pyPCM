PCM_DIR = /file0/bartolo/build-new/pcm

all:
	#gcc -O pcm_test.cpp -o pcm_test --std=c++11 -lstdc++ -lpthread -L $(PCM_DIR)/cpucounters.o $(PCM_DIR)/msr.o $(PCM_DIR)/pci.o -I $(PCM_DIR)

	g++ -std=c++11 -I $(PCM_DIR) -L $(PCM_DIR)/cpucounters.o $(PCM_DIR)/pci.o $(PCM_DIR)/msr.o $(PCM_DIR)/client_bw.o $(PCM_DIR)/cpucounters.o -lpthread  pcm_test.cpp -o pcm_test


	g++ -std=c++11 -I $(PCM_DIR) -L $(PCM_DIR)/cpucounters.o $(PCM_DIR)/pci.o $(PCM_DIR)/msr.o $(PCM_DIR)/client_bw.o $(PCM_DIR)/cpucounters.o -lpthread  mt.cpp -o mt

	g++ -O0 -std=c++11 -I $(PCM_DIR) -L $(PCM_DIR)/cpucounters.o $(PCM_DIR)/pci.o $(PCM_DIR)/msr.o $(PCM_DIR)/client_bw.o $(PCM_DIR)/cpucounters.o -lpthread  mt.cpp -o mt_O0
	g++ -O1 -std=c++11 -I $(PCM_DIR) -L $(PCM_DIR)/cpucounters.o $(PCM_DIR)/pci.o $(PCM_DIR)/msr.o $(PCM_DIR)/client_bw.o $(PCM_DIR)/cpucounters.o -lpthread  mt.cpp -o mt_O1
	g++ -O2 -std=c++11 -I $(PCM_DIR) -L $(PCM_DIR)/cpucounters.o $(PCM_DIR)/pci.o $(PCM_DIR)/msr.o $(PCM_DIR)/client_bw.o $(PCM_DIR)/cpucounters.o -lpthread  mt.cpp -o mt_O2
	g++ -O3 -std=c++11 -I $(PCM_DIR) -L $(PCM_DIR)/cpucounters.o $(PCM_DIR)/pci.o $(PCM_DIR)/msr.o $(PCM_DIR)/client_bw.o $(PCM_DIR)/cpucounters.o -lpthread  mt.cpp -o mt_O3

clean:
	rm pcm_test mt mt_O*
