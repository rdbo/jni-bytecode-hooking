CXXFLAGS := -I/usr/lib/jvm/java-21-openjdk/include -I/usr/lib/jvm/java-21-openjdk/include/linux

all:
	javac Target.java
	cd hook && javac Target.java
	$(CXX) -o libtest.so -shared -fPIC $(CXXFLAGS) test.cpp
