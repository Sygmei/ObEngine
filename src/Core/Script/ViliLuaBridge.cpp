#include <Script/ViliLuaBridge.hpp>
#include <Utils/StringUtils.hpp>
#include <Utils/VectorUtils.hpp>
#include <sol/sol.hpp>

namespace obe::Script::ViliLuaBridge
{
    sol::lua_value viliToLua(vili::node& convert)
    {
        if (convert.is<vili::array>())
        {
            return viliArrayToLuaTable(convert);
        }
        else if (convert.is<vili::object>())
        {
            return viliObjectToLuaTable(convert);
        }
        else if (convert.is_primitive())
        {
            return viliPrimitiveToLuaValue(convert);
        }
    }

    vili::node luaToVili(sol::object convert)
    {
        if (convert.is<sol::table>())
        {
            vili::integer expect = 1;
            for (auto& [k, _] : convert.as<sol::table>())
            {
                if (!k.is<vili::integer>() || k.as<vili::integer>() != expect++)
                {
                    return luaTableToViliObject(convert);
                }
            }
            return luaTableToViliArray(convert);
        }
        else
        {
            return luaValueToViliPrimitive(convert);
        }
    }

    sol::lua_value viliObjectToLuaTable(vili::node& convert)
    {
        std::unordered_map<std::string, sol::lua_value> result;
        for (auto [key, value] : convert.items())
        {
            if (value.is_primitive())
            {
                result.emplace(key, viliPrimitiveToLuaValue(value));
            }
            else if (value.is<vili::object>())
            {
                result.emplace(key, viliObjectToLuaTable(value));
            }
            else if (value.is<vili::array>())
            {
                result.emplace(key, viliArrayToLuaTable(value));
            }
        }
        return sol::as_table(result);
    }

    sol::lua_value viliPrimitiveToLuaValue(vili::node& convert)
    {
        if (convert.is<vili::integer>())
            return convert.as<vili::integer>();
        else if (convert.is<vili::string>())
            return convert.as<vili::string>();
        else if (convert.is<vili::boolean>())
            return convert.as<vili::boolean>();
        else if (convert.is<vili::number>())
            return convert.as<vili::number>();
    }

    sol::lua_value viliArrayToLuaTable(vili::node& convert)
    {
        std::vector<sol::lua_value> result;
        std::size_t index = 0;
        for (vili::node& value : convert)
        {
            if (value.is_primitive())
                result.push_back(viliPrimitiveToLuaValue(value));
            else if (value.is<vili::array>())
                result.push_back(viliArrayToLuaTable(value));
            else if (value.is<vili::object>())
                result.push_back(viliObjectToLuaTable(value));
        }
        return sol::as_table(result);
    }

    vili::node luaTableToViliObject(sol::table convert)
    {
        vili::node result = vili::object {};

        for (auto& [key, value] : convert)
        {
            result.insert(key.as<std::string>(), luaToVili(value));
        }
        return result;
    }

    vili::node luaValueToViliPrimitive(sol::lua_value convert)
    {
        vili::node result;
        if (convert.is<vili::integer>())
        {
            result = convert.as<vili::integer>();
        }
        else if (convert.is<vili::number>())
        {
            result = convert.as<vili::number>();
        }
        else if (convert.is<vili::boolean>())
        {
            result = convert.as<vili::boolean>();
        }
        else if (convert.is<vili::string>())
        {
            result = convert.as<vili::string>();
        }
        return result;
    }

    vili::node luaTableToViliArray(sol::table convert)
    {
        vili::node result = vili::array {};
        for (auto& [_, value] : convert)
        {
            result.push(luaToVili(value));
        }
        return result;
    }
} // namespace obe::Script::DataBridge
