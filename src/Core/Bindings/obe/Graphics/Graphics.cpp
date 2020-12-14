#include <Bindings/obe/Graphics/Graphics.hpp>

#include <Graphics/Color.hpp>
#include <Graphics/Font.hpp>
#include <Graphics/PositionTransformers.hpp>
#include <Graphics/RenderTarget.hpp>
#include <Graphics/Shader.hpp>
#include <Graphics/Sprite.hpp>
#include <Graphics/Text.hpp>
#include <Graphics/Texture.hpp>

#include <Bindings/Config.hpp>

namespace obe::Graphics::Bindings
{
    void LoadEnumColorType(sol::state_view state)
    {
        sol::table GraphicsNamespace = state["obe"]["Graphics"].get<sol::table>();
        GraphicsNamespace.new_enum<obe::Graphics::ColorType>("ColorType",
            { { "Rgba", obe::Graphics::ColorType::Rgba },
                { "Hsv", obe::Graphics::ColorType::Hsv },
                { "Hex", obe::Graphics::ColorType::Hex },
                { "ColorName", obe::Graphics::ColorType::ColorName } });
    }
    void LoadEnumSpriteHandlePointType(sol::state_view state)
    {
        sol::table GraphicsNamespace = state["obe"]["Graphics"].get<sol::table>();
        GraphicsNamespace.new_enum<obe::Graphics::SpriteHandlePointType>(
            "SpriteHandlePointType",
            { { "ScaleHandle", obe::Graphics::SpriteHandlePointType::ScaleHandle },
                { "RotateHandle", obe::Graphics::SpriteHandlePointType::RotateHandle } });
    }
    void LoadClassColor(sol::state_view state)
    {
        sol::table GraphicsNamespace = state["obe"]["Graphics"].get<sol::table>();
        sol::usertype<obe::Graphics::Color> bindColor
            = GraphicsNamespace.new_usertype<obe::Graphics::Color>("Color",
                sol::call_constructor,
                sol::constructors<obe::Graphics::Color(),
                    obe::Graphics::Color(double, double, double),
                    obe::Graphics::Color(double, double, double, double),
                    obe::Graphics::Color(const std::string&),
                    obe::Graphics::Color(const obe::Graphics::Color&),
                    obe::Graphics::Color(const sf::Color&)>(),
                sol::base_classes, sol::bases<obe::Types::Serializable>());
        bindColor["dump"] = sol::overload(
            static_cast<vili::node (obe::Graphics::Color::*)(obe::Graphics::ColorType)>(
                &obe::Graphics::Color::dump),
            static_cast<vili::node (obe::Graphics::Color::*)() const>(
                &obe::Graphics::Color::dump));
        bindColor["load"] = &obe::Graphics::Color::load;
        bindColor["fromString"] = &obe::Graphics::Color::fromString;
        bindColor["fromName"] = sol::overload(
            [](obe::Graphics::Color* self, std::string name) -> bool {
                return self->fromName(name);
            },
            [](obe::Graphics::Color* self, std::string name, bool strict) -> bool {
                return self->fromName(name, strict);
            });
        bindColor["fromHex"] = &obe::Graphics::Color::fromHex;
        bindColor["fromRgb"]
            = sol::overload([](obe::Graphics::Color* self, double r, double g,
                                double b) -> void { return self->fromRgb(r, g, b); },
                [](obe::Graphics::Color* self, double r, double g, double b,
                    double a) -> void { return self->fromRgb(r, g, b, a); });
        bindColor["fromHsv"] = &obe::Graphics::Color::fromHsv;
        bindColor["toInteger"] = &obe::Graphics::Color::toInteger;
        bindColor[sol::meta_function::equal_to] = &obe::Graphics::Color::operator==;
        bindColor[sol::meta_function::addition] = &obe::Graphics::Color::operator+;
        bindColor[sol::meta_function::subtraction] = sol::overload(
            static_cast<obe::Graphics::Color (obe::Graphics::Color::*)(
                const obe::Graphics::Color&) const>(&obe::Graphics::Color::operator-),
            static_cast<obe::Graphics::Color (obe::Graphics::Color::*)() const>(
                &obe::Graphics::Color::operator-));
        bindColor[sol::meta_function::multiplication] = sol::overload(
            static_cast<obe::Graphics::Color (obe::Graphics::Color::*)(
                const obe::Graphics::Color&) const>(&obe::Graphics::Color::operator*),
            static_cast<obe::Graphics::Color (obe::Graphics::Color::*)(double) const>(
                &obe::Graphics::Color::operator*));
        bindColor[sol::meta_function::division] = sol::overload(
            static_cast<obe::Graphics::Color (obe::Graphics::Color::*)(
                const obe::Graphics::Color&) const>(&obe::Graphics::Color::operator/),
            static_cast<obe::Graphics::Color (obe::Graphics::Color::*)(double) const>(
                &obe::Graphics::Color::operator/));
        bindColor["operator sf::Color"] = &obe::Graphics::Color::operator sf::Color;
        bindColor["r"] = &obe::Graphics::Color::r;
        bindColor["g"] = &obe::Graphics::Color::g;
        bindColor["b"] = &obe::Graphics::Color::b;
        bindColor["a"] = &obe::Graphics::Color::a;
        bindColor["AliceBlue"] = sol::var(&obe::Graphics::Color::AliceBlue);
        bindColor["AntiqueWhite"] = sol::var(&obe::Graphics::Color::AntiqueWhite);
        bindColor["Aqua"] = sol::var(&obe::Graphics::Color::Aqua);
        bindColor["Aquamarine"] = sol::var(&obe::Graphics::Color::Aquamarine);
        bindColor["Azure"] = sol::var(&obe::Graphics::Color::Azure);
        bindColor["Beige"] = sol::var(&obe::Graphics::Color::Beige);
        bindColor["Bisque"] = sol::var(&obe::Graphics::Color::Bisque);
        bindColor["Black"] = sol::var(&obe::Graphics::Color::Black);
        bindColor["BlanchedAlmond"] = sol::var(&obe::Graphics::Color::BlanchedAlmond);
        bindColor["Blue"] = sol::var(&obe::Graphics::Color::Blue);
        bindColor["BlueViolet"] = sol::var(&obe::Graphics::Color::BlueViolet);
        bindColor["Brown"] = sol::var(&obe::Graphics::Color::Brown);
        bindColor["BurlyWood"] = sol::var(&obe::Graphics::Color::BurlyWood);
        bindColor["CadetBlue"] = sol::var(&obe::Graphics::Color::CadetBlue);
        bindColor["Chartreuse"] = sol::var(&obe::Graphics::Color::Chartreuse);
        bindColor["Chocolate"] = sol::var(&obe::Graphics::Color::Chocolate);
        bindColor["Coral"] = sol::var(&obe::Graphics::Color::Coral);
        bindColor["CornflowerBlue"] = sol::var(&obe::Graphics::Color::CornflowerBlue);
        bindColor["Cornsilk"] = sol::var(&obe::Graphics::Color::Cornsilk);
        bindColor["Crimson"] = sol::var(&obe::Graphics::Color::Crimson);
        bindColor["Cyan"] = sol::var(&obe::Graphics::Color::Cyan);
        bindColor["DarkBlue"] = sol::var(&obe::Graphics::Color::DarkBlue);
        bindColor["DarkCyan"] = sol::var(&obe::Graphics::Color::DarkCyan);
        bindColor["DarkGoldenrod"] = sol::var(&obe::Graphics::Color::DarkGoldenrod);
        bindColor["DarkGray"] = sol::var(&obe::Graphics::Color::DarkGray);
        bindColor["DarkGreen"] = sol::var(&obe::Graphics::Color::DarkGreen);
        bindColor["DarkKhaki"] = sol::var(&obe::Graphics::Color::DarkKhaki);
        bindColor["DarkMagenta"] = sol::var(&obe::Graphics::Color::DarkMagenta);
        bindColor["DarkOliveGreen"] = sol::var(&obe::Graphics::Color::DarkOliveGreen);
        bindColor["DarkOrange"] = sol::var(&obe::Graphics::Color::DarkOrange);
        bindColor["DarkOrchid"] = sol::var(&obe::Graphics::Color::DarkOrchid);
        bindColor["DarkRed"] = sol::var(&obe::Graphics::Color::DarkRed);
        bindColor["DarkSalmon"] = sol::var(&obe::Graphics::Color::DarkSalmon);
        bindColor["DarkSeaGreen"] = sol::var(&obe::Graphics::Color::DarkSeaGreen);
        bindColor["DarkSlateBlue"] = sol::var(&obe::Graphics::Color::DarkSlateBlue);
        bindColor["DarkSlateGray"] = sol::var(&obe::Graphics::Color::DarkSlateGray);
        bindColor["DarkTurquoise"] = sol::var(&obe::Graphics::Color::DarkTurquoise);
        bindColor["DarkViolet"] = sol::var(&obe::Graphics::Color::DarkViolet);
        bindColor["DeepPink"] = sol::var(&obe::Graphics::Color::DeepPink);
        bindColor["DeepSkyBlue"] = sol::var(&obe::Graphics::Color::DeepSkyBlue);
        bindColor["DimGray"] = sol::var(&obe::Graphics::Color::DimGray);
        bindColor["DodgerBlue"] = sol::var(&obe::Graphics::Color::DodgerBlue);
        bindColor["FireBrick"] = sol::var(&obe::Graphics::Color::FireBrick);
        bindColor["FloralWhite"] = sol::var(&obe::Graphics::Color::FloralWhite);
        bindColor["ForestGreen"] = sol::var(&obe::Graphics::Color::ForestGreen);
        bindColor["Fuchsia"] = sol::var(&obe::Graphics::Color::Fuchsia);
        bindColor["Gainsboro"] = sol::var(&obe::Graphics::Color::Gainsboro);
        bindColor["GhostWhite"] = sol::var(&obe::Graphics::Color::GhostWhite);
        bindColor["Gold"] = sol::var(&obe::Graphics::Color::Gold);
        bindColor["Goldenrod"] = sol::var(&obe::Graphics::Color::Goldenrod);
        bindColor["Gray"] = sol::var(&obe::Graphics::Color::Gray);
        bindColor["Green"] = sol::var(&obe::Graphics::Color::Green);
        bindColor["GreenYellow"] = sol::var(&obe::Graphics::Color::GreenYellow);
        bindColor["HoneyDew"] = sol::var(&obe::Graphics::Color::HoneyDew);
        bindColor["HotPink"] = sol::var(&obe::Graphics::Color::HotPink);
        bindColor["IndianRed"] = sol::var(&obe::Graphics::Color::IndianRed);
        bindColor["Indigo"] = sol::var(&obe::Graphics::Color::Indigo);
        bindColor["Ivory"] = sol::var(&obe::Graphics::Color::Ivory);
        bindColor["Khaki"] = sol::var(&obe::Graphics::Color::Khaki);
        bindColor["Lavender"] = sol::var(&obe::Graphics::Color::Lavender);
        bindColor["LavenderBlush"] = sol::var(&obe::Graphics::Color::LavenderBlush);
        bindColor["LawnGreen"] = sol::var(&obe::Graphics::Color::LawnGreen);
        bindColor["LemonChiffon"] = sol::var(&obe::Graphics::Color::LemonChiffon);
        bindColor["LightBlue"] = sol::var(&obe::Graphics::Color::LightBlue);
        bindColor["LightCoral"] = sol::var(&obe::Graphics::Color::LightCoral);
        bindColor["LightCyan"] = sol::var(&obe::Graphics::Color::LightCyan);
        bindColor["LightGoldenrodYellow"]
            = sol::var(&obe::Graphics::Color::LightGoldenrodYellow);
        bindColor["LightGray"] = sol::var(&obe::Graphics::Color::LightGray);
        bindColor["LightGreen"] = sol::var(&obe::Graphics::Color::LightGreen);
        bindColor["LightPink"] = sol::var(&obe::Graphics::Color::LightPink);
        bindColor["LightSalmon"] = sol::var(&obe::Graphics::Color::LightSalmon);
        bindColor["LightSeaGreen"] = sol::var(&obe::Graphics::Color::LightSeaGreen);
        bindColor["LightSkyBlue"] = sol::var(&obe::Graphics::Color::LightSkyBlue);
        bindColor["LightSlateGray"] = sol::var(&obe::Graphics::Color::LightSlateGray);
        bindColor["LightSteelBlue"] = sol::var(&obe::Graphics::Color::LightSteelBlue);
        bindColor["LightYellow"] = sol::var(&obe::Graphics::Color::LightYellow);
        bindColor["Lime"] = sol::var(&obe::Graphics::Color::Lime);
        bindColor["LimeGreen"] = sol::var(&obe::Graphics::Color::LimeGreen);
        bindColor["Linen"] = sol::var(&obe::Graphics::Color::Linen);
        bindColor["Magenta"] = sol::var(&obe::Graphics::Color::Magenta);
        bindColor["Maroon"] = sol::var(&obe::Graphics::Color::Maroon);
        bindColor["MediumAquamarine"] = sol::var(&obe::Graphics::Color::MediumAquamarine);
        bindColor["MediumBlue"] = sol::var(&obe::Graphics::Color::MediumBlue);
        bindColor["MediumOrchid"] = sol::var(&obe::Graphics::Color::MediumOrchid);
        bindColor["MediumPurple"] = sol::var(&obe::Graphics::Color::MediumPurple);
        bindColor["MediumSeaGreen"] = sol::var(&obe::Graphics::Color::MediumSeaGreen);
        bindColor["MediumSlateBlue"] = sol::var(&obe::Graphics::Color::MediumSlateBlue);
        bindColor["MediumSpringGreen"]
            = sol::var(&obe::Graphics::Color::MediumSpringGreen);
        bindColor["MediumTurquoise"] = sol::var(&obe::Graphics::Color::MediumTurquoise);
        bindColor["MediumVioletRed"] = sol::var(&obe::Graphics::Color::MediumVioletRed);
        bindColor["MidnightBlue"] = sol::var(&obe::Graphics::Color::MidnightBlue);
        bindColor["MintCream"] = sol::var(&obe::Graphics::Color::MintCream);
        bindColor["MistyRose"] = sol::var(&obe::Graphics::Color::MistyRose);
        bindColor["Moccasin"] = sol::var(&obe::Graphics::Color::Moccasin);
        bindColor["NavajoWhite"] = sol::var(&obe::Graphics::Color::NavajoWhite);
        bindColor["Navy"] = sol::var(&obe::Graphics::Color::Navy);
        bindColor["OldLace"] = sol::var(&obe::Graphics::Color::OldLace);
        bindColor["Olive"] = sol::var(&obe::Graphics::Color::Olive);
        bindColor["OliveDrab"] = sol::var(&obe::Graphics::Color::OliveDrab);
        bindColor["Orange"] = sol::var(&obe::Graphics::Color::Orange);
        bindColor["OrangeRed"] = sol::var(&obe::Graphics::Color::OrangeRed);
        bindColor["Orchid"] = sol::var(&obe::Graphics::Color::Orchid);
        bindColor["PaleGoldenrod"] = sol::var(&obe::Graphics::Color::PaleGoldenrod);
        bindColor["PaleGreen"] = sol::var(&obe::Graphics::Color::PaleGreen);
        bindColor["PaleTurquoise"] = sol::var(&obe::Graphics::Color::PaleTurquoise);
        bindColor["PaleVioletRed"] = sol::var(&obe::Graphics::Color::PaleVioletRed);
        bindColor["PapayaWhip"] = sol::var(&obe::Graphics::Color::PapayaWhip);
        bindColor["PeachPuff"] = sol::var(&obe::Graphics::Color::PeachPuff);
        bindColor["Peru"] = sol::var(&obe::Graphics::Color::Peru);
        bindColor["Pink"] = sol::var(&obe::Graphics::Color::Pink);
        bindColor["Plum"] = sol::var(&obe::Graphics::Color::Plum);
        bindColor["PowderBlue"] = sol::var(&obe::Graphics::Color::PowderBlue);
        bindColor["Purple"] = sol::var(&obe::Graphics::Color::Purple);
        bindColor["RebeccaPurple"] = sol::var(&obe::Graphics::Color::RebeccaPurple);
        bindColor["Red"] = sol::var(&obe::Graphics::Color::Red);
        bindColor["RosyBrown"] = sol::var(&obe::Graphics::Color::RosyBrown);
        bindColor["RoyalBlue"] = sol::var(&obe::Graphics::Color::RoyalBlue);
        bindColor["SaddleBrown"] = sol::var(&obe::Graphics::Color::SaddleBrown);
        bindColor["Salmon"] = sol::var(&obe::Graphics::Color::Salmon);
        bindColor["SandyBrown"] = sol::var(&obe::Graphics::Color::SandyBrown);
        bindColor["SeaGreen"] = sol::var(&obe::Graphics::Color::SeaGreen);
        bindColor["SeaShell"] = sol::var(&obe::Graphics::Color::SeaShell);
        bindColor["Sienna"] = sol::var(&obe::Graphics::Color::Sienna);
        bindColor["Silver"] = sol::var(&obe::Graphics::Color::Silver);
        bindColor["SkyBlue"] = sol::var(&obe::Graphics::Color::SkyBlue);
        bindColor["SlateBlue"] = sol::var(&obe::Graphics::Color::SlateBlue);
        bindColor["SlateGray"] = sol::var(&obe::Graphics::Color::SlateGray);
        bindColor["Snow"] = sol::var(&obe::Graphics::Color::Snow);
        bindColor["SpringGreen"] = sol::var(&obe::Graphics::Color::SpringGreen);
        bindColor["SteelBlue"] = sol::var(&obe::Graphics::Color::SteelBlue);
        bindColor["Tan"] = sol::var(&obe::Graphics::Color::Tan);
        bindColor["Teal"] = sol::var(&obe::Graphics::Color::Teal);
        bindColor["Thistle"] = sol::var(&obe::Graphics::Color::Thistle);
        bindColor["Tomato"] = sol::var(&obe::Graphics::Color::Tomato);
        bindColor["Turquoise"] = sol::var(&obe::Graphics::Color::Turquoise);
        bindColor["Violet"] = sol::var(&obe::Graphics::Color::Violet);
        bindColor["Wheat"] = sol::var(&obe::Graphics::Color::Wheat);
        bindColor["White"] = sol::var(&obe::Graphics::Color::White);
        bindColor["WhiteSmoke"] = sol::var(&obe::Graphics::Color::WhiteSmoke);
        bindColor["Yellow"] = sol::var(&obe::Graphics::Color::Yellow);
        bindColor["YellowGreen"] = sol::var(&obe::Graphics::Color::YellowGreen);
    }
    void LoadClassFont(sol::state_view state)
    {
        sol::table GraphicsNamespace = state["obe"]["Graphics"].get<sol::table>();
        sol::usertype<obe::Graphics::Font> bindFont
            = GraphicsNamespace.new_usertype<obe::Graphics::Font>("Font",
                sol::call_constructor,
                sol::constructors<obe::Graphics::Font(),
                    obe::Graphics::Font(const obe::Graphics::Font&),
                    obe::Graphics::Font(const sf::Font&)>());
        bindFont["loadFromFile"] = &obe::Graphics::Font::loadFromFile;
        bindFont[sol::meta_function::equal_to] = &obe::Graphics::Font::operator==;
        bindFont["operator sf::Font &"] = &obe::Graphics::Font::operator sf::Font &;
        bindFont["operator const sf::Font &"]
            = &obe::Graphics::Font::operator const sf::Font &;
        bindFont["operator bool"] = &obe::Graphics::Font::operator bool;
    }
    void LoadClassPositionTransformer(sol::state_view state)
    {
        sol::table GraphicsNamespace = state["obe"]["Graphics"].get<sol::table>();
        sol::usertype<obe::Graphics::PositionTransformer> bindPositionTransformer
            = GraphicsNamespace.new_usertype<obe::Graphics::PositionTransformer>(
                "PositionTransformer", sol::call_constructor,
                sol::constructors<obe::Graphics::PositionTransformer(),
                    obe::Graphics::PositionTransformer(
                        const std::string&, const std::string&)>());
        bindPositionTransformer["getXTransformer"]
            = &obe::Graphics::PositionTransformer::getXTransformer;
        bindPositionTransformer["getXTransformerName"]
            = &obe::Graphics::PositionTransformer::getXTransformerName;
        bindPositionTransformer["getYTransformer"]
            = &obe::Graphics::PositionTransformer::getYTransformer;
        bindPositionTransformer["getYTransformerName"]
            = &obe::Graphics::PositionTransformer::getYTransformerName;
        bindPositionTransformer[sol::meta_function::call]
            = &obe::Graphics::PositionTransformer::operator();
    }
    void LoadClassRenderTarget(sol::state_view state)
    {
        sol::table GraphicsNamespace = state["obe"]["Graphics"].get<sol::table>();
        sol::usertype<obe::Graphics::RenderTarget> bindRenderTarget
            = GraphicsNamespace.new_usertype<obe::Graphics::RenderTarget>("RenderTarget",
                sol::call_constructor,
                sol::constructors<obe::Graphics::RenderTarget(sf::RenderTarget&),
                    obe::Graphics::RenderTarget(sf::RenderWindow&)>());
        bindRenderTarget["draw"] = sol::overload(
            static_cast<void (obe::Graphics::RenderTarget::*)(const sf::Drawable&,
                const sf::RenderStates&) const>(&obe::Graphics::RenderTarget::draw),
            static_cast<void (obe::Graphics::RenderTarget::*)(const sf::Vertex*,
                std::size_t, sf::PrimitiveType, const sf::RenderStates&) const>(
                &obe::Graphics::RenderTarget::draw));
        bindRenderTarget["operator sf::RenderTarget &"]
            = &obe::Graphics::RenderTarget::operator sf::RenderTarget &;
        bindRenderTarget["operator const sf::RenderTarget &"]
            = &obe::Graphics::RenderTarget::operator const sf::RenderTarget &;
    }
    void LoadClassRichText(sol::state_view state)
    {
        sol::table GraphicsNamespace = state["obe"]["Graphics"].get<sol::table>();
        sol::usertype<obe::Graphics::RichText> bindRichText
            = GraphicsNamespace.new_usertype<obe::Graphics::RichText>("RichText",
                sol::call_constructor,
                sol::constructors<obe::Graphics::RichText(),
                    obe::Graphics::RichText(const obe::Graphics::Font&)>());
        bindRichText["clear"] = &obe::Graphics::RichText::clear;
        bindRichText["append"] = &obe::Graphics::RichText::append;
        bindRichText["getFont"] = &obe::Graphics::RichText::getFont;
        bindRichText["setFont"] = &obe::Graphics::RichText::setFont;
        bindRichText["getLines"] = &obe::Graphics::RichText::getLines;
        bindRichText["getCharacterSize"] = &obe::Graphics::RichText::getCharacterSize;
        bindRichText["setCharacterSize"] = &obe::Graphics::RichText::setCharacterSize;
        bindRichText["getLocalBounds"] = &obe::Graphics::RichText::getLocalBounds;
        bindRichText["getGlobalBounds"] = &obe::Graphics::RichText::getGlobalBounds;
    }
    void LoadClassShader(sol::state_view state)
    {
        sol::table GraphicsNamespace = state["obe"]["Graphics"].get<sol::table>();
        sol::usertype<obe::Graphics::Shader> bindShader
            = GraphicsNamespace.new_usertype<obe::Graphics::Shader>("Shader",
                sol::call_constructor,
                sol::constructors<obe::Graphics::Shader(),
                    obe::Graphics::Shader(const std::string&)>(),
                sol::base_classes, sol::bases<obe::Types::Serializable>());
        bindShader["dump"] = &obe::Graphics::Shader::dump;
        bindShader["load"] = &obe::Graphics::Shader::load;
        bindShader["loadShader"] = &obe::Graphics::Shader::loadShader;
    }
    void LoadClassSprite(sol::state_view state)
    {
        sol::table GraphicsNamespace = state["obe"]["Graphics"].get<sol::table>();
        sol::usertype<obe::Graphics::Sprite> bindSprite
            = GraphicsNamespace.new_usertype<obe::Graphics::Sprite>("Sprite",
                sol::call_constructor,
                sol::constructors<obe::Graphics::Sprite(const std::string&)>(),
                sol::base_classes,
                sol::bases<obe::Transform::UnitBasedObject, obe::Types::Selectable,
                    obe::Transform::Rect, obe::Transform::Movable,
                    obe::Component::Component<Sprite>, obe::Component::ComponentBase,
                    obe::Types::Identifiable, obe::Types::Serializable,
                    obe::Engine::ResourceManagedObject>());
        bindSprite["drawHandle"] = &obe::Graphics::Sprite::drawHandle;
        bindSprite["dump"] = &obe::Graphics::Sprite::dump;
        bindSprite["getColor"] = &obe::Graphics::Sprite::getColor;
        bindSprite["getHandlePoint"] = &obe::Graphics::Sprite::getHandlePoint;
        bindSprite["getLayer"] = &obe::Graphics::Sprite::getLayer;
        bindSprite["getParentId"] = &obe::Graphics::Sprite::getParentId;
        bindSprite["getPath"] = &obe::Graphics::Sprite::getPath;
        bindSprite["getPositionTransformer"]
            = &obe::Graphics::Sprite::getPositionTransformer;
        bindSprite["getRect"] = &obe::Graphics::Sprite::getRect;
        bindSprite["getShader"] = &obe::Graphics::Sprite::getShader;
        bindSprite["getSprite"] = &obe::Graphics::Sprite::getSprite;
        bindSprite["getSpriteHeight"] = &obe::Graphics::Sprite::getSpriteHeight;
        bindSprite["getSpriteWidth"] = &obe::Graphics::Sprite::getSpriteWidth;
        bindSprite["getTexture"] = &obe::Graphics::Sprite::getTexture;
        bindSprite["getXScaleFactor"] = &obe::Graphics::Sprite::getXScaleFactor;
        bindSprite["getYScaleFactor"] = &obe::Graphics::Sprite::getYScaleFactor;
        bindSprite["getZDepth"] = &obe::Graphics::Sprite::getZDepth;
        bindSprite["getAntiAliasing"] = &obe::Graphics::Sprite::getAntiAliasing;
        bindSprite["hasShader"] = &obe::Graphics::Sprite::hasShader;
        bindSprite["isVisible"] = &obe::Graphics::Sprite::isVisible;
        bindSprite["load"] = &obe::Graphics::Sprite::load;
        bindSprite["loadTexture"] = &obe::Graphics::Sprite::loadTexture;
        bindSprite["rotate"] = &obe::Graphics::Sprite::rotate;
        bindSprite["setColor"] = &obe::Graphics::Sprite::setColor;
        bindSprite["setLayer"] = &obe::Graphics::Sprite::setLayer;
        bindSprite["setParentId"] = &obe::Graphics::Sprite::setParentId;
        bindSprite["setPositionTransformer"]
            = &obe::Graphics::Sprite::setPositionTransformer;
        bindSprite["setRotation"] = &obe::Graphics::Sprite::setRotation;
        bindSprite["setRotationOrigin"] = &obe::Graphics::Sprite::setRotationOrigin;
        bindSprite["setScalingOrigin"] = &obe::Graphics::Sprite::setScalingOrigin;
        bindSprite["setShader"] = &obe::Graphics::Sprite::setShader;
        bindSprite["setTexture"] = &obe::Graphics::Sprite::setTexture;
        bindSprite["setTextureRect"] = &obe::Graphics::Sprite::setTextureRect;
        bindSprite["setTranslationOrigin"] = &obe::Graphics::Sprite::setTranslationOrigin;
        bindSprite["setVisible"] = &obe::Graphics::Sprite::setVisible;
        bindSprite["setZDepth"] = &obe::Graphics::Sprite::setZDepth;
        bindSprite["setAntiAliasing"] = &obe::Graphics::Sprite::setAntiAliasing;
        bindSprite["useTextureSize"] = &obe::Graphics::Sprite::useTextureSize;
        bindSprite["draw"] = &obe::Graphics::Sprite::draw;
        bindSprite["attachResourceManager"]
            = &obe::Graphics::Sprite::attachResourceManager;
        bindSprite["type"] = &obe::Graphics::Sprite::type;
        bindSprite["m_layerChanged"] = &obe::Graphics::Sprite::m_layerChanged;
    }
    void LoadClassSpriteHandlePoint(sol::state_view state)
    {
        sol::table GraphicsNamespace = state["obe"]["Graphics"].get<sol::table>();
        sol::usertype<obe::Graphics::SpriteHandlePoint> bindSpriteHandlePoint
            = GraphicsNamespace.new_usertype<obe::Graphics::SpriteHandlePoint>(
                "SpriteHandlePoint", sol::call_constructor,
                sol::constructors<obe::Graphics::SpriteHandlePoint(obe::Graphics::Sprite&,
                                      obe::Transform::Referential),
                    obe::Graphics::SpriteHandlePoint(obe::Graphics::Sprite&)>());
        bindSpriteHandlePoint["getRect"] = &obe::Graphics::SpriteHandlePoint::getRect;
        bindSpriteHandlePoint["getReferential"]
            = &obe::Graphics::SpriteHandlePoint::getReferential;
        bindSpriteHandlePoint["getType"] = &obe::Graphics::SpriteHandlePoint::getType;
        bindSpriteHandlePoint["moveTo"] = &obe::Graphics::SpriteHandlePoint::moveTo;
        bindSpriteHandlePoint["m_dp"] = &obe::Graphics::SpriteHandlePoint::m_dp;
        bindSpriteHandlePoint["radius"]
            = sol::var(&obe::Graphics::SpriteHandlePoint::radius);
    }
    void LoadClassText(sol::state_view state)
    {
        sol::table GraphicsNamespace = state["obe"]["Graphics"].get<sol::table>();
        sol::usertype<obe::Graphics::Text> bindText
            = GraphicsNamespace.new_usertype<obe::Graphics::Text>("Text",
                sol::call_constructor,
                sol::constructors<obe::Graphics::Text(),
                    obe::Graphics::Text(const std::string&)>());
        bindText["color"] = &obe::Graphics::Text::color;
        bindText["outline"] = &obe::Graphics::Text::outline;
        bindText["thickness"] = &obe::Graphics::Text::thickness;
        bindText["style"] = &obe::Graphics::Text::style;
        bindText["string"] = &obe::Graphics::Text::string;
    }
    void LoadClassTexture(sol::state_view state)
    {
        sol::table GraphicsNamespace = state["obe"]["Graphics"].get<sol::table>();
        sol::usertype<obe::Graphics::Texture> bindTexture
            = GraphicsNamespace.new_usertype<obe::Graphics::Texture>("Texture",
                sol::call_constructor,
                sol::constructors<obe::Graphics::Texture(),
                    obe::Graphics::Texture(std::shared_ptr<sf::Texture>),
                    obe::Graphics::Texture(const sf::Texture&),
                    obe::Graphics::Texture(const obe::Graphics::Texture&)>());
        bindTexture["create"] = &obe::Graphics::Texture::create;
        bindTexture["loadFromFile"] = sol::overload(
            static_cast<bool (obe::Graphics::Texture::*)(const std::string&)>(
                &obe::Graphics::Texture::loadFromFile),
            static_cast<bool (obe::Graphics::Texture::*)(const std::string&,
                const obe::Transform::Rect&)>(&obe::Graphics::Texture::loadFromFile));
        bindTexture["loadFromImage"] = &obe::Graphics::Texture::loadFromImage;
        bindTexture["getSize"] = &obe::Graphics::Texture::getSize;
        bindTexture["setAntiAliasing"] = &obe::Graphics::Texture::setAntiAliasing;
        bindTexture["isAntiAliased"] = &obe::Graphics::Texture::isAntiAliased;
        bindTexture["setRepeated"] = &obe::Graphics::Texture::setRepeated;
        bindTexture["isRepeated"] = &obe::Graphics::Texture::isRepeated;
        bindTexture["reset"] = &obe::Graphics::Texture::reset;
        bindTexture["useCount"] = &obe::Graphics::Texture::useCount;
        bindTexture["operator sf::Texture &"]
            = &obe::Graphics::Texture::operator sf::Texture &;
        bindTexture["operator const sf::Texture &"]
            = &obe::Graphics::Texture::operator const sf::Texture &;
        bindTexture["operator="] = sol::overload(
            static_cast<obe::Graphics::Texture& (
                obe::Graphics::Texture::*)(const obe::Graphics::Texture&)>(
                &obe::Graphics::Texture::operator=),
            static_cast<obe::Graphics::Texture& (
                obe::Graphics::Texture::*)(const sf::Texture&)>(
                &obe::Graphics::Texture::operator=),
            static_cast<obe::Graphics::Texture& (
                obe::Graphics::Texture::*)(std::shared_ptr<sf::Texture>)>(
                &obe::Graphics::Texture::operator=));
    }
    void LoadClassHsv(sol::state_view state)
    {
        sol::table GraphicsNamespace = state["obe"]["Graphics"].get<sol::table>();
        sol::usertype<obe::Graphics::Hsv> bindHsv
            = GraphicsNamespace.new_usertype<obe::Graphics::Hsv>(
                "Hsv", sol::call_constructor, sol::default_constructor);
        bindHsv["H"] = &obe::Graphics::Hsv::H;
        bindHsv["S"] = &obe::Graphics::Hsv::S;
        bindHsv["V"] = &obe::Graphics::Hsv::V;
    }
    void LoadFunctionInitPositionTransformer(sol::state_view state)
    {
        sol::table GraphicsNamespace = state["obe"]["Graphics"].get<sol::table>();
        GraphicsNamespace.set_function(
            "InitPositionTransformer", obe::Graphics::InitPositionTransformer);
    }
    void LoadFunctionMakeNullTexture(sol::state_view state)
    {
        sol::table GraphicsNamespace = state["obe"]["Graphics"].get<sol::table>();
        GraphicsNamespace.set_function("MakeNullTexture", obe::Graphics::MakeNullTexture);
    }
    void LoadGlobalTransformers(sol::state_view state)
    {
        sol::table GraphicsNamespace = state["obe"]["Graphics"].get<sol::table>();
        GraphicsNamespace["Transformers"] = obe::Graphics::Transformers;
    }
    void LoadGlobalParallax(sol::state_view state)
    {
        sol::table GraphicsNamespace = state["obe"]["Graphics"].get<sol::table>();
        GraphicsNamespace["Parallax"] = obe::Graphics::Parallax;
    }
    void LoadGlobalCamera(sol::state_view state)
    {
        sol::table GraphicsNamespace = state["obe"]["Graphics"].get<sol::table>();
        GraphicsNamespace["Camera"] = obe::Graphics::Camera;
    }
    void LoadGlobalPosition(sol::state_view state)
    {
        sol::table GraphicsNamespace = state["obe"]["Graphics"].get<sol::table>();
        GraphicsNamespace["Position"] = obe::Graphics::Position;
    }
};