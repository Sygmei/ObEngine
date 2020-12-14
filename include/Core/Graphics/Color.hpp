#pragma once

#include <string>

#include <SFML/Graphics/Color.hpp>
#include <Types/Serializable.hpp>

namespace obe::Graphics
{
    struct Hsv
    {
        int H;
        double S, V;
    };

    enum class ColorType
    {
        Rgba,
        Hsv,
        Hex,
        ColorName
    };

    /**
     * \brief A class to handle colors
     * \bind{Color}
     */
    class Color : public Types::Serializable
    {
    public:

        double r = 0;
        double g = 0;
        double b = 0;
        double a = 255;

        Color();
        Color(double r, double g, double b, double a = 255);
        explicit Color(const std::string& nameOrHex);
        Color(const Color& color);
        Color(const sf::Color& color);

        vili::node dump(ColorType type);
        vili::node dump() const override;
        void load(vili::node& data) override;

        void fromString(std::string string);
        bool fromName(std::string name, bool strict = true);
        void fromHex(std::string hexCode);
        void fromRgb(double r, double g, double b, double a = 255);
        void fromHsv(int H, double S, double V);

        [[nodiscard]] uint32_t toInteger() const;

        bool operator==(const Color& color) const;
        bool operator!=(const Color& color) const;
        Color operator+(const Color& color) const;
        void operator+=(const Color& color);
        Color operator-(const Color& color) const;
        void operator-=(const Color& color);
        Color operator*(const Color& color) const;
        Color operator*(double value) const;
        void operator*=(const Color& color);
        void operator*=(double value);
        Color operator/(const Color& color) const;
        Color operator/(double value) const;
        void operator/=(const Color& color);
        void operator/=(double value);
        Color operator-() const;

        operator sf::Color() const;

        static Color AliceBlue;
        static Color AntiqueWhite;
        static Color Aqua;
        static Color Aquamarine;
        static Color Azure;
        static Color Beige;
        static Color Bisque;
        static Color Black;
        static Color BlanchedAlmond;
        static Color Blue;
        static Color BlueViolet;
        static Color Brown;
        static Color BurlyWood;
        static Color CadetBlue;
        static Color Chartreuse;
        static Color Chocolate;
        static Color Coral;
        static Color CornflowerBlue;
        static Color Cornsilk;
        static Color Crimson;
        static Color Cyan;
        static Color DarkBlue;
        static Color DarkCyan;
        static Color DarkGoldenrod;
        static Color DarkGray;
        static Color DarkGreen;
        static Color DarkKhaki;
        static Color DarkMagenta;
        static Color DarkOliveGreen;
        static Color DarkOrange;
        static Color DarkOrchid;
        static Color DarkRed;
        static Color DarkSalmon;
        static Color DarkSeaGreen;
        static Color DarkSlateBlue;
        static Color DarkSlateGray;
        static Color DarkTurquoise;
        static Color DarkViolet;
        static Color DeepPink;
        static Color DeepSkyBlue;
        static Color DimGray;
        static Color DodgerBlue;
        static Color FireBrick;
        static Color FloralWhite;
        static Color ForestGreen;
        static Color Fuchsia;
        static Color Gainsboro;
        static Color GhostWhite;
        static Color Gold;
        static Color Goldenrod;
        static Color Gray;
        static Color Green;
        static Color GreenYellow;
        static Color HoneyDew;
        static Color HotPink;
        static Color IndianRed;
        static Color Indigo;
        static Color Ivory;
        static Color Khaki;
        static Color Lavender;
        static Color LavenderBlush;
        static Color LawnGreen;
        static Color LemonChiffon;
        static Color LightBlue;
        static Color LightCoral;
        static Color LightCyan;
        static Color LightGoldenrodYellow;
        static Color LightGray;
        static Color LightGreen;
        static Color LightPink;
        static Color LightSalmon;
        static Color LightSeaGreen;
        static Color LightSkyBlue;
        static Color LightSlateGray;
        static Color LightSteelBlue;
        static Color LightYellow;
        static Color Lime;
        static Color LimeGreen;
        static Color Linen;
        static Color Magenta;
        static Color Maroon;
        static Color MediumAquamarine;
        static Color MediumBlue;
        static Color MediumOrchid;
        static Color MediumPurple;
        static Color MediumSeaGreen;
        static Color MediumSlateBlue;
        static Color MediumSpringGreen;
        static Color MediumTurquoise;
        static Color MediumVioletRed;
        static Color MidnightBlue;
        static Color MintCream;
        static Color MistyRose;
        static Color Moccasin;
        static Color NavajoWhite;
        static Color Navy;
        static Color OldLace;
        static Color Olive;
        static Color OliveDrab;
        static Color Orange;
        static Color OrangeRed;
        static Color Orchid;
        static Color PaleGoldenrod;
        static Color PaleGreen;
        static Color PaleTurquoise;
        static Color PaleVioletRed;
        static Color PapayaWhip;
        static Color PeachPuff;
        static Color Peru;
        static Color Pink;
        static Color Plum;
        static Color PowderBlue;
        static Color Purple;
        static Color RebeccaPurple;
        static Color Red;
        static Color RosyBrown;
        static Color RoyalBlue;
        static Color SaddleBrown;
        static Color Salmon;
        static Color SandyBrown;
        static Color SeaGreen;
        static Color SeaShell;
        static Color Sienna;
        static Color Silver;
        static Color SkyBlue;
        static Color SlateBlue;
        static Color SlateGray;
        static Color Snow;
        static Color SpringGreen;
        static Color SteelBlue;
        static Color Tan;
        static Color Teal;
        static Color Thistle;
        static Color Tomato;
        static Color Turquoise;
        static Color Violet;
        static Color Wheat;
        static Color White;
        static Color WhiteSmoke;
        static Color Yellow;
        static Color YellowGreen;

    private:
        Hsv toHsv() const;
        std::string toNamed() const;
        std::string toHex() const;
        std::string toString() const;

        ColorType type;

    };

    std::ostream& operator<<(std::ostream& os, const Color& color);
} // namespace obe::Graphics