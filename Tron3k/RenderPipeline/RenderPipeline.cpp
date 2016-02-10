#include "RenderPipeline.h"
#include <iostream>
#include <GL\glew.h>

#include "Shader.h"
#include "Utils\GPUMemoryLeakChecker.h"

#include <vld.h>

#include <sstream>

#include "Utils\TimeQuery.h"

#ifdef _DEBUG
extern "C"
{
	void __stdcall openglCallbackFunction(GLenum source,
		GLenum type,
		GLuint id,
		GLenum severity,
		GLsizei length,
		const GLchar* message,
		void* userParam)
	{
		if (severity == GL_DEBUG_SEVERITY_LOW || severity == GL_DEBUG_SEVERITY_MEDIUM || severity == GL_DEBUG_SEVERITY_HIGH)
		{
			printf("---------------------opengl-callback-start------------\n");
			printf("message: %s\n", message);
			printf("type: ");
			switch (type) {
			case GL_DEBUG_TYPE_ERROR:
				printf("ERROR");
				break;
			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
				printf("DEPRECATED_BEHAVIOR");
				break;
			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
				printf("UNDEFINED_BEHAVIOR");
				break;
			case GL_DEBUG_TYPE_PORTABILITY:
				printf("PORTABILITY");
				break;
			case GL_DEBUG_TYPE_PERFORMANCE:
				printf("PERFORMANCE");
				break;
			case GL_DEBUG_TYPE_OTHER:
				printf("OTHER");
				break;
			}
			printf("\n");

			printf("id: %d\n", id);
			printf("severity: ");
			switch (severity) {
			case GL_DEBUG_SEVERITY_LOW:
				printf("LOW");
				break;
			case GL_DEBUG_SEVERITY_MEDIUM:
				printf("MEDIUM");
				break;
			case GL_DEBUG_SEVERITY_HIGH:
				printf("HIGH");
				break;
			}
			printf("\n");
			printf("---------------------opengl-callback-end--------------\n");
		}
	}
}
#endif

int fps = 0;

bool RenderPipeline::init(unsigned int WindowWidth, unsigned int WindowHeight)
{
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	if (initialized)
	{
		return true;
	}

	GLenum result = glewInit();
	if (result != GLEW_OK)
	{
		return false;
	}

	cam.init(WindowWidth, WindowHeight);

	contMan.init();

	gBuffer = new Gbuffer();
	
	Text::ScreenResHeight = WindowHeight;
	Text::ScreenResWidth = WindowWidth;

	fontTexture = loadTexture("GameFiles/Font/font16.png", false);

	debugText = new Text("", 16, fontTexture, vec2(10, 24));
	chatHistoryText = ".\n.\n.\n.\n.\n";
	chatTypeText = "..";
	chatText = new Text(chatHistoryText + chatTypeText, 11, fontTexture, vec2(10, 420));

	uglyCrosshairSolution = new Text("+", 32, fontTexture, vec2(WindowWidth / 2 - 10, WindowHeight / 2 + 18));


#ifdef _DEBUG
	if (glDebugMessageCallback) {
		printf("Register OpenGL debug callback\n");
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback((GLDEBUGPROC)openglCallbackFunction, nullptr);
		GLuint unusedIds = 0;
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, &unusedIds, true);
	}
	else
		printf("glDebugMessageCallback not available\n");
#endif

	glClearColor(0.4f, 0.4f, 0.4f, 1);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE); 

	reloadShaders();

	cam.setViewProjMat(regularShader, viewProjMat[0]);
	cam.setViewProjMat(animationShader, viewProjMat[1]);
	cam.setViewMat(regularShader, viewMat);

	gBuffer->init(WindowWidth, WindowHeight, 5, true);

	for (size_t i = 0; i < 5*AnimationState::none; i++)
	{
		anims.keyFrameLenghts[i] = contMan.keyFrameLengths[i];
	}
	
	//light wall init INIT 2 points then change all info though uniforms to build quads
	glGenBuffers(1, &lwVertexDataId);
	glBindBuffer(GL_ARRAY_BUFFER, lwVertexDataId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 2 , &lwPosDefault[0], GL_STATIC_DRAW);
	glGenVertexArrays(1, &lwVertexAttribute);
	glBindVertexArray(lwVertexAttribute);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, BUFFER_OFFSET(0));

	uiQuad.Init(vec3(-1, -1, 0), vec3(1, 1, 0));

	//glGenBuffers(1, &decal_struct_UBO);
	//int maxDecals = 50;
	//glBindBuffer(GL_UNIFORM_BUFFER, decal_struct_UBO);
	//glBufferData(GL_UNIFORM_BUFFER, maxDecals * sizeof(float) * 12, NULL, GL_DYNAMIC_DRAW);

	resetQuery();

	initialized = true;
	return true;
}

void RenderPipeline::reloadShaders()
{
	std::cout << "Loading shaders\n";

	GLuint temp;

	if(gBuffer->shaderPtr == nullptr)
		gBuffer->shaderPtr = new GLuint();
	std::string shaderNamesDeffered[] = { "GameFiles/Shaders/BlitLightShader_vs.glsl", "GameFiles/Shaders/BlitLightShader_fs.glsl" };
	GLenum shaderTypesDeffered[] = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER };
	CreateProgram(temp, shaderNamesDeffered, shaderTypesDeffered, 2);
	if (temp != 0)
	{
		*gBuffer->shaderPtr = temp;
		temp = 0;
	}

	//portal shader
	if (gBuffer->portal_shaderPtr == nullptr)
		gBuffer->portal_shaderPtr = new GLuint();
	std::string shaderPortalRender[] = { "GameFiles/Shaders/portal_vs.glsl", "GameFiles/Shaders/portal_fs.glsl" };
	GLenum shaderTypesPortal[] = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER };
	CreateProgram(temp, shaderPortalRender, shaderTypesPortal, 2);
	if (temp != 0)
	{
		*gBuffer->portal_shaderPtr = temp;
		temp = 0;
	}

	//portal shader v2
	std::string shaderNamesPortal2[] = { "GameFiles/Shaders/portal_mat_vs.glsl", "GameFiles/Shaders/portal_mat_gs.glsl", "GameFiles/Shaders/portal_mat_fs.glsl" };
	GLenum shaderTypesPortal2[] = { GL_VERTEX_SHADER, GL_GEOMETRY_SHADER, GL_FRAGMENT_SHADER };
	CreateProgram(temp, shaderNamesPortal2, shaderTypesPortal2, 3);
	if (temp != 0)
	{
		portalShaderV2 = temp;
		temp = 0;
	}
	//portal2 uniforms
	portal_World = glGetUniformLocation(portalShaderV2, "world");
	portal_VP = glGetUniformLocation(portalShaderV2, "vp");


	std::string shaderNamesRegular[] = { "GameFiles/Shaders/RegularShader_vs.glsl", "GameFiles/Shaders/RegularShader_gs.glsl", "GameFiles/Shaders/RegularShader_fs.glsl" };
	GLenum shaderTypesRegular[] = { GL_VERTEX_SHADER, GL_GEOMETRY_SHADER, GL_FRAGMENT_SHADER };
	CreateProgram(temp, shaderNamesRegular, shaderTypesRegular, 3);
	if (temp != 0)
	{
		regularShader = temp;
		temp = 0;
	}

	//Skeleton Animation Shader
	std::string shaderNamesAnimation[] = { "GameFiles/Shaders/SkeletonAnimation_vs.glsl", "GameFiles/Shaders/SkeletonAnimation_fs.glsl" };
	GLenum shaderTypesAnimation[] = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER };
	CreateProgram(temp, shaderNamesAnimation, shaderTypesAnimation, 2);
	if (temp != 0)
	{
		animationShader = temp;
		temp = 0;
	}

	//Glow Shader
	std::string shaderNamesGlow[] = { "GameFiles/Shaders/GlowFade_vs.glsl", "GameFiles/Shaders/GlowFade_fs.glsl" };
	GLenum shaderTypesGlow[] = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER };
	CreateProgram(temp, shaderNamesGlow, shaderTypesGlow, 2);
	if (temp != 0)
	{
		glowShaderTweeks = temp;
		temp = 0;
	}

	//UI shader
	std::string shaderNamesUI[] = { "GameFiles/Shaders/uiShader_vs.glsl", "GameFiles/Shaders/uiShader_fs.glsl" };
	GLenum shaderTypesUI[] = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER };
	CreateProgram(temp, shaderNamesUI, shaderTypesUI, 2);
	if (temp != 0)
	{
		uiShader = temp;
		temp = 0;
	}

	//Decal shader
	std::string shaderNamesDecal[] = { "GameFiles/Shaders/decal_shader_vs.glsl", "GameFiles/Shaders/decal_shader_gs.glsl", "GameFiles/Shaders/decal_shader_fs.glsl" };
	GLenum shaderTypesDecal[] = { GL_VERTEX_SHADER, GL_GEOMETRY_SHADER, GL_FRAGMENT_SHADER };
	CreateProgram(temp, shaderNamesDecal, shaderTypesDecal, 3);
	if (temp != 0)
	{
		decal_Shader = temp;
		temp = 0;
	}

	//UI shaderLocations
	ui_Texture = glGetUniformLocation(uiShader, "textureSample");
	ui_World = glGetUniformLocation(uiShader, "WorldMatrix");
	uniformPivotLocation = glGetUniformLocation(uiShader, "pivot");

	//decal uniforms
	//http:/d/www.opentk.com/node/2926
	//decal_struct_UBO_index = glGetUniformBlockIndex(decal_Shader, "decal_in");
	//decal_nrDecals = glGetUniformLocation(decal_Shader, "NrDecals");
	decal_viewProj = glGetUniformLocation(decal_Shader, "ViewProjMatrix");
	decal_pos = glGetUniformLocation(decal_Shader, "pos");
	decal_normal = glGetUniformLocation(decal_Shader, "normal");
	decal_color = glGetUniformLocation(decal_Shader, "color");

	decal_inten = glGetUniformLocation(decal_Shader, "inten");
	//decal_Uniformtexsample = glGetUniformLocation(decal_Shader, "texsample");

	worldMat[0] = glGetUniformLocation(regularShader, "WorldMatrix"); //worldMat regular shader
	viewMat = glGetUniformLocation(regularShader, "ViewMatrix"); //view
	viewProjMat[0] = glGetUniformLocation(regularShader, "ViewProjMatrix"); //view regular shader

	worldMat[1] = glGetUniformLocation(animationShader, "WorldMatrix"); //worldMat animation shader
	viewProjMat[1] = glGetUniformLocation(animationShader, "ViewProjMatrix"); //view animation shader

	uniformTextureLocation[0] = glGetUniformLocation(regularShader, "textureSample"); //view regular shader
	uniformNormalLocation[0] = glGetUniformLocation(regularShader, "normalSample"); //view regular shader
	uniformGlowSpecLocation[0] = glGetUniformLocation(regularShader, "glowSpecSample"); //view regular shader

	uniformTextureLocation[1] = glGetUniformLocation(animationShader, "textureSample"); //view animation shader
	uniformNormalLocation[1] = glGetUniformLocation(animationShader, "normalSample"); //view animation shader
	uniformGlowSpecLocation[1] = glGetUniformLocation(animationShader, "glowSpecSample"); //view animation shader

	cam.setViewMat(regularShader, viewMat);
	worldMat[0] = glGetUniformLocation(regularShader, "WorldMatrix"); //worldMat regular shader
	viewMat = glGetUniformLocation(regularShader, "ViewProjMatrix"); //view

	worldMat[1] = glGetUniformLocation(animationShader, "WorldMatrix"); //worldMat animation shader

	uniformTextureLocation[0] = glGetUniformLocation(regularShader, "textureSample"); //view regular shader
	uniformTextureLocation[1] = glGetUniformLocation(animationShader, "textureSample"); //view animation shader

	uniformDynamicGlowColorLocation[0] = glGetUniformLocation(regularShader, "dynamicGlowColor"); //regular shader
	uniformStaticGlowIntensityLocation[0] = glGetUniformLocation(regularShader, "staticGlowIntensity"); //regular shader
	uniformGlowTrail[0] = glGetUniformLocation(regularShader, "trail"); //regular shader

	uniformDynamicGlowColorLocation[1] = glGetUniformLocation(animationShader, "dynamicGlowColor"); //animation shader
	uniformStaticGlowIntensityLocation[1] = glGetUniformLocation(animationShader, "staticGlowIntensity"); //animation shader
	uniformGlowTrail[1] = glGetUniformLocation(animationShader, "trail"); //animation shader

	uniformKeyMatrixLocation = glGetUniformBlockIndex(animationShader, "boneMatrices");

	uniformGlowTimeDelta = glGetUniformLocation(glowShaderTweeks, "deltaTime");
	uniformGlowFalloff = glGetUniformLocation(glowShaderTweeks, "falloff");
	uniformGlowTexture = glGetUniformLocation(glowShaderTweeks, "glowAdd");
	uniformGlowSelf = glGetUniformLocation(glowShaderTweeks, "self");

	//build shader
	std::string shaderNamesLW[] = { "GameFiles/Shaders/lw_shader_vs.glsl", "GameFiles/Shaders/lw_shader_gs.glsl", "GameFiles/Shaders/lw_shader_fs.glsl" };
	GLenum shaderTypesLW[] = { GL_VERTEX_SHADER, GL_GEOMETRY_SHADER, GL_FRAGMENT_SHADER };
	CreateProgram(temp, shaderNamesLW, shaderTypesLW, 3);
	if (temp != 0)
	{
		lw_Shader = temp;
		temp = 0;
	}
	//get uniform locations
	lw_pos1 = glGetUniformLocation(lw_Shader, "pos1");
	lw_pos2 = glGetUniformLocation(lw_Shader, "pos2");
	lw_uv1 = glGetUniformLocation(lw_Shader, "uv1");
	lw_uv2 = glGetUniformLocation(lw_Shader, "uv2");
	lw_vp = glGetUniformLocation(lw_Shader, "ViewProjMatrix");
	lw_tex = glGetUniformLocation(lw_Shader, "texsample");


	uniformDynamicGlowColorLocation_wall = glGetUniformLocation(lw_Shader, "dynamicGlowColor");

	std::string textShaderNames[] = { "GameFiles/Shaders/simple_vs.glsl",  "GameFiles/Shaders/simple_fs.glsl" };
	GLenum textshaderTypes[] = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER };
	CreateProgram(temp, textShaderNames, textshaderTypes, 2);
	if (temp != 0)
	{
		textShader = temp;
		temp = 0;
	}

	textShaderLocation = glGetUniformLocation(textShader, "textureIN");
	textShaderModel = glGetUniformLocation(textShader, "worldMat");
	textShaderVP = glGetUniformLocation(textShader, "view");
	textShaderOffset = glGetUniformLocation(textShader, "offset");


	std::cout << "Done loading shaders\n";

}

void RenderPipeline::release()
{
	// place delete code here

	glDeleteBuffers(1, &lwVertexDataId);
	glDeleteVertexArrays(1, &lwVertexAttribute);

	glDeleteShader(lw_Shader);
	glDeleteShader(regularShader);
	glDeleteShader(animationShader);
	glDeleteShader(uiShader);
	glDeleteShader(decal_Shader);
	uiQuad.release();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	delete debugText;
	delete chatText;

	delete uglyCrosshairSolution;


	delete gBuffer;

	contMan.release();

	reportGPULeaks();

	delete this; // yes this is safe
}

void RenderPipeline::update(float x, float y, float z, float dt)
{
	timepass += dt;
	delta = dt;
	//set camera matrixes
	cam.setViewMat(regularShader, viewMat);
	cam.setViewProjMat(regularShader, viewProjMat[0]);

	cam.setViewProjMat(animationShader, viewProjMat[1]);
	cam.setViewProjMat(*gBuffer->portal_shaderPtr, gBuffer->portal_vp);
	cam.setViewProjMat(portalShaderV2, portal_VP);

	contMan.update(dt);

	updateTakeDamageEffect(dt);

	gBuffer->eyePosLast = gBuffer->eyePos;
	gBuffer->eyePos.x = x;
	gBuffer->eyePos.y = y;
	gBuffer->eyePos.z = z;

	gBuffer->clearLights();
	
	std::stringstream ss;

	terminateQuery();
	
	if (renderDebugText)
	{
		ss << "Draw count: " << drawCount << "\n";
		ss << "Primitive count: " << primitiveCount << "\n";
		//ss << "Buffer count: " << genBufferPeak << "\n";
		//ss << "Vao count: " << genVaoPeak << "\n";
		//ss << "Texture count: " << genTexturePeak << "\n";
		//ss << "Memory usage: " << memusage << "(B)\n";
		ss << "Memory usage: " << memusage / 1024.0f / 1024.0f << "(MB)\n";
		ss << "FPS: " << fps << "\n";
		//ss << "Texture binds: " << textureBinds << "\n";
		//ss << "Buffer binds: " << bufferBinds << "\n";
		//ss << "Shader binds: " << shaderBinds << "\n";
		//ss << "State changes: " << stateChange << "\n";
		//ss << "Total uptime:" << timepass << "\n";
		//ss << result << "\n";
		if (counter > 1.0f)
		{
			//result = getQueryResult();
			fps = 0;
			counter = 0;
			debugText->setText(ss.str());
		}
		fps++;
	}

	resetQuery();

	drawCount = 0;
	primitiveCount = 0;
	textureBinds = 0;
	bufferBinds = 0;
	shaderBinds = 0;
	stateChange = 0;

	startTimer("Frame : Total");
	renderFrameTimeID = startTimer("Frame : Render only");

	counter += dt;

}

int RenderPipeline::portalIntersection(float* pos1, float* pos2, int in_chunk)
{
	if (contMan.f_portal_culling)
		return contMan.getPortalID((vec3*)pos1, (vec3*)pos2, in_chunk);
	else
		return -1;
}

void RenderPipeline::setCullingCurrentChunkID(int roomID)
{
	contMan.setRoomID(roomID);
}

void RenderPipeline::addLight(SpotLight* newLight, int roomID)
{
	if (contMan.f_portal_culling)
	{
		if(contMan.renderedChunks[roomID] == true)
			gBuffer->pushLights(newLight, 1);
	}
	else
		gBuffer->pushLights(newLight, 1);
}

void RenderPipeline::renderIni()
{
	glUseProgram(regularShader);
	gBuffer->bind(GL_FRAMEBUFFER);
	
	gBuffer->clearBuffers();

}

void RenderPipeline::render()
{
	int chunkRender = startTimer("Chunks (render)");

	glProgramUniform1f(regularShader, uniformGlowTrail[0], 0.0f);

	contMan.renderChunks(regularShader, worldMat[0], uniformTextureLocation[0], uniformNormalLocation[0], uniformGlowSpecLocation[0], uniformDynamicGlowColorLocation[0], uniformStaticGlowIntensityLocation[0],  *gBuffer->portal_shaderPtr, gBuffer->portal_model, portalShaderV2, portal_World);
	
	stopTimer(chunkRender);
	//glDepthMask(GL_TRUE);glEnable(GL_CULL_FACE);glDisable(GL_BLEND);)
	//renderEffects();
}

void RenderPipeline::finalizeRender()
{
	glDepthMask(GL_TRUE);
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);

	//system("CLS");

	//push the lights of the rendered chunks
	for (int n = 0; n < contMan.nrChunks; n++)
		if (contMan.renderedChunks[n] == true)
		{
			//if (n > 0)
			{
				int count = contMan.testMap.chunks[n].lights.size();
				if (count > 0)
					gBuffer->pushLights(&contMan.testMap.chunks[n].lights[0], count);
			//printf("Chunk : %d Lights: %d \n", n, count);
			}
		}
	

	glUseProgram(glowShaderTweeks);
	
	glProgramUniform1fv(glowShaderTweeks, uniformGlowTimeDelta, 1, &delta);

	gBuffer->preRender(glowShaderTweeks, uniformGlowTexture, uniformGlowSelf);

	//GBuffer Render
	glBindFramebuffer(GL_FRAMEBUFFER, NULL);
	glClear(GL_DEPTH_BUFFER_BIT);

	gBuffer->render();
	
	glUseProgram(textShader);
	glProgramUniformMatrix4fv(textShader, textShaderModel, 1, GL_FALSE, (GLfloat*)&glm::mat4());
	glProgramUniformMatrix4fv(textShader, textShaderVP, 1, GL_FALSE, (GLfloat*)&glm::mat4());
	glProgramUniform3f(textShader, textShaderOffset,  0, 0, 0);
	
	TextureInfo asd;
	asd.lastTextureSlot = GL_TEXTURE0;
	asd.state = TEXTURE_LOADED;
	asd.textureID = fontTexture;

	TextureManager::gTm->bind(asd, textShader, textShaderLocation);//bindTexture(fontTexture, textShader, textShaderLocation, DIFFUSE_FB);
	
	glEnable(GL_BLEND);

	debugText->draw();
	chatText->draw();
	
	uglyCrosshairSolution->draw();

	glDisable(GL_BLEND);

	stopTimer(renderFrameTimeID);

}

void RenderPipeline::renderWallEffect(void* pos1, void* pos2, float uvStartOffset, float* dgColor)
{
	glUseProgram(lw_Shader);
	//glProgramUniform1i(lw_Shader, lw_tex, 0);
	//call contentman and bind the lightwal texture to 0
	contMan.bindLightwalTexture(lw_Shader, lw_tex);
	cam.setViewProjMat(lw_Shader, lw_vp);
	glProgramUniform3fv(lw_Shader, uniformDynamicGlowColorLocation_wall, 1, (GLfloat*)&dgColor[0]);

	glBindBuffer(GL_ARRAY_BUFFER, lwVertexDataId);
	glBindVertexArray(lwVertexAttribute);

	float of = timepass * 0.4f;

	glm::vec3 wpos1 = *(glm::vec3*)pos1;
	glm::vec3 wpos2 = *(glm::vec3*)pos2;

	float dist = glm::distance(wpos1, wpos2) / 2.0f;

	vec2 uv1 = vec2(uvStartOffset - of, 0);
	vec2 uv2 = vec2(uvStartOffset - dist - of, 0);
	glProgramUniform2fv(lw_Shader, lw_uv1, 1, &uv1[0]);
	glProgramUniform2fv(lw_Shader, lw_uv2, 1, &uv2[0]);
	glProgramUniform3fv(lw_Shader, lw_pos1, 1, &wpos1[0]);
	glProgramUniform3fv(lw_Shader, lw_pos2, 1, &wpos2[0]);

	glDrawArrays(GL_POINTS, 0, 1);

}

void RenderPipeline::renderExploEffect(float* pos, float rad, float transp, float* dgColor)
{
	glUseProgram(regularShader);

	//Glow values for player
	glProgramUniform1f(regularShader, uniformStaticGlowIntensityLocation[0], transp);
	glProgramUniform3fv(regularShader, uniformDynamicGlowColorLocation[0], 1, (GLfloat*)&dgColor[0]);

	glProgramUniform1f(regularShader, uniformGlowTrail[0], 0.3f);

	glProgramUniform1i(regularShader, uniformTextureLocation[0], 0);
	glProgramUniform1i(regularShader, uniformNormalLocation[0], 1);
	glProgramUniform1i(regularShader, uniformGlowSpecLocation[0], 2);

	//set temp objects worldmat
	glm::mat4 mat;

	mat[0].w = pos[0];
	mat[1].w = pos[1];
	mat[2].w = pos[2];

	mat[0].x = rad;
	mat[1].y = rad;
	mat[2].z = rad;

	glProgramUniformMatrix4fv(regularShader, worldMat[0], 1, GL_FALSE, (GLfloat*)&mat[0][0]);

	contMan.renderBullet(GRENADE_SHOT);
}

void RenderPipeline::renderThunderDomeEffect(float* pos, float rad, float transp, float* dgColor)
{
	glUseProgram(regularShader);

	//Glow values for player
	glProgramUniform1f(regularShader, uniformStaticGlowIntensityLocation[0], transp);
	glProgramUniform3fv(regularShader, uniformDynamicGlowColorLocation[0], 1, (GLfloat*)&dgColor[0]);

	glProgramUniform1f(regularShader, uniformGlowTrail[0], 0.3f);

	glProgramUniform1i(regularShader, uniformTextureLocation[0], 0);
	glProgramUniform1i(regularShader, uniformNormalLocation[0], 1);
	glProgramUniform1i(regularShader, uniformGlowSpecLocation[0], 2);

	//set temp objects worldmat
	glm::mat4 mat;

	mat[0].w = pos[0];
	mat[1].w = pos[1];
	mat[2].w = pos[2];

	mat[0].x = rad;
	mat[1].y = rad;
	mat[2].z = rad;

	glProgramUniformMatrix4fv(regularShader, worldMat[0], 1, GL_FALSE, (GLfloat*)&mat[0][0]);

	contMan.renderThunderDome();
}

void RenderPipeline::renderDecals(void* data, int size)
{
	if (size > 0)
	{
		glUseProgram(decal_Shader);
		//glBindBuffer(GL_UNIFORM_BUFFER, decal_struct_UBO);
		//glBufferData(GL_UNIFORM_BUFFER, sizeof(float) * 12 * size, data, GL_DYNAMIC_DRAW);
		//glBindBufferBase(GL_UNIFORM_BUFFER, decal_struct_UBO_index, decal_struct_UBO);
		//glProgramUniform1i(decal_Shader, decal_nrDecals, size);
		cam.setViewProjMat(decal_Shader, decal_viewProj);
		contMan.bindDecalTexture();
		glBindBuffer(GL_ARRAY_BUFFER, lwVertexDataId);
		glBindVertexArray(lwVertexAttribute);

		Decal_RenderInfo_temp* decals = (Decal_RenderInfo_temp*)data;

		for (int n = 0; n < size; n++)
		{
			glProgramUniform3fv(decal_Shader, decal_pos, 1, &decals[n].pos[0]);
			glProgramUniform3fv(decal_Shader, decal_normal, 1, &decals[n].normal[0]);
			glProgramUniform3fv(decal_Shader, decal_color, 1, &decals[n].color[0]);
			glProgramUniform1f(decal_Shader, decal_inten, decals[n].inten);

			glDrawArrays(GL_POINTS, 0, 1);
		}
		
	}
}

void RenderPipeline::renderEffects()
{
	//glUseProgram(lw_Shader);
	//glProgramUniform1i(lw_Shader, lw_tex, 0);
	////call contentman and bind the lightwal texture to 0
	//contMan.bindLightwalTexture();
	//cam.setViewProjMat(lw_Shader, lw_vp);
	//
	//glBindBuffer(GL_ARRAY_BUFFER, lwVertexDataId);
	//glBindVertexArray(lwVertexAttribute);
	//
	//float of = timepass * 0.1f;
	//
	//for (size_t i = 0; i < lw.playerWall.size(); i++)
	//{
	//	for (size_t w = 0; w < lw.playerWall[i].lightQuad.size(); w++)
	//	{
	//		vec2 uv1 = lw.playerWall[i].lightQuad[w].uv1 + vec2(of, 0);
	//		vec2 uv2 = lw.playerWall[i].lightQuad[w].uv2 + vec2(of, 0);
	//		glProgramUniform2fv(lw_Shader, lw_uv1, 1, &uv1[0]);
	//		glProgramUniform2fv(lw_Shader, lw_uv2, 1, &uv2[0]);
	//		glProgramUniform3fv(lw_Shader, lw_pos1, 1, &lw.playerWall[i].lightQuad[w].pos1[0]);
	//		glProgramUniform3fv(lw_Shader, lw_pos2, 1, &lw.playerWall[i].lightQuad[w].pos2[0]);
	//
	//		glDrawArrays(GL_POINTS, 0, 2);
	//	}
	//}

	//vec2 uv1 = vec2(of, 0);
	//vec2 uv2 = vec2(3 + of, 0);
	//glProgramUniform2fv(lw_Shader, lw_uv1, 1, &uv1[0]);
	//glProgramUniform2fv(lw_Shader, lw_uv2, 1, &uv2[0]);
	//vec3 pos1 = vec3(0, 3, 0);
	//vec3 pos2 = vec3(0, 3, 6);
	//glProgramUniform3fv(lw_Shader, lw_pos1, 1, &pos1[0]);
	//glProgramUniform3fv(lw_Shader, lw_pos2, 1, &pos2[0]);
	//
	//glDrawArrays(GL_POINTS, 0, 2);
	
	//uv1 = vec2(3 + of, 0);
	//uv2 = vec2(5 + of, 0);
	//glProgramUniform2fv(lw_Shader, lw_uv1, 1, &uv1[0]);
	//glProgramUniform2fv(lw_Shader, lw_uv2, 1, &uv2[0]);
	////pos1 = vec3(0, 3, 6);
	////pos2 = vec3(0, 5, 8);
	//glProgramUniform3fv(lw_Shader, lw_pos1, 1, &pos1[0]);
	//glProgramUniform3fv(lw_Shader, lw_pos2, 1, &pos2[0]);
	//
	//glDrawArrays(GL_POINTS, 0, 2);
}

void* RenderPipeline::getView()
{
	return (void*)cam.getViewMat();
}

void RenderPipeline::setChunkColorAndInten(int ID, float* color, float inten)
{
	//contMan.testMap.chunks[ID].color = { color[0], color[1], color[2] };
	//contMan.testMap.chunks[ID].staticIntes = inten;
}

void RenderPipeline::renderMISC(int miscID, void* world, float* dgColor, float sgInten)
{
	glUseProgram(regularShader);

	//Glow values for player
	glProgramUniform1f(regularShader, uniformStaticGlowIntensityLocation[0], sgInten);
	glProgramUniform3fv(regularShader, uniformDynamicGlowColorLocation[0], 1, (GLfloat*)&dgColor[0]);

	if(miscID != -3)
		glProgramUniform1f(regularShader, uniformGlowTrail[0], 1.0f);
	else
		glProgramUniform1f(regularShader, uniformGlowTrail[0], 0.0f);

	//set temp objects worldmat
	glProgramUniformMatrix4fv(regularShader, worldMat[0], 1, GL_FALSE, (GLfloat*)world);

	contMan.renderMisc(miscID, regularShader, uniformTextureLocation[0], uniformNormalLocation[0], uniformGlowSpecLocation[0]);
}

void RenderPipeline::renderBullet(int bid, void* world, float* dgColor, float sgInten)
{
	glUseProgram(regularShader);

	//Glow values for player
	glProgramUniform1f(regularShader, uniformStaticGlowIntensityLocation[0], sgInten);
	glProgramUniform3fv(regularShader, uniformDynamicGlowColorLocation[0], 1, (GLfloat*)&dgColor[0]);

	if (bid == BULLET_TYPE::PULSE_SHOT)
		glProgramUniform1f(regularShader, uniformGlowTrail[0], 1.0f);
	else
		glProgramUniform1f(regularShader, uniformGlowTrail[0], 0.0f);

	glProgramUniform1i(regularShader, uniformTextureLocation[0], 0);
	glProgramUniform1i(regularShader, uniformNormalLocation[0], 1);
	glProgramUniform1i(regularShader, uniformGlowSpecLocation[0], 2);

	//set temp objects worldmat
	glProgramUniformMatrix4fv(regularShader, worldMat[0], 1, GL_FALSE, (GLfloat*)world);

	contMan.renderBullet(bid);
}

void RenderPipeline::renderCapturePoint(int capPointID)
{
	glDisable(GL_BLEND);
	glUseProgram(regularShader);
	glProgramUniform1f(regularShader, uniformStaticGlowIntensityLocation[0], 1.0f);
	glProgramUniform3fv(regularShader, uniformDynamicGlowColorLocation[0], 1, (GLfloat*)contMan.testMap.getCapPointColor(capPointID));
	contMan.renderCapturePoint(capPointID, regularShader, worldMat[0], uniformTextureLocation[0], uniformNormalLocation[0], uniformGlowSpecLocation[0]);
}

void RenderPipeline::renderAnimation(int playerID, int roleID, void* world, AnimationState animState, float* dgColor, float sgInten, bool first, bool primary, int roomID)
{
	anims.updateAnimStates(playerID, roleID, animState, delta, first);

	if (contMan.renderedChunks[roomID] == true || contMan.f_portal_culling == false)
	{

		glUseProgram(animationShader);

		//Texture for the glow
		//glProgramUniform1i(animationShader, uniformTextureLocation[1], 0);
		//glProgramUniform1i(animationShader, uniformNormalLocation[1], 1);
		//glProgramUniform1i(animationShader, uniformGlowSpecLocation[1], 2);

		//Glow values for player
		glProgramUniform1f(animationShader, uniformStaticGlowIntensityLocation[1], sgInten);
		glProgramUniform3fv(animationShader, uniformDynamicGlowColorLocation[1], 1, (GLfloat*)&dgColor[0]);
		glProgramUniform1f(animationShader, uniformGlowTrail[1], 0.0f);

		//set temp objects worldmat
		glProgramUniformMatrix4fv(animationShader, worldMat[1], 1, GL_FALSE, (GLfloat*)world);

		if (anims.animStates[playerID].state != AnimationState::none && anims.animStates[playerID].frameEnd > 0)
		{
			AnimManager::animState state = anims.animStates[playerID];
			contMan.renderPlayer(anims.animStates[playerID], *(glm::mat4*)world, uniformKeyMatrixLocation, first, primary, animationShader, uniformTextureLocation[1], uniformNormalLocation[1], uniformGlowSpecLocation[1]);
		}
	}
}

bool RenderPipeline::setSetting(PIPELINE_SETTINGS type, PipelineValues value)
{
	if (value.type != getType(type))
	{
		return false;
	}
	else
	{

	}
	return true;
}

SETTING_INPUT RenderPipeline::getType(PIPELINE_SETTINGS type) const
{

	switch (type) {
	case PIPELINE_SETTINGS::CLEAR_COLOR:
		return SETTING_INPUT::CLEAR_COLOR_IN;
	case PIPELINE_SETTINGS::GLOW:
		return SETTING_INPUT::GLOW_IN;
	case PIPELINE_SETTINGS::SHADOWS:
		return SETTING_INPUT::SHADOWS_IN;
	case PIPELINE_SETTINGS::AO:
		return SETTING_INPUT::AO_IN;
	case PIPELINE_SETTINGS::VIEWPORT:
		return SETTING_INPUT::VIEWPORT_IN;
	case PIPELINE_SETTINGS::REDUCEPARTICLES:
		return SETTING_INPUT::REDUCEPARTICLES_IN;
	case PIPELINE_SETTINGS::DOF:
		return SETTING_INPUT::DOF_IN;
	case PIPELINE_SETTINGS::DOF_VALUE:
		return SETTING_INPUT::DOF_VALUE_IN;
	default:
		break;
	}

	return SETTING_INPUT::NONE_IN;
}

void* RenderPipeline::getChunkCollisionVectorAsPoint(int chunkID)
{
	return contMan.getChunkCollisionVectorAsPointer(chunkID);
}

void* RenderPipeline::getCapPointsAsPoint(int& count)
{
	return contMan.getCapAsPointer(count);


}

void* RenderPipeline::getRoomBoxes()
{
	return contMan.getRoomBoxes();
}

void RenderPipeline::getPlayerBox(float &xMax, float &xMin, float &yMax, float &yMin, float &zMax, float &zMin)
{
	//Will be changed later, this is number for now
	xMax = 0.5f;
	xMin = -0.5f;
	yMax = 0.3f;  //distance from camera to head max
	yMin = -1.55f; //distance from camera to ground
	zMax = 0.5f;
	zMin = -0.5f;
}

void RenderPipeline::getWorldBoxes(int &current, float &xMax, float &xMin, float &yMax, float &yMin, float &zMax, float &zMin)
{
	std::vector<std::vector<float>> wBoxes = contMan.getMeshBoxes();
	xMax = wBoxes[current][0];
	xMin = wBoxes[current][1];
	yMax = wBoxes[current][2];
	yMin = wBoxes[current][3];
	zMax = wBoxes[current][4];
	zMin = wBoxes[current][5];
}

int RenderPipeline::getNrOfWorldBoxes()
{
	return contMan.getMeshBoxes().size();
}

IRenderPipeline* CreatePipeline()
{
	return new RenderPipeline();
}

void RenderPipeline::setGBufferWin(unsigned int WindowWidth, unsigned int WindowHeight)
{
	gBuffer->resize(WindowWidth, WindowHeight);
}

void RenderPipeline::setRenderFlag(RENDER_FLAGS flag)
{
	switch (flag)
	{
		case PORTAL_CULLING:	contMan.f_portal_culling = !contMan.f_portal_culling;		break;
		case FREEZE_CULLING:	contMan.f_freeze_portals = !contMan.f_freeze_portals;		break;
		case RENDER_CHUNK:		contMan.f_render_chunks = !contMan.f_render_chunks;			break;
		case RENDER_ABB:		contMan.f_render_abb = !contMan.f_render_abb;				break;
		case RENDER_OBB:		contMan.f_render_obb = !contMan.f_render_obb;				break;
		case RENDER_DEBUG_TEXT:	renderDebugText = !renderDebugText; debugText->setText(""); break;
		case RENDER_GUI:		contMan.f_render_gui = !contMan.f_render_gui;				break;
	}
}

void RenderPipeline::getSpawnpoints(std::vector < std::vector < SpawnpointG > > &spoints)
{
	//add teams
	vector<SpawnpointG> team0;
	vector<SpawnpointG> team1;
	vector<SpawnpointG> team2;

	float yincrease = 0.0f;

	int size = contMan.testMap.spFFACount;
	for (int n = 0; n < size; n++)
	{
		glm::mat4 mat = contMan.testMap.spFFA[n].transform;
		vec3 pos;
		pos.x = mat[0].w;
		pos.y = mat[1].w + yincrease;
		pos.z = mat[2].w;
		team0.push_back(SpawnpointG());
		team0[n].pos = pos;
		team0[n].dir = vec3(contMan.testMap.spFFA[n].dx, contMan.testMap.spFFA[n].dy, contMan.testMap.spFFA[n].dz);
		team0[n].roomID = contMan.testMap.spFFA[n].roomID;
	}
	spoints.push_back(team0);

	size = contMan.testMap.spTACount;
	for (int n = 0; n < size; n++)
	{
		glm::mat4 mat = contMan.testMap.spA[n].transform;
		vec3 pos;
		pos.x = mat[0].w;
		pos.y = mat[1].w + yincrease;
		pos.z = mat[2].w;
		team1.push_back(SpawnpointG());
		team1[n].pos = pos;
		team1[n].dir = vec3(contMan.testMap.spA[n].dx, contMan.testMap.spA[n].dy, contMan.testMap.spA[n].dz);
		team1[n].roomID = contMan.testMap.spA[n].roomID;
	}
	spoints.push_back(team1);

	size = contMan.testMap.spTBCount;
	for (int n = 0; n < size; n++)
	{
		glm::mat4 mat = contMan.testMap.spB[n].transform;
		vec3 pos;
		pos.x = mat[0].w;
		pos.y = mat[1].w + yincrease;
		pos.z = mat[2].w;
		team2.push_back(SpawnpointG());
		team2[n].pos = pos;
		team2[n].dir = vec3(contMan.testMap.spB[n].dx, contMan.testMap.spB[n].dy, contMan.testMap.spB[n].dz);
		team2[n].roomID = contMan.testMap.spB[n].roomID;
	}
	spoints.push_back(team2);

	//contMan.testMap.deleteSpawnposData();
}

void RenderPipeline::setChatHistoryText(std::string text)
{
	chatHistoryText = text;
	chatText->setText(chatHistoryText + chatTypeText);
}

void RenderPipeline::setChatTypeMessage(std::string text)
{
	chatTypeText = text;
	chatText->setText(chatHistoryText + chatTypeText);
}

void RenderPipeline::ui_initRender()
{
	//glBindFramebuffer(GL_FRAMEBUFFER, NULL);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(uiShader);
	//uniformlocation set texture 0  it defaults to 0 so not needed
	
	uiQuad.BindVertData();
}

void RenderPipeline::ui_InGameRenderInit()
{
	glEnable(GL_BLEND);
	glUseProgram(uiShader);
	//uniformlocation set texture 0  it defaults to 0 so not needed

	uiQuad.BindVertData();
}

void RenderPipeline::ui_loadTexture(unsigned int* texid, char* filepath, int* xres, int* yres)
{
	*texid = loadTexture(std::string(filepath), true, xres, yres);
}

void RenderPipeline::ui_renderQuad(float* mat, float* pivot, GLuint textureID, float transp, int i)
{
	if (contMan.f_render_gui)
	{
		glm::mat4* world = (glm::mat4*)mat;

		//glActiveTexture(GL_TEXTURE0);

		TextureInfo temp;
		temp.state = TEXTURE_LOADED;
		temp.lastTextureSlot = GL_TEXTURE0;
		temp.textureID = textureID;
		TextureManager::gTm->bind(temp, uiShader, ui_Texture);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glProgramUniformMatrix4fv(uiShader, ui_World, 1, GL_FALSE, mat);
		glProgramUniform3fv(uiShader, uniformPivotLocation, 1, pivot);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}
}

void RenderPipeline::ui_textureRelease(vector<unsigned int> texids)
{
	for (unsigned int n = 0; n < texids.size(); n++)
		glDeleteTextures(1, &texids[n]);
}


void RenderPipeline::enableDepthTest()
{
	glEnable(GL_DEPTH_TEST);
}

void RenderPipeline::disableDepthTest()
{
	glDisable(GL_DEPTH_TEST);
}

void RenderPipeline::enableBlend()
{
	glEnable(GL_BLEND);
}

void RenderPipeline::disableBlend()
{
	glDisable(GL_BLEND);
}

void RenderPipeline::clearBothBuffers()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderPipeline::clearColor()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

int RenderPipeline::startExecTimer(std::string name)
{
	return startTimer(name);
}

void RenderPipeline::stopExecTimer(int id)
{
	stopTimer(id);
}

bool* RenderPipeline::getRenderedChunks(int& get_size)
{
	get_size = contMan.nrChunks;
	return contMan.renderedChunks;
}

int RenderPipeline::createTextObject(std::string text, int fontSize, glm::vec2 pos)
{
	int id = textObjects.size();

	textObjects.push_back(new Text(text, fontSize, fontTexture, pos));

	return id;
}

void RenderPipeline::setTextObjectText(int id, std::string text)
{
	textObjects[id]->setText(text);
}

void RenderPipeline::setTextPos(int id, glm::vec2 pos)
{
	textObjects[id]->setPos(pos);
}

void RenderPipeline::removeTextObject(int id)
{
	delete textObjects[id];
}

void RenderPipeline::renderTextObject(int id)
{
	glUseProgram(textShader);
	glProgramUniformMatrix4fv(textShader, textShaderModel, 1, GL_FALSE, (GLfloat*)&glm::mat4());
	glProgramUniformMatrix4fv(textShader, textShaderVP, 1, GL_FALSE, (GLfloat*)&glm::mat4());
	glProgramUniform3f(textShader, textShaderOffset, 0, 0, 0);
	
	TextureInfo asd;
	asd.lastTextureSlot = GL_TEXTURE0;
	asd.state = TEXTURE_LOADED;
	asd.textureID = fontTexture;

	TextureManager::gTm->bind(asd, textShader, textShaderLocation);//TextureManager::gTm->bindTexture(fontTexture, textShader, textShaderLocation, DIFFUSE_FB);

	textObjects[id]->draw();

}

void RenderPipeline::renderTextObjectWorldPos(int id, glm::mat4 world)
{
	glUseProgram(textShader);
	glProgramUniformMatrix4fv(textShader, textShaderModel, 1, GL_FALSE, (GLfloat*)&world);
	cam.setViewProjMat(textShader, textShaderVP);
	//glProgramUniform3fv(textShader, textShaderOffset, 1, (GLfloat*)&textObjects[id]->getOffset()[0]);

	vec3 pos = textObjects[id]->getOffset();

	glProgramUniform3f(textShader, textShaderOffset, -pos.x ,0,0);

	TextureInfo asd;
	asd.lastTextureSlot = GL_TEXTURE0;
	asd.state = TEXTURE_LOADED;
	asd.textureID = fontTexture;

	TextureManager::gTm->bind(asd, textShader, textShaderLocation);//TextureManager::gTm->bindTexture(fontTexture, textShader, textShaderLocation, DIFFUSE_FB);

	textObjects[id]->draw();

}

void RenderPipeline::setCapRoomColor(int capPoint, vec3 color, float intensity)
{
	int roomID = contMan.testMap.getCapPointRoomID(capPoint);
	contMan.testMap.setCapPointColor(capPoint, color);
	//contMan.testMap.chunks[8].color = { color[0], color[1], color[2] };
	//contMan.testMap.chunks[8].staticIntes = intensity;

}

void RenderPipeline::startTakeDamageEffect(int maxDisplace, float time)
{
	takeDamage_startDispalce = maxDisplace;
	takeDamage_timer = time;
	takeDamage_timerStartValue = takeDamage_timer;
}

void RenderPipeline::updateTakeDamageEffect(float dt)
{
	if (takeDamage_timer > 0)
	{
		takeDamage_timer -= dt;
		if (takeDamage_timer > 0)
		{
			float precent = (takeDamage_timer / takeDamage_timerStartValue);
			gBuffer->setGlowSamplingDist(precent * takeDamage_startDispalce + 2);
		}
		else //timeout
			gBuffer->setGlowSamplingDist(2.0f);
	}
}

void RenderPipeline::renderMinimap(float* yourPos, float* yourdir, float* teammates, int nrOfTeammates, int team)
{
	//init render
	glUseProgram(uiShader);
	//uniformlocation set texture 0  it defaults to 0 so not needed
	uiQuad.BindVertData();
	glActiveTexture(GL_TEXTURE0);
	
	TextureInfo asd;
	asd.lastTextureSlot = GL_TEXTURE0;
	asd.state = TEXTURE_LOADED;
	asd.textureID = contMan.miniMapTexture;
	TextureManager::gTm->bind(asd, uiShader, ui_Texture);

	minimapRenderMat = mat4();
	//pos
	minimapRenderMat[0].w = 0;
	minimapRenderMat[1].w = 0;
	minimapRenderMat[2].w = 0;
	//scale
	minimapRenderMat[0].x = contMan.minimapscaleX;
	minimapRenderMat[1].y = contMan.minimapScaleY;

	vec3 piv(0);
	glProgramUniformMatrix4fv(uiShader, ui_World, 1, GL_FALSE, &minimapRenderMat[0][0]);
	glProgramUniform3fv(uiShader, uniformPivotLocation, 1, &piv[0]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	//render player markers

	asd.lastTextureSlot = GL_TEXTURE0;
	asd.state = TEXTURE_LOADED;
	asd.textureID = contMan.youarehereTexture;
	TextureManager::gTm->bind(asd, uiShader, ui_Texture);

	minimapRenderMat = mat4();

	//scale, rotate, translate

	//scale
	minimapRenderMat[0].x = contMan.youareherescaleX;
	minimapRenderMat[1].y = contMan.youareherescaleX;

	//rotate
	float rotXZ = -(atan2(yourdir[0], yourdir[2]) - atan2(0, 1));

	minimapRenderMat = glm::rotate(minimapRenderMat, rotXZ , vec3(0, 0, 1));

	////translate
	float xpos = (yourPos[0] - contMan.mapBotcord.x) / (contMan.mapTopcord.x - contMan.mapBotcord.x);
	float ypos = (yourPos[2] - contMan.mapBotcord.y) / (contMan.mapTopcord.y - contMan.mapBotcord.y);
	//to screenspace
	xpos = (xpos * 2) - 1;
	ypos = (ypos * 2) - 1;
	
	minimapRenderMat[0].w = xpos;
	minimapRenderMat[1].w = ypos;
	
	glProgramUniformMatrix4fv(uiShader, ui_World, 1, GL_FALSE, &minimapRenderMat[0][0]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);


	//set teammates texture
	asd.lastTextureSlot = GL_TEXTURE0;
	asd.state = TEXTURE_LOADED;
	asd.textureID = contMan.teamishereTexture;
	TextureManager::gTm->bind(asd, uiShader, ui_Texture);

	vec3* teamMateData = (vec3*)teammates;

	// render teammate indicators 
	for (int n = 0; n < nrOfTeammates; n++)
	{
		minimapRenderMat = mat4();
		//scale, rotate, translate

		//scale
		minimapRenderMat[0].x = contMan.youareherescaleX;
		minimapRenderMat[1].y = contMan.youareherescaleX;

		vec3 dirrr = teamMateData[n * 2 + 1];

		//rotate
		float rotXZ = -(atan2(dirrr[0], dirrr[2]) - atan2(0, 1));

		minimapRenderMat = glm::rotate(minimapRenderMat, rotXZ, vec3(0, 0, 1));

		vec3 poss = teamMateData[n * 2];

		////translate
		float xpos = (poss[0] - contMan.mapBotcord.x) / (contMan.mapTopcord.x - contMan.mapBotcord.x);
		float ypos = (poss[2] - contMan.mapBotcord.y) / (contMan.mapTopcord.y - contMan.mapBotcord.y);

		//to screenspace
		xpos = (xpos * 2) - 1;
		ypos = (ypos * 2) - 1;

		minimapRenderMat[0].w = xpos;
		minimapRenderMat[1].w = ypos;

		glProgramUniformMatrix4fv(uiShader, ui_World, 1, GL_FALSE, &minimapRenderMat[0][0]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}
}

void RenderPipeline::renderScoreBoard(int team1size, int team2size)
{
	glUseProgram(uiShader);
	//uniformlocation set texture 0  it defaults to 0 so not needed
	uiQuad.BindVertData();
	glActiveTexture(GL_TEXTURE0);

	// render headder ORANGE
	TextureInfo asd;
	asd.lastTextureSlot = GL_TEXTURE0;
	asd.state = TEXTURE_LOADED;
	asd.textureID = contMan.score_header_orange;
	TextureManager::gTm->bind(asd, uiShader, ui_Texture);
	minimapRenderMat = mat4();
	//pos
	minimapRenderMat[0].w = -0.35f;
	minimapRenderMat[1].w = 0.43;
	//scale
	minimapRenderMat[0].x = contMan.score_headerscale.x;
	minimapRenderMat[1].y = contMan.score_headerscale.y;

	glProgramUniformMatrix4fv(uiShader, ui_World, 1, GL_FALSE, &minimapRenderMat[0][0]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	// render headder GREEN
	asd.lastTextureSlot = GL_TEXTURE0;
	asd.state = TEXTURE_LOADED;
	asd.textureID = contMan.score_header_green;
	TextureManager::gTm->bind(asd, uiShader, ui_Texture);
	//pos
	minimapRenderMat[0].w = 0.65f;
	minimapRenderMat[1].w = 0.43f;

	glProgramUniformMatrix4fv(uiShader, ui_World, 1, GL_FALSE, &minimapRenderMat[0][0]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	//render Orange playerinfo boxes
	asd.lastTextureSlot = GL_TEXTURE0;
	asd.state = TEXTURE_LOADED;
	asd.textureID = contMan.score_team_orange;
	TextureManager::gTm->bind(asd, uiShader, ui_Texture);
	
	minimapRenderMat[0].x = contMan.score_teamscale.x;
	minimapRenderMat[1].y = contMan.score_teamscale.y;
	
	for (int n = 0; n < team1size; n++)
	{
		//pos
		minimapRenderMat[0].w = -0.5f;
		minimapRenderMat[1].w = 0.3f - 0.22 * n;
	
		glProgramUniformMatrix4fv(uiShader, ui_World, 1, GL_FALSE, &minimapRenderMat[0][0]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}
	
	//render Green playerinfo boxes
	asd.lastTextureSlot = GL_TEXTURE0;
	asd.state = TEXTURE_LOADED;
	asd.textureID = contMan.score_team_green;
	TextureManager::gTm->bind(asd, uiShader, ui_Texture);
	
	for (int n = 0; n < team2size; n++)
	{
		//pos
		minimapRenderMat[0].w = 0.5f;
		minimapRenderMat[1].w = 0.3f - 0.22 * n;
	
		glProgramUniformMatrix4fv(uiShader, ui_World, 1, GL_FALSE, &minimapRenderMat[0][0]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}

}
