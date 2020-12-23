#include "scene.h"

#include <cmath>
#include <algorithm>
#include <limits>
#include <array>

#include "material.h"
#include "ray_trace.h"

constexpr auto centre = glm::vec3{0.9, -1.925, -6.69};
constexpr auto radius = 0.825f;

constexpr auto blue_pyramid = std::array{
    glm::vec3(-0.4, -2.75, -9.55),
    glm::vec3(-0.93, 0.55, -8.51),
    glm::vec3(0.11, -2.75, -7.98),

    glm::vec3(0.11, -2.75, -7.98),
    glm::vec3(-0.93, 0.55, -8.51),
    glm::vec3(-1.46, -2.75, -7.47),

    glm::vec3(-1.46, -2.75, -7.47),
    glm::vec3(-0.93, 0.55, -8.51),
    glm::vec3(-1.97, -2.75, -9.04),

    glm::vec3(-1.97, -2.75, -9.04),
    glm::vec3(-0.93, 0.55, -8.51),
    glm::vec3(-0.4, -2.75, -9.55)
};

constexpr auto floor1 = std::array{
  glm::vec3(2.75, -2.75, -5),
  glm::vec3(2.75, -2.75, -10.5),
  glm::vec3(-2.75, -2.75, -10.5),
  glm::vec3(-2.75, -2.75, -5),
  glm::vec3(2.75, -2.75, -5),
  glm::vec3(-2.75, -2.75, -10.5)
};

//Ceiling
constexpr auto ceiling = std::array{
  glm::vec3(2.75, 2.75, -10.5),
  glm::vec3(2.75, 2.75, -5),
  glm::vec3(-2.75, 2.75, -5),

  glm::vec3(-2.75, 2.75, -10.5),
  glm::vec3(2.75, 2.75, -10.5),
  glm::vec3(-2.75, 2.75, -5)
};

//Green wall on right
constexpr auto right_wall = std::array{
  glm::vec3(2.75, 2.75, -5),
  glm::vec3(2.75, 2.75, -10.5),
  glm::vec3(2.75, -2.75, -10.5),

  glm::vec3(2.75, -2.75, -5),
  glm::vec3(2.75, 2.75, -5),
  glm::vec3(2.75, -2.75, -10.5)
};

//Red wall on left
constexpr auto left_wall = std::array{
  glm::vec3(-2.75, -2.75, -5),
  glm::vec3(-2.75, -2.75, -10.5),
  glm::vec3(-2.75, 2.75, -10.5),

  glm::vec3(-2.75, 2.75, -5),
  glm::vec3(-2.75, -2.75, -5),
  glm::vec3(-2.75, 2.75, -10.5)
};

////Back wall
//glm::vec3 back_wall[2] {
//  glm::vec3(0, 0, 1),	//normal
//  glm::vec3(0, 0, -10.5)	//point
//};

//Back wall
constexpr auto back_wall = std::array{
	glm::vec3(2.75, -2.75, -10.5),
	glm::vec3(2.75, 2.75, -10.5),
	glm::vec3(-2.75, 2.75, -10.5),

	glm::vec3(-2.75, 2.75, -10.5),
	glm::vec3(-2.75, -2.75, -10.5),
	glm::vec3(2.75, -2.75, -10.5)
};

//---------------------------------------------------------------------------
// Shiny red icosahedron
constexpr auto icosahedron = std::array{
	glm::vec3(-2, -1, -7),
	glm::vec3(-1.276, -0.4472, -6.474),
	glm::vec3(-2.276, -0.4472, -6.149),

	glm::vec3(-1.276, -0.4472, -6.474),
	glm::vec3(-2 ,-1, -7),
	glm::vec3(-1.276 ,-0.4472 ,-7.526),

	glm::vec3(-2, -1 ,-7),
	glm::vec3(-2.276 ,-0.4472 ,-6.149),
	glm::vec3(-2.894 ,-0.4472, -7),

	glm::vec3(-2 ,-1 ,-7),
	glm::vec3(-2.894 ,-0.4472 ,-7),
	glm::vec3(-2.276 ,-0.4472, -7.851),

	glm::vec3(-2 ,-1 ,-7),
	glm::vec3(-2.276 ,-0.4472 ,-7.851),
	glm::vec3(-1.276 ,-0.4472 ,-7.526),

	glm::vec3(-1.276 ,-0.4472, -6.474),
	glm::vec3(-1.276 ,-0.4472 ,-7.526),
	glm::vec3(-1.106, 0.4472, -7),

	glm::vec3(-2.276, -0.4472 ,-6.149),
	glm::vec3(-1.276, -0.4472 ,-6.474),
	glm::vec3(-1.724 ,0.4472, -6.149),

	glm::vec3(-2.894 ,-0.4472, -7),
	glm::vec3(-2.276, -0.4472, -6.149),
	glm::vec3(-2.724 ,0.4472 ,-6.474),

	glm::vec3(-2.276, -0.4472, -7.851),
	glm::vec3(-2.894 ,-0.4472 ,-7),
	glm::vec3( -2.724 ,0.4472 ,-7.526),

	glm::vec3(-1.276, -0.4472, -7.526),
	glm::vec3(-2.276, -0.4472 ,-7.851),
	glm::vec3(-1.724, 0.4472, -7.851),

	glm::vec3(-1.276 ,-0.4472 ,-6.474),
	glm::vec3(-1.106, 0.4472, -7),
	glm::vec3(-1.724 ,0.4472, -6.149),

	glm::vec3( -2.276 ,-0.4472 ,-6.149),
	glm::vec3(-1.724, 0.4472, -6.149),
	glm::vec3(-2.724 ,0.4472, -6.474),

	glm::vec3( -2.894, -0.4472, -7),
	glm::vec3( -2.724, 0.4472, -6.474),
	glm::vec3( -2.724, 0.4472, -7.526),

	glm::vec3(-2.276 ,-0.4472 ,-7.851),
	glm::vec3( -2.724 ,0.4472 ,-7.526),
	glm::vec3( -1.724 ,0.4472 ,-7.851),

	glm::vec3(-1.276 ,-0.4472 ,-7.526),

	glm::vec3( -1.724 ,0.4472, -7.851),
	glm::vec3( -1.106, 0.4472, -7),

	glm::vec3(-1.724, 0.4472, -6.149),
	glm::vec3(-1.106, 0.4472 ,-7),
	glm::vec3( -2, 1, -7),

	glm::vec3( -2.724 ,0.4472 ,-6.474),
	glm::vec3(-1.724, 0.4472 ,-6.149),
	glm::vec3(-2, 1, -7),

	glm::vec3(-2.724, 0.4472, -7.526),
	glm::vec3( -2.724, 0.4472 ,-6.474),
	glm::vec3(-2, 1 ,-7),

	glm::vec3(-1.724 ,0.4472 ,-7.851),
	glm::vec3(-2.724, 0.4472 ,-7.526),
	glm::vec3(-2 ,1 ,-7),

	glm::vec3(-1.106 ,0.4472 ,-7),
	glm::vec3(-1.724 ,0.4472 ,-7.851),
	glm::vec3(-2, 1, -7)
};

// Green cone
constexpr auto green_Cone = std::array{
	glm::vec3(0, -1, -5.8),
	glm::vec3(0 ,0.6, -5),
	glm::vec3(0.4, -1 ,-5.693),

	glm::vec3(0.4 ,-1, -5.693),
	glm::vec3(0, 0.6, -5),
	glm::vec3(0.6928 ,-1 ,-5.4),

	glm::vec3(0.6928 ,-1 ,-5.4),
	glm::vec3(0 ,0.6 ,-5),
	glm::vec3(0.8 ,-1 ,-5),

	glm::vec3(0.8 ,-1 ,-5),
	glm::vec3(0,0.6 ,-5),
	glm::vec3(0.6928, -1 ,-4.6),

	glm::vec3( 0.6928, -1 ,-4.6),
	glm::vec3(0 ,0.6, -5),
	glm::vec3(0.4, -1 ,-4.307),

	glm::vec3(0.4 ,-1 ,-4.307),
	glm::vec3(0 ,0.6, -5),
	glm::vec3(0 ,-1 ,-4.2),

	glm::vec3(0 ,-1 ,-4.2),
	glm::vec3(0, 0.6, -5),
	glm::vec3(-0.4, -1 ,-4.307),

	glm::vec3(-0.4 ,-1, -4.307),
	glm::vec3(0 ,0.6 ,-5),
	glm::vec3(-0.6928 ,-1 ,-4.6),

	glm::vec3(-0.6928, -1 ,-4.6),
	glm::vec3(0, 0.6 ,-5),
	glm::vec3(-0.8 ,-1 ,-5),

	glm::vec3(-0.8 ,-1 ,-5),
	glm::vec3(0 ,0.6 ,-5),
	glm::vec3(-0.6928 ,-1, -5.4),

	glm::vec3(-0.6928 ,-1 ,-5.4),
	glm::vec3(0, 0.6, -5),
	glm::vec3(-0.4, -1, -5.693),

	glm::vec3(-0.4 ,-1, -5.693),
	glm::vec3(0, 0.6 ,-5),
	glm::vec3(0 ,-1 ,-5.8)
};

//Floor
constexpr auto floor2 = std::array{
		glm::vec3(-10,-1,-2),
		glm::vec3(10,-1,-2),
		glm::vec3(10,-1,-12),

		glm::vec3(10,-1,-12),
		glm::vec3(-10,-1,-12),
		glm::vec3(-10,-1,-2)
};

//Back wall
constexpr auto back_wall2 = std::array{
		glm::vec3(-10, -1, -12),
		glm::vec3(10, -1, -12),
		glm::vec3(10, 9, -12),

		glm::vec3(10,9,-12),
		glm::vec3(-10, 9,-12),
		glm::vec3(-10, -1, -12)
};

/*------------------------------------------------------------------------
Initializing functions
--------------------------------------------------------------------------*/

auto initScene1() -> Scene
{
	//Scene 1
	Scene scene1;

	//Reflective grey sphere
	std::shared_ptr<Sphere> sphere1 = std::make_shared<Sphere>(glm::vec3( 0.9f, -1.925f, -6.69f), 0.825f, 1);
	sphere1->material.diffuse = glm::vec3(0.6f, 0.6f, 0.6f);
	sphere1->material.specular = 1.0f*sphere1->material.diffuse;
	sphere1->material.reflectionStrength = glm::vec3(0.4f);
	sphere1->material.specularCoefficient = 64;
	scene1.shapesInScene.push_back(sphere1);

	//pyramid
	std::shared_ptr<Triangles> pyramid = std::make_shared<Triangles>(blue_pyramid, 2);
	pyramid->material.diffuse = glm::vec3(0.0f, 0.85f, 0.95f);
	pyramid->material.specular = 0.1f*pyramid->material.diffuse;
	pyramid->material.reflectionStrength = glm::vec3(0.3f);
	scene1.shapesInScene.push_back(pyramid);

	std::shared_ptr<Triangles> rightWall = std::make_shared<Triangles>(right_wall, 3);
	rightWall->material.diffuse = glm::vec3(0.0f, 0.7f, 0.0f);
	rightWall->material.ambient = 0.1f * rightWall->material.diffuse;
	//rightWall->material.specular = 0.1f*rightWall->material.diffuse;
	scene1.shapesInScene.push_back(rightWall);

	std::shared_ptr<Triangles> leftWall = std::make_shared<Triangles>(left_wall, 4);
	leftWall->material.diffuse = glm::vec3(0.7f, 0.0f, 0.0f);
	leftWall->material.ambient = 0.1f*leftWall->material.diffuse;
	//leftWall->material.specular = 0.1f*leftWall->material.diffuse;
	scene1.shapesInScene.push_back(leftWall);

	std::shared_ptr<Triangles> floorWall = std::make_shared<Triangles>(floor1, 5);
	floorWall->material.diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
	floorWall->material.ambient = 0.1f*floorWall->material.diffuse;
	//floorWall->material.specular = 0.1f*floorWall->material.diffuse;
	scene1.shapesInScene.push_back(floorWall);

	std::shared_ptr<Triangles> ceilingWall = std::make_shared<Triangles>(ceiling, 6);
	ceilingWall->material.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	ceilingWall->material.ambient = 0.1f*ceilingWall->material.diffuse;
	scene1.shapesInScene.push_back(ceilingWall);

	std::shared_ptr<Triangles> backWall = std::make_shared<Triangles>(back_wall, 7);
	backWall->material.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	scene1.shapesInScene.push_back(backWall);

	scene1.lightPos = glm::vec3(0.0f, 2.5f, -7.75f);

	scene1.lightCol = glm::vec3(1.0f, 1.0f, 1.0f);
	scene1.ambiantFactor = 0.1f;
	return scene1;
}

auto initScene2() -> Scene
{
	//Scene 2
	Scene scene2;
	std::shared_ptr<Triangles> Icosahedron = std::make_shared<Triangles>(icosahedron, 1);
	Icosahedron->material.diffuse = glm::vec3(1.0f, 0.0f, 0.0f);
	Icosahedron->material.reflectionStrength = glm::vec3(0.5f);
	scene2.shapesInScene.push_back(Icosahedron);

	// Large yellow sphere
	std::shared_ptr<Sphere> sphere2 = std::make_shared<Sphere>(glm::vec3(1.0f, -0.5f, -3.5f), 0.5f, 2);
	sphere2->material.diffuse = glm::vec3(1.0f, 1.0f, 0.0f);
	sphere2->material.specular = sphere2->material.diffuse;
	sphere2->material.specularCoefficient = 64;
	scene2.shapesInScene.push_back(sphere2);

	// Reflective grey sphere
	std::shared_ptr<Sphere> sphere3 = std::make_shared<Sphere>(glm::vec3(0.0f, 1.0f, -5.0f), 0.4f, 3);
	sphere3->material.diffuse = glm::vec3(0.6f, 0.6f, 0.6f);
	sphere3->material.specular = sphere3->material.diffuse;
	sphere3->material.reflectionStrength = glm::vec3(0.5f, 0.5f, 0.5f);
	sphere3->material.specularCoefficient = 64;
	scene2.shapesInScene.push_back(sphere3);


	// Metallic purple sphere
	std::shared_ptr<Sphere> sphere4 = std::make_shared<Sphere>(glm::vec3(-0.8f, -0.75f, -4.0f), 0.25f, 4);
	sphere4->material.diffuse = glm::vec3(0.4f, 0.1f, 1.0f);
	sphere4->material.specular = sphere4->material.diffuse;
	sphere4->material.reflectionStrength = glm::vec3(0.3f);
	sphere4->material.specularCoefficient = 64;
	scene2.shapesInScene.push_back(sphere4);

	//Green cone
	std::shared_ptr<Triangles> greenCone = std::make_shared<Triangles>(green_Cone, 5);
	greenCone->material.diffuse = glm::vec3(0.0f, 0.8f, 0.0f);
	greenCone->material.specular = greenCone->material.diffuse;
	greenCone->material.specularCoefficient = 8;
	scene2.shapesInScene.push_back(greenCone);

	//Floor
	std::shared_ptr<Plane> floorTwo = std::make_shared<Plane>(glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 6);
	floorTwo->material.diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
	floorTwo->material.ambient = 0.5f * floorTwo->material.diffuse;
	scene2.shapesInScene.push_back(floorTwo);

	//Back wall
	std::shared_ptr<Plane> backWallTwo = std::make_shared<Plane>(glm::vec3(0.0f, 0.0f, -12.0f), glm::vec3(0.0f, 0.0f, 1.0f), 7);
	backWallTwo->material.diffuse = glm::vec3(0.0f, 0.6f, 0.6f);
	backWallTwo->material.ambient = 0.5f * backWallTwo->material.diffuse;
	backWallTwo->material.specular = backWallTwo->material.diffuse;
	backWallTwo->material.specularCoefficient = 8;
	scene2.shapesInScene.push_back(backWallTwo);

	scene2.lightPos = glm::vec3(4.0f, 6.0f, -1.0f);
	scene2.lightCol = glm::vec3(1.0f, 1.0f, 1.0f);
	scene2.ambiantFactor = 0.1f;

	return scene2;
}