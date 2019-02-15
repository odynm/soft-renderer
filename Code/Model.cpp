#include <stdlib.h>

#include "Model.h"

Face 
CreateFace(int a, int b, int c)
{
    Face face = {};
    face.data[0] = a;
    face.data[1] = b;
    face.data[2] = c;
    return face;
}

Model* 
LoadModel(char* file)
{
    char* content = OpenFile(file);
    
    //
    //Count number of faces and vertices
    //
    int numVertex = 0;
    int numFaces = 0;
    for(int fileIndex = 0; content[fileIndex]; fileIndex++)
    {   //Iterate through file buffer
        if(content[fileIndex] == 'v' && content[fileIndex+1] == ' ')
        {   //Is it a vector (starts with v)?
            numVertex++;
        }
        else if (content[fileIndex] == 'f' && content[fileIndex+1] == ' ')
        {   //Is it a face (starts with f)?
            numFaces++;
        }
        
        //Resume to the end of the line
        while (content[fileIndex] != '\0' && content[fileIndex] != '\n') fileIndex++;
    }
    
    
    //
    //Parse file
    //
    Model* model = (Model*)Alloc(sizeof(Model));
    model->vertexCount = numVertex;
    model->facesCount = numFaces;
    model->vertices = (Vector3*)Alloc(sizeof(Vector3)*numVertex);
    model->faces = (Face*)Alloc(sizeof(Face)*numFaces);
    
    char* parseBuffer = (char*)Alloc(32);
    int vertexIndex = 0;
    int faceIndex = 0;
    
    for(int fileIndex = 0; content[fileIndex]; fileIndex++)
    {   //Iterate through file buffer
        if(content[fileIndex] == 'v' && content[fileIndex+1] == ' ')
        {   //Is it a vector (starts with v)?
            float vectorValues[3];
            
            //v xxxx xxxx xxxx
            for (int vectorArrayIndex = 0; vectorArrayIndex < 3; vectorArrayIndex++)
            {   //Iterate through the three float values
                
                CleanStringBuffer(parseBuffer, 32);
                
                fileIndex = fileIndex+2; //Jump last char and white space
                int start = fileIndex;
                for (; content[fileIndex+1] != ' '; fileIndex++) {} //Find end of float
                SubString(parseBuffer, content, start, fileIndex);
                parseBuffer[(fileIndex - start) + 1] = '\0'; //Terminate string
                vectorValues[vectorArrayIndex] = atof(parseBuffer);
            }
            fileIndex--; //Return to end of the line (\n)
            
            model->vertices[vertexIndex] = CreateVector3(vectorValues[0],
                                                         vectorValues[1],
                                                         vectorValues[2]);
            vertexIndex++;
        }
        else if (content[fileIndex] == 'f' && content[fileIndex+1] == ' ')
        {   //Is it a face (starts with f)?
            int faceValues[3];
            
            //f xxxx/xxxx/xxxx xxxx/xxxx/xxxx xxxx/xxxx/xxxx
            for (int faceArrayIndex = 0; faceArrayIndex < 3; faceArrayIndex++)
            {   //Iterate through the three int values
                fileIndex = fileIndex+2; //Jump last char and white space
                int start = fileIndex;
                for (; content[fileIndex+1] != '/'; fileIndex++) {} //Find end of first int
                SubString(parseBuffer, content, start, fileIndex);
                parseBuffer[(fileIndex - start) + 1] = '\0'; //Terminate string
                faceValues[faceArrayIndex] = atoi(parseBuffer);
                
                while (content[fileIndex+1] != ' ') fileIndex++;
            }
            fileIndex--; //Return to end of the line (\n)
            
            model->faces[faceIndex] = CreateFace(faceValues[0],
                                                 faceValues[1],
                                                 faceValues[2]);
            faceIndex++;
        }
        else
        {   //If is nothing of interest, resume to the end of the line
            while (content[fileIndex] != '\0' && content[fileIndex] != '\n') fileIndex++;
        }
    }
    
    Release((void*)parseBuffer);
    Release(content);
    return model;
}
