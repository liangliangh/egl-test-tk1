#include <iostream>
#include <stdio.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>

typedef void* EGLDeviceEXT;
typedef intptr_t EGLAttrib;

typedef EGLBoolean FUNC_eglQueryDevicesEXT(
				EGLint max_devices,
                                EGLDeviceEXT *devices,
                                EGLint *num_devices);

typedef EGLDisplay FUNC_eglGetPlatformDisplayEXT(
        			EGLenum platform,
        			void *native_display,
        			const EGLint *attrib_list);

typedef EGLBoolean FUNC_eglQueryDeviceAttribEXT(
				EGLDeviceEXT device,
				EGLint attribute,
				EGLAttrib *value);
typedef const char* FUNC_eglQueryDeviceStringEXT(
				EGLDeviceEXT device,
				EGLint name);

#define EGL_PLATFORM_DEVICE_EXT   0x313F
#define EGL_PLATFORM_X11_EXT      0x31D5 // yes
#define EGL_PLATFORM_GBM_KHR      0x31D7 // no
#define EGL_PLATFORM_ANDROID_KHR  0x3141 // no
#define EGL_CUDA_DEVICE_NV        0x323A
#define EGL_DEVICE_EXT            0x322C

int main()
{
	//EGLDisplay d = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	//if(d) std::cout << "eglGetDisplay() success.\n";
	//else return -1;
	//int maj, min;
	//if(eglInitialize(d, &maj, &min)) std::cout << "eglInitialize() success.\n";
	//else return -2;
	//std::cout << "query result: \n";
	//std::cout << "version " << eglQueryString(d, EGL_VERSION) << "\n";
	//std::cout << "VENDOR " << eglQueryString(d, EGL_VENDOR) << "\n";
	//std::cout << "EXTENSIONS: " << eglQueryString(d, EGL_EXTENSIONS) << "\n";
	//std::cout << "CLIENT_APIS: " << eglQueryString(d, EGL_CLIENT_APIS) << "\n";

	//std::cout << "eglQueryDevicesEXT:" << (int)eglGetProcAddress("eglQueryDevicesEXT") << "\n";
	//std::cout << "eglGetPlatformDisplayEXT:" << (int)eglGetProcAddress("eglGetPlatformDisplayEXT") << "\n";	
	
	FUNC_eglQueryDevicesEXT* eglQueryDevicesEXT =
		(FUNC_eglQueryDevicesEXT*)eglGetProcAddress("eglQueryDevicesEXT");
	if(eglQueryDevicesEXT) std::cout << "get eglQueryDevicesEXT() success.\n";
	else std::cout << "get eglQueryDevicesEXT() failed.\n";

	FUNC_eglGetPlatformDisplayEXT* eglGetPlatformDisplayEXT =
		(FUNC_eglGetPlatformDisplayEXT*)eglGetProcAddress("eglGetPlatformDisplayEXT");
	if(eglGetPlatformDisplayEXT) std::cout << "get eglGetPlatformDisplayEXT() success.\n";
	else std::cout << "get eglGetPlatformDisplayEXT() failed.\n";

	FUNC_eglQueryDeviceAttribEXT* eglQueryDeviceAttribEXT =
		(FUNC_eglQueryDeviceAttribEXT*)eglGetProcAddress("eglQueryDeviceAttribEXT");
	if(eglQueryDeviceAttribEXT) std::cout << "get eglQueryDeviceAttribEXT() success.\n";
	else std::cout << "get eglQueryDeviceAttribEXT() failed.\n";

	FUNC_eglQueryDeviceStringEXT* eglQueryDeviceStringEXT =
		(FUNC_eglQueryDeviceStringEXT*)eglGetProcAddress("eglQueryDeviceStringEXT");
	if(eglQueryDeviceAttribEXT) std::cout << "get eglQueryDeviceStringEXT() success.\n";
	else std::cout << "get eglQueryDeviceStringEXT() failed.\n";

	EGLint num_devices=0;
	EGLDeviceEXT device_gpu=0;
	eglQueryDevicesEXT(0, 0, &num_devices);
	std::cout << "num_devices: " << num_devices << "\n";
	eglQueryDevicesEXT(1, &device_gpu, &num_devices);
	std::cout << "device_gpu: " << device_gpu << "\n";

	//EGLAttrib device_cuda=0;
	//eglQueryDeviceAttribEXT(device_gpu, EGL_DEVICE_EXT, &device_cuda);
	//std::cout << "device_cuda: " << device_cuda << "\n";

	printf("surported extensions: %s\n", eglQueryDeviceStringEXT(device_gpu, EGL_EXTENSIONS));

	printf("eglGetError(): 0x%x\n", eglGetError());
	EGLint attrib_list[] = {EGL_NONE};
	EGLDisplay d = eglGetPlatformDisplayEXT(EGL_PLATFORM_DEVICE_EXT, device_gpu, attrib_list);
	if(d) std::cout << "eglGetDisplay() success.\n";
	else std::cout << "eglGetDisplay() failed.\n";
	
	printf("eglGetError(): 0x%x\n", eglGetError());

	return 0;
}
