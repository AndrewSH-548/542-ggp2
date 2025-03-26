#ifndef CAMERA_H
#define CAMERA_H

#include "Hittable.h"
#include "Material.h"

using namespace std;

class Camera {
public:
    /* Public Camera Parameters Here */
    double aspectRatio = 1.0;
    int imageWidth = 100;
    int samplesPerPixel = 10;
    int maxDepth = 10;
    double verticalFov = 90;

    Point3 lookFrom = Point3(0, 0, 0);
    Point3 lookAt = Point3(0, 0, -1);
    Vector3 up = Vector3(0, 1, 0);

    double defocusAngle = 0;
    double focusDistance = 10;

    void Render(const Hittable& world) {
        Initialize();

        // Render it
        cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

        for (int i = 0; i < imageHeight; i++) {
            clog << "\rScanlines remaining: " << (imageHeight - i) << " " << flush;
            for (int j = 0; j < imageWidth; j++) {
                Color pixelColor(0, 0, 0);
                for (int sample = 0; sample < samplesPerPixel; sample++) {
                    Ray ray = GetRay(j, i);
                    pixelColor += RayColor(ray, maxDepth, world);
                }
                WriteColor(cout, pixelSampleScale * pixelColor);
            }
        }
        clog << "\nDone.		\n";
    }

private:
    /* Private Camera Variables Here */
    int imageHeight;
    double pixelSampleScale;
    Point3 center;
    Point3 pixel00Location;
    Vector3 pixelDeltaU;
    Vector3 pixelDeltaV;
    Vector3 u, v, w;
    Vector3 defocusDiskU;
    Vector3 defocusDiskV;

    void Initialize() {
        imageHeight = int(imageWidth / aspectRatio);
        imageHeight = imageHeight < 1 ? 1 : imageHeight;
        pixelSampleScale = 1.0 / samplesPerPixel;
        center = lookFrom;

        // Determine viewport dimensions
        double theta = DegreesToRadians(verticalFov);
        double h = tan(theta / 2);
        double viewportHeight = 2 * h * focusDistance;
        double viewportWidth = viewportHeight * imageWidth / imageHeight;

        w = UnitVector(lookFrom - lookAt);
        u = UnitVector(Cross(up, w));
        v = Cross(w, u);

        // Calculate vectors across edges
        Vector3 viewportU = viewportWidth * u;
        Vector3 viewportV = viewportHeight * -v;

        // Calculate deltas from pixel to pixel
        pixelDeltaU = viewportU / imageWidth;
        pixelDeltaV = viewportV / imageHeight;

        // calculate upper left pixel location
        Vector3 viewportUpperLeft = center - focusDistance * w - viewportU / 2 - viewportV / 2;
        pixel00Location = viewportUpperLeft + 0.5 * (pixelDeltaU + pixelDeltaV);

        // Calculate camera defocus disk vectors
        double defocusRadius = focusDistance * tan(DegreesToRadians(defocusAngle / 2));
        defocusDiskU = u * defocusRadius;
        defocusDiskV = v * defocusRadius;
    }

    Ray GetRay(int i, int j) const {
        // Construct a camera ray originating from the defocus disk and directed at randomly sampled
        // point around the pixel location i, j.

        Vector3 offset = SampleSquare();
        Point3 pixelSample = pixel00Location
            + ((i + offset.x()) * pixelDeltaU)
            + ((j + offset.y()) * pixelDeltaV);

        Point3 rayOrigin = defocusAngle <= 0 ? center : DefocusDiskSample();
        Vector3 rayDirection = pixelSample - rayOrigin;

        return Ray(rayOrigin, rayDirection);
    }

    Vector3 SampleSquare() const {
        // Returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit square.
        return Vector3(RandomDouble() - 0.5, RandomDouble() - 0.5, 0);
    }

    Point3 DefocusDiskSample() const{
        Point3 point = RandomInUnitDisk();
        return center + point[0] * defocusDiskU + point[1] * defocusDiskV;
    }

    Color RayColor(const Ray& ray, int depth, const Hittable& world) const {
        //Stop getting light if we exceed the bounce limit
        if (depth <= 0) return Color(0, 0, 0);

        HitRecord record;

        if (world.Hit(ray, Interval(0.001, infinity), record)) {
            Ray scattered;
            Color attenuation;
            if (record.material->Scatter(ray, record, attenuation, scattered))
                return attenuation * RayColor(scattered, depth - 1, world);
            return Color(0, 0, 0);
        }

        Vector3 unitDirection = UnitVector(ray.Direction());
        double a = 0.5 * (unitDirection.y() + 1);
        Color skyGradientTop = Color(1, 0.7, 0.5);
        Color skyGradientBottom = Color(1, 1, 0.5);
        return (1 - a) * skyGradientBottom + a * skyGradientTop;
    }
};

#endif