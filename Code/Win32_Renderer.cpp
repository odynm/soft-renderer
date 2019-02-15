//
// This file uses malloc and free
//

#include <stdint.h>

typedef int8_t int8;
typedef uint8_t uint8;
typedef int16_t int16;
typedef uint16_t uint16;
typedef int32_t int32;
typedef uint32_t uint32;
typedef int64_t int64;
typedef uint64_t uint64;

#include "Renderer.cpp"

#include <windows.h>

struct Win32WindowDimension
{
	int width;
	int height;
};

struct Win32OffscreenBuffer
{
	BITMAPINFO info;
	void* memory;
	int width;
	int height;
	int pitch;
	int bytesPerPixel;
};

bool running;
Win32OffscreenBuffer globalBackBuffer;

int64 perfCountFrequency;
LARGE_INTEGER lastCounter;

char exePath[MAX_PATH];

void 
Win32Print (char* buffer)
{
    OutputDebugStringA(buffer);
}

void 
ProfileStart()
{
    QueryPerformanceCounter(&lastCounter);
}

void* 
Alloc(int size)
{
    return malloc(size);
}

void
Release(void* address)
{
    free(address);
}

void
ProfileEnd()
{
    LARGE_INTEGER endCounter;
    QueryPerformanceCounter(&endCounter);
    
    int64 counterElapsed = endCounter.QuadPart - lastCounter.QuadPart;
    int32 msPerFrame = (int32)((1000000*counterElapsed) / perfCountFrequency);
    
    char string[256];
    wsprintf(string, "%d\n", msPerFrame);
    Win32Print(string);
}

#define MAX_FILE_SIZE 5242880 // 5Mb
char* 
OpenFile(char* filePath)
{
    
    if (exePath[0] == NULL)
    {   //Do we have the exe path yet?
        HMODULE module = GetModuleHandleA(NULL);
        int length = GetModuleFileName(module, exePath, MAX_PATH);
        
        int i = length;
        while(exePath[i] != '\\')
        {
            exePath[i] = NULL;
            i--;
        }
    }
    char path[MAX_PATH];
    
    int i = 0; int j = 0;
    while(exePath[j] != NULL)
    {
        path[i] = exePath[j];
        i++; j++;
    }
    j= 0;
    while(filePath[j] != NULL)
    {
        path[i] = filePath[j];
        i++; j++;
    }
	//Terminate string
	path[i] = '\0';
    
    HANDLE file = CreateFile(path, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    void* stringBuffer = Alloc(MAX_FILE_SIZE);
    LPDWORD bytesReaded = 0;
    ReadFile(file, stringBuffer, MAX_FILE_SIZE, bytesReaded, NULL);
    
    CloseHandle(file);
    
    return (char*)stringBuffer;
}

Win32WindowDimension
Win32GetWindowDimension(HWND window)
{
    Win32WindowDimension result;
    
    RECT clientRect;
    GetClientRect(window, &clientRect);
    result.width = clientRect.right - clientRect.left;
    result.height = clientRect.bottom - clientRect.top;
    
    return result;
}

void
Win32ResizeDibSection(Win32OffscreenBuffer* buffer, int width, int height)
{
    if (buffer->memory)
    {
        VirtualFree(buffer->memory, 0, MEM_RELEASE);
    }
    
    buffer->width = width;
    buffer->height = height;
    buffer->bytesPerPixel = 4;
    
    buffer->info.bmiHeader.biSize = sizeof(buffer->info.bmiHeader);
    buffer->info.bmiHeader.biWidth = buffer->width;
    buffer->info.bmiHeader.biHeight = -buffer->height;
    buffer->info.bmiHeader.biPlanes = 1;
    buffer->info.bmiHeader.biBitCount = 32;
    buffer->info.bmiHeader.biCompression = BI_RGB;
    
    
    int bitmapMemorySize = buffer->bytesPerPixel * buffer->width * buffer->height;
    buffer->memory = VirtualAlloc(0, bitmapMemorySize, MEM_COMMIT, PAGE_READWRITE);
    
    buffer->pitch = width * buffer->bytesPerPixel;
    
}

void
Win32DisplayBufferInWindow(Win32OffscreenBuffer* buffer, HDC deviceContext, int windowWidth, int windowHeight)
{
    StretchDIBits(deviceContext,
                  0, 0, windowWidth, windowHeight,
                  0, 0, buffer->width, buffer->height,
                  buffer->memory,
                  &buffer->info,
                  DIB_RGB_COLORS, SRCCOPY);
}

LRESULT CALLBACK
Win32MainWindowCallback(
HWND   window,
UINT   message,
WPARAM wparam,
LPARAM lparam
)
{
    LRESULT result = 0;
    
    switch (message)
    {
        case WM_SIZE:
        {
            break;
        }
        
        
        case WM_CLOSE:
        {
            running = false;
            break;
        }
        
        case WM_ACTIVATEAPP:
        {
            OutputDebugStringA("WM_ACTIVATEAPP\n");
            break;
        }
        case WM_DESTROY:
        {
            running = false;
            break;
        }
        
        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
        
        case WM_PAINT:
        {
            PAINTSTRUCT paint;
            HDC deviceContext = BeginPaint(window, &paint);
            
            int x = paint.rcPaint.left;
            int y = paint.rcPaint.top;
            
            Win32WindowDimension dim = Win32GetWindowDimension(window);
            
            int width = paint.rcPaint.right - paint.rcPaint.left;
            int height = paint.rcPaint.bottom - paint.rcPaint.top;
            RECT clientRect;
            GetClientRect(window, &clientRect);
            
            Win32DisplayBufferInWindow(&globalBackBuffer, deviceContext, dim.width, dim.height);
            EndPaint(window, &paint);
            
            break;
        }
        
        default:
        {
            result = DefWindowProc(window, message, wparam, lparam);
            break;
        }
    }
    return result;
}

int CALLBACK WinMain(
HINSTANCE instance,
HINSTANCE prevInstance,
LPSTR     cmdLine,
int       showCode
)
{
    LARGE_INTEGER perfCounterFrequencyResult;
    QueryPerformanceFrequency(&perfCounterFrequencyResult);
    perfCountFrequency = perfCounterFrequencyResult.QuadPart;
    
    WNDCLASSA windowClass = {};
    
    Win32ResizeDibSection(&globalBackBuffer, 1280, 720);
    
    windowClass.style = CS_HREDRAW | CS_VREDRAW;
    
    windowClass.lpfnWndProc = Win32MainWindowCallback;
    windowClass.hInstance = instance;
    windowClass.lpszClassName = "RendererClass";
    
    
    if (RegisterClass(&windowClass))
    {
        HWND window = CreateWindowEx(0,
                                     windowClass.lpszClassName,
                                     "3D Realms",
                                     WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                                     CW_USEDEFAULT,
                                     CW_USEDEFAULT,
                                     1280,
                                     720,
                                     0,
                                     0,
                                     instance,
                                     0);
        if (window)
        {
            HDC deviceContext = GetDC(window);
            
            InitRenderer();
            
            running = true;
            while (running)
            {
                
                MSG message;
                while (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
                {
                    if (message.message == WM_QUIT)
                    {
                        running = false;
                    }
                    
                    TranslateMessage(&message);
                    DispatchMessage(&message);
                }
                
                OffscreenBuffer buffer = {};
                buffer.memory = globalBackBuffer.memory;
                buffer.width = globalBackBuffer.width;
                buffer.height = globalBackBuffer.height;
                buffer.pitch = globalBackBuffer.pitch;
                UpdateAndRender(&buffer);
                
                
                Win32WindowDimension dim = Win32GetWindowDimension(window);
                Win32DisplayBufferInWindow(&globalBackBuffer, deviceContext, dim.width, dim.height);
                
            }
        }
        else
        {
            
        }
    }
    else
    {
        // TODO: Logging
    }
    return 0;
}
