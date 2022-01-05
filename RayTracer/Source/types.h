#pragma once

#include <memory>

class Object;
using ObjectPtr = std::shared_ptr<Object>;
using ObjectConstPtr = std::shared_ptr<Object const>;