#include <jni.h>
#include <jvmti.h>
#include <pthread.h>
#include <iostream>
#include <iomanip>
#include <vector>

std::vector<unsigned char>
read_custom_class()
{
	FILE *f = fopen("hook/Target.class", "r");
	fseek(f, 0, SEEK_END);
	
	size_t size = ftell(f);
	std::vector<unsigned char> vec;
	fseek(f, 0, SEEK_SET);

	vec.resize(size);

	fread(vec.data(), 1, size, f);
	return vec;
}

JNIEXPORT jvalue JNICALL hkMyFunction(JNIEnv *env, jclass clazz, jint number) {
	std::cout << "myFunction was fully hooked from JNI!" << std::endl;
	std::cout << "env: " << env << std::endl;
	std::cout << "class: " << clazz<< std::endl;
	std::cout << "the original number was: " << number << std::endl;
	return jvalue {};
}

void *main_thread(void *args)
{
	JavaVM *jvm;
	JNIEnv *env;
	jvmtiEnv *jvmti;
	jsize size;
	jclass clazz;
	jvmtiCapabilities capabilities;
	jvmtiPhase phase;
	
	std::cout << "[*] Main thread started" << std::endl;

	// Initialize JVM helpers
	JNI_GetCreatedJavaVMs(&jvm, 1, &size);
	std::cout << "[*] JVM: " << jvm << std::endl;

	jvm->AttachCurrentThread(reinterpret_cast<void **>(&env), NULL);
	std::cout << "[*] JNIEnv: " << env << std::endl;

	jvm->GetEnv(reinterpret_cast<void **>(&jvmti), JVMTI_VERSION_1_2);
	std::cout << "[*] JVMTI: " << jvmti << std::endl;

	jvmti->GetPhase(&phase);
	std::cout << "[*] JVMTI Phase: " << phase << std::endl;

	// Give capabilities to JVMTI
	jvmti->GetPotentialCapabilities(&capabilities);
	std::cout << "[*] Add JVMTI capabilities status: " << (jvmti->AddCapabilities(&capabilities) ? "ERR" : "OK") << std::endl;

	clazz = env->FindClass("Target");
	std::cout << "[*] Target class: " << clazz << std::endl;

	// Redefine Target class
	auto hook_class = read_custom_class();
	jvmtiClassDefinition definition = { clazz, static_cast<jint>(hook_class.size()), hook_class.data() };

	std::cout << "[*] New class (size: " << hook_class.size() << "): [ ";

	for (auto b : hook_class) {
		std::cout << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(b);
		std::cout << " ";
	}

	std::cout << "]" << std::dec << std::endl;

	std::cout << "[*] RedefineClasses result: " << (jvmti->RedefineClasses(1, &definition) ? "ERR" : "OK") << std::endl;

	// Register native method for hooking
	JNINativeMethod method = {
		const_cast<char *>("myFunction"), const_cast<char *>("(I)V"), reinterpret_cast<void *>(hkMyFunction)
	};
	env->RegisterNatives(clazz, &method, 1);
	

	jvm->DetachCurrentThread();

	return NULL;
}

void __attribute__((constructor))
dl_entry()
{
	printf("[*] Library loaded!\n");
	
	pthread_t th;
	pthread_create(&th, NULL, main_thread, NULL);
}
