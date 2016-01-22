#ifndef PHYSICS_H
#define PHYSICS_H

#ifdef PHYSICS_EXPORTS
#define PHYSICS_API __declspec( dllexport)
#else
#define PHYSICS_API __declspec( dllimport  )
#endif

#define GRAVITY 18.0f


#include "Collision\Geometry.h"
#include "Collision\CollideMesh.h"
#include "Effect\EffectMesh.h"
#include<vector>
#include<iterator>


//----COLLISION INFO----
/*
The first parameter for each collision check is ALWAYS The player or the bullet
The object we check against will ALWAYS be second parameter

It is ALWAYS the FIRST object COLLIDING with the SECOND
This means:
- We check the FIRST object towards a bunch of SECOND ones
- When we want the normal of the collision, it's the normal against the side of the SECOND OBJECT
	This is because we want to know in which direction to send the FIRST object
*/

class Physics
{
private:
	//General physics components will go here, and things will be added as we go
	std::vector<std::vector<CollideMesh>> worldBoxes; //each CollideMesh is an ABB, and the vector is all CollideMeshes in the chunk
	std::vector<EffectMesh> effectBoxes;
	CollideMesh playerBox;
	CollideMesh bulletBox;

	//--------AABB Collisions--------//
	bool checkAABBvAABBCollision(AABB* mesh1, AABB* mesh2);
	glm::vec3 checkAABBvAABBCollision(Geometry* obj1, Geometry* obj2); //unnecessary redefinition
	
	glm::vec3 checkAABBvAngledCylinderCollision(CollideMesh mesh1, CollideMesh mesh2);
	glm::vec3 checkAABBvCylinderCollision(CollideMesh mesh1, CollideMesh mesh2);
	
	glm::vec3 checkAABBvSphereCollision(CollideMesh mesh1, CollideMesh mesh2);
	//--------------//--------------//

	//--------OBB Collisions--------//
	glm::vec3 checkOBBvOBBCollision(CollideMesh mesh1, CollideMesh mesh2); //Probably not needed
	glm::vec3 checkOBBvOBBCollision(Geometry* obj1, Geometry* obj2); //Most likely not needed
	
	glm::vec3 checkOBBvCylinderCollision(CollideMesh mesh1, CollideMesh mesh2);
	glm::vec3 checkOBBvAngledCylinderCollision(CollideMesh mesh1, CollideMesh mesh2);
	
	
	//--------------//--------------//

	//-----Cylinder Collisions------//
	glm::vec3 checkCylindervCylinderCollision(CollideMesh mesh1, CollideMesh mesh2);
	glm::vec3 checkCylindervSphereCollision(CollideMesh mesh1, CollideMesh mesh2);
	glm::vec3 checkAngledCylindervSphereCollision(CollideMesh mesh1, CollideMesh mesh2);
	//--------------//--------------//

	//-------Sphere Collision-------//
	glm::vec3 checkSpherevSphereCollision(CollideMesh mesh1, CollideMesh mesh2);

	glm::vec3 checkSpherevOBBCollision(CollideMesh mesh1, CollideMesh mesh2);
	//--------------//--------------//

	//--------Line Collision--------//
	float checkLinevPlaneCollision(glm::vec3 l1, glm::vec3 l2, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);


	//------Normal Calculators------//
	std::vector<glm::vec3> getCollisionNormal(AABB* aabb1, AABB* aabb2);
	glm::vec3 getCollisionNormal(Cylinder cylinder, AABB aabb);
	glm::vec3 getCollisionNormal(AngledCylinder cylinder, AABB aabb);
	glm::vec3 getCollisionNormal(Sphere sphere, AABB aabb);
	glm::vec3 getCollisionNormal(OBB obb1, OBB obb2);
	glm::vec3 getCollisionNormal(Cylinder cylinder, OBB obb);
	glm::vec3 getCollisionNormal(AngledCylinder cylinder, OBB obb);
	glm::vec3 getCollisionNormal(Sphere sphere, OBB obb);

	glm::vec4 getSpherevOBBNorms(glm::vec3 pos, float rad, OBB* obb);
	//--------------//--------------//

	void storeChunkBox(int chunkID, std::vector<AABB> cBox);
	void storeEffectBox(CollideMesh efml);

	void initBulletBox();
public:
	Physics();
	~Physics();

	virtual bool init();
	virtual bool release();

	virtual glm::vec3 normalize(glm::vec3 vec3);

	virtual bool removeEffect(int effID) { return false; }; //SKRIV EN FUNKTIONSFAN
	
	virtual glm::vec3 checkPlayerVPlayerCollision(glm::vec3 playerPos1, glm::vec3 playerPos2);
	virtual glm::vec3 checkPlayerVBulletCollision(glm::vec3 playerPos, glm::vec3 bulletPos);
	virtual std::vector<glm::vec4> sphereVWorldCollision(glm::vec3 playerPos, float rad);
	virtual glm::vec3 checkBulletVWorldCollision(glm::vec3 bulletPos);
	virtual glm::vec3 checkPlayerVEffectCollision(glm::vec3 playerPos, float rad, unsigned int eType);
	virtual glm::vec3 checkBulletVEffectCollision(glm::vec3 bulletPos);

	virtual float addGravity(float dt);

	virtual void receiveChunkBoxes(int chunkID, void* cBoxes);
	virtual void receiveWorldBoxes(std::vector<std::vector<float>> wBoxes);
	virtual void receivePlayerBox(std::vector<float> pBox);
	virtual void receiveEffectBox(std::vector<float> eBox, unsigned int etype, int pID, int eID);
};

extern "C" PHYSICS_API Physics* CreatePhysics();

#endif