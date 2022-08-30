#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include <functional>

////////////////////////////////////////////////////////////////
// Macros

#define STD_BIND  (FUNCTION, OBJECT) std::bind(FUNCTION, OBJECT)
#define STD_BIND_1(FUNCTION, OBJECT) std::bind(FUNCTION, OBJECT, std::placeholders::_1)
#define STD_BIND_2(FUNCTION, OBJECT) std::bind(FUNCTION, OBJECT, std::placeholders::_1, std::placeholders::_2)
#define STD_BIND_3(FUNCTION, OBJECT) std::bind(FUNCTION, OBJECT, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)
#define STD_BIND_4(FUNCTION, OBJECT) std::bind(FUNCTION, OBJECT, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4)

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {
        
using Callback = std::function<void()>;

}   // namespace gugu
