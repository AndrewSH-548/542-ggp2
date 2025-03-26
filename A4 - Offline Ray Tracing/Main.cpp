#include "RTWeekend.h"
#include "Camera.h"
#include "Hittable.h"
#include "HittableList.h"
#include "Material.h"
#include "Sphere.h"

int main() {
	// World Data
	HittableList world;

	// Ground
	world.Add(make_shared<Sphere>(Point3(0, -100.5, -11), 100, make_shared<Lambertian>(Color(0.2, 0.2, 0.1))));
	
	// Big Mirror (Metal)
	world.Add(make_shared<Sphere>(Point3(-6, 1, 4), 5.5, make_shared<Metal>(Color(0.4, 0.6, 1), 0)));

	// Big Diamond Ball (Dielectric)
	world.Add(make_shared<Sphere>(Point3(6, 1, 4), 5.5, make_shared<Dielectric>(2.4)));

	// 4 Lambertian Spheres
	world.Add(make_shared<Sphere>(Point3(2.6, -0.2, -4), 1, make_shared<Lambertian>(Color(0.537, 0.92, 0.794))));
	world.Add(make_shared<Sphere>(Point3(0.8, -0.3, -4), 0.7, make_shared<Lambertian>(Color(0.47, 0.66, 0.98))));
	world.Add(make_shared<Sphere>(Point3(-0.8, -0.3, -4), 0.7, make_shared<Lambertian>(Color(0.945, 0.78, 0.85))));
	world.Add(make_shared<Sphere>(Point3(-2.6, 0, -4), 1, make_shared<Lambertian>(Color(0.96, 0.86, 0.43))));

	Camera camera;

	camera.aspectRatio = 16.0 / 9.0;
	camera.imageWidth = 1200;
	camera.samplesPerPixel = 500;
	camera.maxDepth = 50;

	camera.verticalFov = 60;
	camera.lookFrom = Point3(0, 3, -10);
	camera.lookAt = Point3(0, 0.6, 10);
	camera.up = Vector3(0, 9, 0);

	camera.defocusAngle = 0.6;
	camera.focusDistance = 10;

	camera.Render(world);
}