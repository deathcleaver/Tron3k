#define REDEFCALLS

#include "GPUMemoryLeakChecker.h"
#include <iostream>
#include <Windows.h>

using std::cout;
using std::endl;

vector<GLBuffer> genBufferChecks;
vector<GLBuffer> genVertexArrayChecks;
vector<GLBuffer> genTextureCheck;

void glGenBuffers_D(GLsizei n, GLuint* id, char* file, int line)
{
	GLBuffer buf;
	
	glGenBuffers(n, id);

	buf.bufferID = *id;
	buf.file = std::string(file);
	buf.line = line;
	buf.free = false;

	genBufferChecks.push_back(buf);
}

void glGenVertexArray_D(GLsizei n, GLuint* id, char* file, int line)
{
	GLBuffer buf;

	glGenVertexArrays(n, id);

	buf.bufferID = *id;
	buf.file = std::string(file);
	buf.line = line;
	buf.free = false;
	
	genVertexArrayChecks.push_back(buf);
}

void glGenTexture_D(GLsizei n, GLuint* id, char* file, int line)
{
	GLBuffer buf;

	glGenTextures(n, id);

	buf.bufferID = *id;
	buf.file = std::string(file);
	buf.line = line;
	buf.free = false;

	genTextureCheck.push_back(buf);
}

void glDeleteBuffers_D(GLsizei n, GLuint* id)
{
	for (size_t i = 0; i < genBufferChecks.size(); i++)
	{
		if (genBufferChecks[i].bufferID == *id)
		{
			glDeleteBuffers(n, id);
			genBufferChecks.erase(genBufferChecks.begin()+i);
			break;
		}
	}
}

void glDeleteVertexArray_D(GLsizei n, GLuint* id)
{
	for (size_t i = 0; i < genVertexArrayChecks.size(); i++)
	{
		if (genVertexArrayChecks[i].bufferID == *id)
		{
			glDeleteVertexArrays(n, id);
			genVertexArrayChecks.erase(genVertexArrayChecks.begin() + i);
			break;
		}
	}
}

void glDeleteTexture_D(GLsizei n, GLuint* id)
{
	for (size_t i = 0; i < genTextureCheck.size(); i++)
	{
		if (genTextureCheck[i].bufferID == *id)
		{
			glDeleteTextures(n, id);
			genTextureCheck.erase(genTextureCheck.begin() + i);
			break;
		}
	}
}

void reportGPULeaks()
{
	bool found = false;
	for (size_t i = 0; i < genBufferChecks.size(); i++)
	{
		if (genBufferChecks[i].free == false)
		{
			cout << "glGenBuffers\n" << genBufferChecks[i].file << " : " << genBufferChecks[i].line << endl;
			found = true;
		}
	}

	for (size_t i = 0; i < genVertexArrayChecks.size(); i++)
	{
		if (genVertexArrayChecks[i].free == false)
		{
			cout << "glGenVertexArrays\n" << genVertexArrayChecks[i].file << " : " << genVertexArrayChecks[i].line << endl;
			found = true;
		}
	}

	for (size_t i = 0; i < genTextureCheck.size(); i++)
	{
		if (genTextureCheck[i].free == false)
		{
			cout << "glGenTextures\n" << genTextureCheck[i].file << " : " << genTextureCheck[i].line << endl;
			found = true;
		}
	}

	if (found)
	{
		// if break here u have gpu mem leaks, press break and check console
		DebugBreak();
	}

}

#undef glGenBuffers
#undef glGenVertexArrays
#undef glGenTextures

#undef glDeleteBuffers
#undef glDeleteVertexArrays
#undef glDeleteTextures

#define glGenBuffers(n,i)			glGenBuffers_D(n,i, __FILE__, __LINE__)
#define glGenVertexArrays(n,i)		glGenVertexArray_D(n,i, __FILE__, __LINE__)
#define glGenTextures(n,i)			glGenTexture_D(n,i, __FILE__, __LINE__)

#define glDeleteBuffers(n,i)		glDeleteBuffers_D(n,i)
#define glDeleteVertexArrays(n,i)	glDeleteVertexArray_D(n,i)
#define glDeleteTextures(n,i)		glDeleteTexture_D(n,i)