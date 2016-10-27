alltarget:thpool
thpool:thread_pool.cpp sig_wrapper.cpp TEST.cpp
	g++ -std=c++11 -lpthread -o thpool thread_pool.cpp sig_wrapper.cpp TEST.cpp
