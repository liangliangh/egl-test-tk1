
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
#define EGL_DEVICE_EXT            0x322C

// https://www.khronos.org/registry/egl/extensions/MESA/EGL_MESA_platform_gbm.txt
#include <gbm.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define EGL_PLATFORM_GBM_MESA     0x31D7


#include <GL/gl.h>
#include <GL/glext.h>

#ifdef __cplusplus
extern "C" {
#endif
GLAPI void APIENTRY glGenFramebuffers (GLsizei n, GLuint *framebuffers);
GLAPI void APIENTRY glBindFramebuffer (GLenum target, GLuint framebuffer);
GLAPI void APIENTRY glGenRenderbuffers (GLsizei n, GLuint *renderbuffers);
GLAPI void APIENTRY glBindRenderbuffer (GLenum target, GLuint renderbuffer);
GLAPI void APIENTRY glRenderbufferStorage (GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
GLAPI void APIENTRY glFramebufferRenderbuffer (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
#ifdef __cplusplus
}
#endif

#include "opencv2/opencv.hpp"
#include <omp.h>

const int width=640, height=480;
void drawUnitSquare()
{
	glBegin( GL_TRIANGLES );
	glNormal3f(0,0,1);
	glTexCoord2f(1,1);
	glVertex3f(0.5f,0.5f,0);
	glTexCoord2f(0,1);
	glVertex3f(-0.5f,0.5f,0);
	glTexCoord2f(0,0);
	glVertex3f(-0.5f,-0.5f,0);
	glTexCoord2f(0,0);
	glVertex3f(-0.5f,-0.5f,0);
	glTexCoord2f(1,0);
	glVertex3f(0.5f,-0.5f,0);
	glTexCoord2f(1,1);
	glVertex3f(0.5f,0.5f,0);
	glEnd();
}
void draw_and_save()
{
	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << ".\n";

	glViewport(0,0,width,height);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLuint frame_buffer_s;
	glGenFramebuffers(1, &frame_buffer_s);
	glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer_s);

	GLuint render_buff_rgba, render_buff_depth;
	glGenRenderbuffers(1, &render_buff_rgba);
	glGenRenderbuffers(1, &render_buff_depth);
	glBindRenderbuffer(GL_RENDERBUFFER, render_buff_rgba);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, render_buff_depth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
    		GL_RENDERBUFFER, render_buff_rgba);
	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
    		GL_RENDERBUFFER, render_buff_depth);

	for(int i=0; i<5; ++i){
		double t = omp_get_wtime();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glColor4f(1,0,0, 0.5f);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glRotatef(i*10, 1,1,0);
		drawUnitSquare();
		glFinish();
		std::cout << (omp_get_wtime()-t)*1000 << "ms.\n";
	}
	cv::Mat rgb_out(height, width, CV_8UC3);
	glReadPixels(0, 0, width, height, GL_BGR, GL_UNSIGNED_BYTE, rgb_out.data);
	cv::imwrite("img.png", rgb_out);
	std::cout << "img.png saved!\n";
}


int main()
{
	std::cout << "EXTENSIONS: " << eglQueryString(EGL_NO_DISPLAY, EGL_EXTENSIONS) << "\n";

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

	/*EGLint num_devices=0;
	EGLDeviceEXT device_gpu=0;
	eglQueryDevicesEXT(0, NULL, &num_devices);
	std::cout << "num_devices: " << num_devices << "\n";
	eglQueryDevicesEXT(1, &device_gpu, &num_devices);
	std::cout << "device_gpu: " << device_gpu << "\n";

	EGLAttrib device_cuda=0;
	eglQueryDeviceAttribEXT(device_gpu, EGL_DEVICE_EXT, &device_cuda);
	std::cout << "device_cuda: " << device_cuda << "\n";

	printf("surported extensions: %s\n", eglQueryDeviceStringEXT(device_gpu, EGL_EXTENSIONS));

	printf("eglGetError(): 0x%x\n", eglGetError());
	EGLint attrib_list[] = {EGL_NONE};
	EGLDisplay d = eglGetPlatformDisplayEXT(EGL_PLATFORM_DEVICE_EXT, 0, attrib_list);
	if(d) std::cout << "eglGetDisplay() success.\n";
	else std::cout << "eglGetDisplay() failed.\n";*/
	
	int fd = open("/dev/fb0", O_RDWR | FD_CLOEXEC);
	if(fd<0) std::cout << "open file failed.\n";
	else std::cout << "open file success.\n";

	struct gbm_device* gbm = gbm_create_device(fd);
	if(gbm) std::cout << "gbm_create_device() success.\n";
	else std::cout << "gbm_create_device() failed.\n";
	
	EGLDisplay egl = eglGetPlatformDisplayEXT(EGL_PLATFORM_GBM_MESA, gbm, NULL);
	if(egl==EGL_NO_DISPLAY) std::cout << "eglGetPlatformDisplayEXT() failed.\n";
	else std::cout << "eglGetPlatformDisplayEXT() success.\n";

	EGLint major, minor;
        if(eglInitialize(egl, &major, &minor))
            std::cout << "eglInitialize() success. version:" << major << "." << minor << "\n";
        else std::cout << "eglInitialize() failed.\n";

	std::cout << "query result: \n";
	std::cout << "version " << eglQueryString(egl, EGL_VERSION) << "\n";
	std::cout << "VENDOR " << eglQueryString(egl, EGL_VENDOR) << "\n";
	std::cout << "EXTENSIONS: " << eglQueryString(egl, EGL_EXTENSIONS) << "\n";
	std::cout << "CLIENT_APIS: " << eglQueryString(egl, EGL_CLIENT_APIS) << "\n";

	if( eglBindAPI(EGL_OPENGL_API) )
		printf("eglBindAPI success!\n");
	else printf("eglBindAPI failed!\n");

	EGLConfig first_config; EGLint numConfigs;
	eglGetConfigs(egl, &first_config, 1, &numConfigs);

	EGLContext ctx = eglCreateContext(egl, first_config, EGL_NO_CONTEXT, NULL);
	if(ctx != EGL_NO_CONTEXT)
		printf("create context success!\n");
	else printf("create context failed!\n");

	// https://www.khronos.org/registry/egl/extensions/KHR/EGL_KHR_surfaceless_context.txt
	EGLBoolean b = eglMakeCurrent(egl, EGL_NO_SURFACE, EGL_NO_SURFACE, ctx);
	if(b) printf("make current success!\n");
	else printf("make current failed!\n");

	printf("eglGetError(): 0x%x\n", eglGetError());

	draw_and_save();
	printf("glGetError(): 0x%x\n", glGetError());

	return 0;
}
