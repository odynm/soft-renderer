#include "Renderer.h"

/////////////////
//This should not be needed later on
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
/////////////////

void
CleanBuffer(OffscreenBuffer* buffer)
{
    uint8* row = (uint8 *)buffer->memory;
    
	for (int y = 0; y < buffer->height; y++)
	{
		uint32* pixel = (uint32*)row;
		for (int x = 0; x < buffer->width; x++)
		{
			uint8 b = 0;
			uint8 g = 0;
            uint8 r = 0;
            
			*pixel++ = ((r << 16) | (g << 8) | b);
		}
		row += buffer->pitch;
	}
}

void inline
PutPixel(OffscreenBuffer* buffer, int x, int y, int rgb)
{
    //invert y axis
    y = abs(720-y);
    
    //check against out of bounds
    if (x >= 1280 || x < 0 || y < 0 || y >= 720) return;
    
    uint8* pixels = (uint8*)buffer->memory;
    uint8* row = (uint8*)&pixels[y * buffer->pitch];
    int32* pixel = (int32*)&row[x * 4];
    (*pixel) = rgb;
}

void 
DrawLine(OffscreenBuffer* buffer, int x0, int y0, int x1, int y1, int rgb)
{
    bool isSteep = false;
    int xLineSpan, yLineSpan, correction, deltaCorrection, curveDirection;
    xLineSpan = abs(x1 - x0);
    yLineSpan = abs(y1 - y0);
    
    if(yLineSpan > xLineSpan)
    {   //Is it a steep line?
        SwapInt(x0,y0);
        SwapInt(x1,y1);
        SwapInt(xLineSpan,yLineSpan);
        isSteep = true;
    }
    
    if(x0>x1)
    {   //Does the line is 'backwards'?
        SwapInt(x0,x1);
        SwapInt(y0,y1);
    }
    
    deltaCorrection = abs(yLineSpan*2);
    correction = 0;
    curveDirection = y1 > y0 ? 1 : -1;
    
    for (; x0 < x1; x0++)
    {
        if (isSteep)
        {   //If is steep we need to 'deswap' x and y
            PutPixel(buffer, y0, x0, rgb);
        }
        else
        {
            PutPixel(buffer, x0, y0, rgb);
        }
        
        correction += deltaCorrection;
        if(correction > xLineSpan)
        {   //Is time to increase the slope?
            y0 += curveDirection;
            correction -= xLineSpan*2;
        }
    }
}

//TODO Don't leave this here
Model* model;
int width = 1280;
int height = 720;

void
InitRenderer()
{
    model = LoadModel("african_head.obj");
}

void
UpdateAndRender(OffscreenBuffer* buffer)
{
    CleanBuffer(buffer);
    
    uint8 r = 255;
    uint8 g = 0;
    uint8 b = 0;
    
    int red = (255 << 24) | (r << 16) | (g << 8) | b;
    int blue = (255 << 24) | (b << 16) | (g << 8) | r;
    
    for(int i = 0; i < model->facesCount; i++)
    {
        int* faceData = model->faces[i].data;
        for (int j = 0; j < 3; j++)
        {
            Vector3 v0 = model->vertices[faceData[j]-1];
            Vector3 v1 = model->vertices[faceData[((j+1)%3)]-1];
            
            int x0 = (v0.x+1.f)*width /2.f;
            int y0 = (v0.y+1.f)*height/2.f;
            int x1 = (v1.x+1.f)*width /2.f;
            int y1 = (v1.y+1.f)*height/2.f;
            
            DrawLine(buffer, x0, y0, x1, y1, red);
        }
    }
    
    
    //ProfileStart();
    
    /*DrawLine(buffer, 100, 50, 100, 250, red);
    DrawLine(buffer, 0, 0, 300, 300, red);
    DrawLine(buffer, 300, 50, 50, 250, blue);
    DrawLine(buffer, 100, 50, 300, 50, red);
    DrawLine(buffer, 100, 150, 0, 0, red);*/
    
    //ProfileEnd();
}
