//
// Created by ruppet on 3/14/2020.
//

#include "../../cheat.h"

#define _s base_state

void c_scripting::globals() {
    auto __x = std::string(str("x"));
    auto __y = std::string(str("y"));
    auto __z = std::string(str("z"));

    _s[str("print")] = [](sol::this_state t, sol::object o) {
        auto s = sol::state_view(t);
        std::string p = s[str("tostring")](o);

        utils->log(str("%s"), p.c_str());
    };



    _s[str("add_callback")] = [](const std::string &name, sol::function fn) {
        if (!scripting->current_script)
            return;

        scripting->current_script->callbacks[util::fnv1a(name.c_str())].emplace_back(fn);
    };


    _s[str("color")] = [](int r, int g, int b, std::optional<int> a) {
        return (uint32)ImColor(r, g, b, a.value_or(255));
    };

    _s[str("vector")] = sol::overload(
        [](float x, float y, float z) {
            return c_vector(x, y, z);
        }, [](float x, float y) {
            return c_vector(x, y);
        }, []() {
            return c_vector();
        }
    );

    _s.new_usertype<c_vector>(str("vector_t"),
        sol::constructors<c_vector(), c_vector(float, float), c_vector(float, float, float)>(),
        sol::meta_function::addition, [](c_vector &a, c_vector &b) { return a + b; },
        sol::meta_function::subtraction, [](c_vector &a, c_vector &b) { return a - b; },
        sol::meta_function::division, [](c_vector &a, c_vector &b) { return c_vector(a.x / b.x, a.y / b.y, a.z / b.z); },
        sol::meta_function::multiplication, [](c_vector &a, c_vector &b) { return c_vector(a.x * b.x, a.y * b.y, a.z * b.z); },
        sol::meta_function::equal_to, [](c_vector &a, c_vector &b) { return a == b; },
        sol::meta_function::less_than, [](c_vector &a, c_vector &b) { return a.x < b.x || a.y < b.y || a.z < b.z; },
        sol::meta_function::less_than_or_equal_to, [](c_vector &a, c_vector &b) { return a.x <= b.x || a.y <= b.y || a.z <= b.z; },
        sol::meta_function::to_string, [](sol::table self) {
            return utils->format(str("[%.3f, %.3f, %.3f]"),
                    self[str("x")].get<float>(), self[str("y")].get<float>(), self[str("z")].get<float>());
        },
        __x, &c_vector::x,
        __y, &c_vector::y,
        __z, &c_vector::z
    );
}

#undef _s