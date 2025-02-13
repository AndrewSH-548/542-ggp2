#include <iostream>
#include <DirectXMath.h>

#include "Color.h"
#include "Ray.h"

using namespace std;
using namespace DirectX;

Color RayColor(const Ray& ray) {
	return Color(0, 0, 0);
}

int main() {
	// Image Size
	float aspectRatio = 16.0f / 9.0f;

	int imageWidth = 400;
	int imageHeight = imageWidth / aspectRatio;
	imageHeight = imageHeight < 1 ? 1 : imageHeight;

	// Camera Data
	float focalLength = 1.0f;
	float viewportHeight = 2.0f;
	float viewportWidth = viewportHeight * (float(imageWidth) / imageHeight);
	XMFLOAT3 cameraCenter = XMFLOAT3(0, 0, 0);

	// Calculate "UV" vectors of the viewport
	// Vectors across edges
	XMVECTOR viewportU = XMVectorSet(viewportWidth, 0.0f, 0.0f, 0.0f);
	XMVECTOR viewportV = XMVectorSet(0.0f, -viewportHeight, 0.0f, 0.0f);

	// Calculate deltas from pixel to pixel
	XMVECTOR pixelDeltaU = viewportU / imageWidth;
	XMVECTOR pixelDeltaV = viewportV / imageHeight;

	// calculate upper left pixel location
	XMVECTOR viewportUpperLeft = XMLoadFloat3(&cameraCenter) - XMVectorSet(0, 0, focalLength, 0) - viewportU / 2 - viewportV / 2;
	XMVECTOR pixel00Location = viewportUpperLeft + 0.5 * (pixelDeltaU / pixelDeltaV);

	// Render it
	cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

	for (int i = 0; i < imageHeight; i++) {
		clog << "\rScanlines remaining: " << (imageHeight - i) << " " << flush;
		for (int j = 0; j < imageWidth; j++) {
			XMVECTOR pixelCenter = pixel00Location + j * pixelDeltaU + i * pixelDeltaV;
			XMVECTOR rayDirection = pixelCenter - XMLoadFloat3(&cameraCenter);
			Ray ray(cameraCenter, rayDirection);

			Color pixelColor = RayColor(ray);
			WriteColor(cout, pixelColor);										
		}
	}
	clog << "\rDone.		\n";
}