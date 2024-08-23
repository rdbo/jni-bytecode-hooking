CXXFLAGS := -I/usr/lib/jvm/java-21-openjdk/include -I/usr/lib/jvm/java-21-openjdk/include/linux

all:
	$(JAVA_HOME)/bin/javac Target.java
	cd hook && $(JAVA_HOME)/bin/javac Target.java
	$(CXX) -o libtest.so -shared -fPIC $(CXXFLAGS) test.cpp
