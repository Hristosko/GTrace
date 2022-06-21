#include <math/Vector3f.h>

namespace gtrace
{
using Color3f = Vector3f;

namespace Colors
{
static const inline auto BLACK = Color3f(0.f);
static const inline auto WHITE = Color3f(1.f);
static const inline auto RED = Color3f(1.f, 0.f, 0.f);
static const inline auto GREEN = Color3f(0.f, 1.f, 0.f);
static const inline auto BLUE = Color3f(0.f, 0.f, 1.f);
static const inline auto YELLOW = Color3f(1.f, 1.f, 0.f);
static const inline auto CYAN = Color3f(0.f, 1.f, 1.f);
static const inline auto ORRANGE = Color3f(1.f, 0.65f, 0.f);
}  // namespace Colors

struct Color
{
    char r, g, b;
};

}  // namespace gtrace
