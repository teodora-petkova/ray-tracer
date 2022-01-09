#pragma once

#include <memory>

class Object;
using ObjectPtr = std::shared_ptr<Object>;
using ObjectConstPtr = std::shared_ptr<Object const>;

class Light;
using LightPtr = std::shared_ptr<Light>;

class Material;
using MaterialPtr = std::shared_ptr<Material>;

class Pattern;
using PatternPtr = std::shared_ptr<Pattern>;