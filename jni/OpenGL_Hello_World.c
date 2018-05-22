/****************************************************************************
*                               Include File                                *
****************************************************************************/
#define __gl2_h_
#include <stdio.h>
#include <stdlib.h>
#include <GLES3/gl3.h>
#include <GLES2/gl2ext.h>
#include <GLES3/gl3platform.h>
#include <EGL/egl.h>

#include "sys/system_properties.h"
#include "OpenGL_Hello_World.h"

/****************************************************************************
*                        Private Constant Definition                        *
****************************************************************************/
const char g_ccCrossMapSrcVertical_Frag[] =
{
    "precision highp float;                                                        \n"
    "                                                                              \n"
    "void main()                                                                   \n"
    "{                                                                             \n"
    "    vec4 color = vec4(1.0, 0.0, 0.0, 1.0);                                    \n"
    "    gl_FragColor = color;                                                     \n"
    "}                                                                             \n"
};

const char g_ccCrossMapSrcVertical_Vert[] =
{
    "precision highp float;                             \n"
    "attribute vec4 position;                           \n"
    "                                                   \n"
    "void main()                                        \n"
    "{                                                  \n"
    "    gl_Position = position;                        \n"
    "}                                                  \n"
};

/****************************************************************************
*                        Private Function Prototype                         *
****************************************************************************/

/****************************************************************************
*                          Private Global Variable                          *
****************************************************************************/
static EGLDisplay g_eglDisplay;
static EGLSurface g_eglSurface;
static EGLContext g_ctxt;

/****************************************************************************
*                             Private Function                              *
****************************************************************************/
void OpenGL_Init()
{
    EGLConfig config[10] = {};
    EGLConfig m_eglConfig;
    EGLint contextClientVersion = 2, iMajorVersion, iMinorVersion;;
    EGLint numConfigs = -1;

    const EGLint cfgAttribs[] =  
    {  
        EGL_SURFACE_TYPE,    EGL_PBUFFER_BIT | EGL_WINDOW_BIT,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_RED_SIZE,               5,
        EGL_GREEN_SIZE,             6,
        EGL_BLUE_SIZE,              5,
        EGL_ALPHA_SIZE, EGL_DONT_CARE,
        EGL_DEPTH_SIZE,            16,
        EGL_STENCIL_SIZE,           4,
        EGL_NONE
    };

    EGLint contextAttributes[] = 
    {
        EGL_CONTEXT_CLIENT_VERSION, contextClientVersion,
        EGL_NONE
    };

    EGLint PBufAttribs[] = 
    { 
        EGL_WIDTH, 1, EGL_HEIGHT, 1, EGL_NONE
    };
    
    g_eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    if (EGL_NO_DISPLAY == g_eglDisplay) LOGD("AJ eglDisplay failed - EGL_NO_DISPLAY");
    else LOGD("AJ eglDisplay successfully");
    
    if (GL_FALSE == eglInitialize(g_eglDisplay, &iMajorVersion, &iMinorVersion)) LOGD("AJ eglInitialize failed");
    else LOGD("AJ eglInitialize successfully");
    
    if(EGL_FALSE == eglChooseConfig(g_eglDisplay, cfgAttribs, &config[0], 10, &numConfigs)) LOGD("AJ eglChooseConfig failed");
    else LOGD("AJ eglChooseConfig successfully");
    
    m_eglConfig = config[0];

    g_eglSurface = eglCreatePbufferSurface(g_eglDisplay, m_eglConfig, PBufAttribs);

    if (g_eglSurface == EGL_NO_SURFACE) LOGD("AJ eglCreatePbufferSurface failed - eglGetError Returned: 0x%x", eglGetError());
    else LOGD("AJ eglCreatePbufferSurface successfully");
    
    g_ctxt = eglCreateContext(g_eglDisplay, m_eglConfig, EGL_NO_CONTEXT, contextAttributes);

    if (EGL_NO_CONTEXT == g_ctxt) LOGD("AJ eglCreateContext failed - eglGetError Returned: 0x%x", eglGetError());
    else LOGD("AJ eglCreateContext successfully");
    
    if (EGL_FALSE == eglMakeCurrent(g_eglDisplay, g_eglSurface, g_eglSurface, g_ctxt)) LOGD("AJ eglMakeCurrent failed - eglGetError Returned: 0x%x", eglGetError());
    else LOGD("AJ eglMakeCurrent successfully");
}

void OpenGL_Close()
{
    if (EGL_TRUE != eglMakeCurrent(g_eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT))
        LOGD("eglMakeCurrent Returned: 0x%x", eglGetError());

    if (EGL_TRUE != eglDestroySurface(g_eglDisplay, g_eglSurface))
        LOGD("eglDestroySurface Returned: 0x%x", eglGetError());

    if (EGL_TRUE != eglDestroyContext(g_eglDisplay, g_ctxt))
        LOGD("eglDestroyContext Returned: 0x%x", eglGetError());

    if(EGL_TRUE != eglTerminate(g_eglDisplay))
        LOGD("eglTerminate Returned: 0x%x", eglGetError());
}

void main()
{
    OpenGL_Init();
    
    int dImageWidth = 320;
    int dImageHeight = 240;
    int TEXTURE_W = 1280;
    int TEXTURE_H = 960;
    int i = 0;
    unsigned char FBO_COUNT = 5;
    GLuint texture_carter[FBO_COUNT];
    GLuint color_tex[FBO_COUNT];
    GLuint fbo[FBO_COUNT];
    GLuint depth_rb[FBO_COUNT];
    char result[2048];
    int  length=2048;
    unsigned int shaderName;
    GLint linked = 0;
    GLint isCompiled = 0;
    GLint m_viewport[4];
    GLenum  GLnGetError;
    const char *p_ccCrossMapSrcVertical_Vert = g_ccCrossMapSrcVertical_Vert;
    const char *p_ccCrossMapSrcVertical_Frag = g_ccCrossMapSrcVertical_Frag;
    
    // Create the shader object
    unsigned int vertShader = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    LOGD("AJ vertShader = %d", vertShader);
    LOGD("AJ fragShader = %d", fragShader);
    
    // Load the shader source
    glShaderSource(vertShader, 1, &p_ccCrossMapSrcVertical_Vert, NULL);
    glShaderSource(fragShader, 1, &p_ccCrossMapSrcVertical_Frag, NULL);

    GLnGetError = glGetError();
    if (GL_NO_ERROR != GLnGetError)
    {
        LOGD("AJ glGetError GLnGetError = %d", GLnGetError);
    }

    // Compile the vertShader
    glCompileShader(vertShader);
    // Compile the fragShader
    glCompileShader(fragShader);

    // Check the compile status
    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &isCompiled);
    if(!isCompiled)
    {
        glGetShaderInfoLog(vertShader, length, &length, &result[0]);
        LOGD("AJ vertShader compiler result = %s", result);
    }
    
    // Check the compile status
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &isCompiled);
    if(!isCompiled)
    {
        glGetShaderInfoLog(fragShader, length, &length, &result[0]);
        LOGD("AJ fragShader compiler result = %s",result);
    }

    // Create the program object
    shaderName = glCreateProgram();

    // Attach the shader to program object
    glAttachShader(shaderName, vertShader);
    glAttachShader(shaderName, fragShader);

    // Link the program object specified by shaderName
    glLinkProgram(shaderName);

    glGetProgramiv(shaderName, GL_LINK_STATUS, &linked);
    
    if(!linked)
    {
        GLint infoLen = 0;
        glGetProgramiv(shaderName, GL_INFO_LOG_LENGTH, &infoLen);

        if(infoLen > 1)
        {
            char* infoLog = (char*)malloc(sizeof(char) * infoLen);
            glGetProgramInfoLog(shaderName, infoLen, NULL, infoLog);
            LOGD("AJ Error linking program:\n%s\n", infoLog);

            free(infoLog);
        }
        glDeleteProgram(shaderName);
    }

    // generate texture names
    glGenTextures(FBO_COUNT, texture_carter);
    glGenTextures(FBO_COUNT, color_tex);
    glGenFramebuffers(FBO_COUNT, fbo);
    glGenRenderbuffers(FBO_COUNT, depth_rb);;
    
    // full size FBO for original resolution.
    glBindTexture(GL_TEXTURE_2D, color_tex[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // specify a two-dimensional texture image
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TEXTURE_W, TEXTURE_H, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    glBindFramebuffer(GL_FRAMEBUFFER, fbo[0]);
    // Attach 2D texture to this FBO
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_tex[0], 0);

    glBindRenderbuffer(GL_RENDERBUFFER, depth_rb[0]);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24_OES, TEXTURE_W, TEXTURE_H);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_rb[0]);

    //fbo[1] and beyond are reduce sized.
    
    for (i=1; i<FBO_COUNT; i++)
    {
        glBindTexture(GL_TEXTURE_2D, color_tex[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // NULL means reserve texture memory, but texels are undefined
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TEXTURE_W, TEXTURE_H, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

        glBindFramebuffer(GL_FRAMEBUFFER, fbo[i]);
        // Attach 2D texture to this FBO
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_tex[i], 0);
        //-------------------------
        glBindRenderbuffer(GL_RENDERBUFFER, depth_rb[i]);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24_OES, TEXTURE_W, TEXTURE_H);
        //-------------------------
        //Attach depth buffer to FBO
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_rb[i]);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0); // return render target to back buffer.

    GLfloat vertices[] = 
    {
        0.0f,  0.5f, 0.0f,
       -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f
    };

    // Set the view port window
    glViewport(0, 0, dImageWidth, dImageHeight);

    glGetIntegerv(GL_VIEWPORT, m_viewport);

    // Clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Use the program object
    glUseProgram(shaderName);
    
    // Load the vertex data
    int mPositionHandle = glGetAttribLocation(shaderName, "position");
    glEnableVertexAttribArray(mPositionHandle);
    glVertexAttribPointer(mPositionHandle, 3, GL_FLOAT, 0, 0, vertices);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo[0]);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    
    // let Color in BLUE anyway
    glClearColor(0.0, 0.0, 1.0, 1.0);

    FILE *fp;
    int displayW = m_viewport[2];
    int displayH = m_viewport[3];

    LOGD("AJ m_viewport = %d", m_viewport[0]);
    LOGD("AJ m_viewport = %d", m_viewport[1]);
    LOGD("AJ m_viewport = %d", m_viewport[2]);
    LOGD("AJ m_viewport = %d", m_viewport[3]);

    unsigned char *Result_buffer = (unsigned char *)malloc(displayW * displayH * 4 * sizeof(unsigned char));
    glReadPixels(0, 0, displayW, displayH, GL_RGBA, GL_UNSIGNED_BYTE, Result_buffer);

    unsigned char *Result_rgb = (unsigned char *)malloc(displayW*displayH*3*sizeof(unsigned char));

    for(i = 0; i < displayW*displayH; i++)
    {
        Result_rgb[3*i] = Result_buffer[4*i];
        Result_rgb[3*i + 1] = Result_buffer[4*i + 1];
        Result_rgb[3*i + 2] = Result_buffer[4*i + 2];
    }

    char debugResultImage[1024];
    char strSerialNumber[512];

    memset(debugResultImage, '\0', sizeof(debugResultImage));
    //__system_property_get("persist.al.LVGLbokeh.fileDir", debugResultImage);
    sprintf(strSerialNumber, "/sdcard/DCIM/GL_output_%dx%d_gpu.raw", 
                               displayW, displayH);
    strcat(debugResultImage, strSerialNumber);
    if((fp=fopen(debugResultImage, "wb"))!=NULL)
    {
        //fwrite(Result_buffer, 1, displayW * displayH *4 * sizeof(unsigned char), fp);
        fwrite(Result_rgb, 1, displayW*displayH * 3 * sizeof(unsigned char), fp);
        fclose(fp);
    }
    else
    {
        LOGD("AJ WriteRAW Image gpu(Result) Failed!!!");
    }

    fclose(fp);
    
    if(Result_buffer)
    {    
        free(Result_buffer);
        free(Result_rgb);
    }
    
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);
    glDeleteTextures(FBO_COUNT, texture_carter);
    glDeleteTextures(FBO_COUNT, color_tex);
    glDeleteFramebuffers(FBO_COUNT, fbo);
    glDeleteRenderbuffers(FBO_COUNT, depth_rb);
    glDeleteProgram(shaderName);
    OpenGL_Close();

    LOGD("AJ Finish!");
}