#include "PinholeCamera.h"
#include "common/Errors.h"
#include "parser/ParamNames.h"

namespace gtrace
{
PinholeCamera::PinholeCamera(const ParsedParams& params)
{
    eye = params.getVector3f(CAMERA_EYE);
    lookPoint = params.getVector3f(CAMERA_LOOK_POINT);
    up = params.getVector3f(CAMERA_UP);
    viewPlaneDistance = params.getFloat(CAMERA_VIEW_PLANE_DISTANCE);
    zoom = params.getFloat(CAMERA_ZOOM);

    if (zoom < 0.f)
        Raise(InvalidValue("Pihnole camera zoom muss be positive; " + std::to_string(zoom)));

    if (viewPlaneDistance < 0.f)
        Raise(InvalidValue("Pihnole camera zoom muss be positive; " + std::to_string(zoom)));

    uvw = OrthonormalBasis(lookPoint - eye, up, OB_fromWV());
}

Ray PinholeCamera::castRay(float px, float py) const
{
    const Vector3f origin = eye;
    const Vector3f direction = normalize(zoom * px * uvw.u() + zoom * py * uvw.v() + viewPlaneDistance * uvw.w());
    return Ray(origin, direction);
}
}  // namespace gtrace