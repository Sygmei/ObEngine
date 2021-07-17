
#include <Graphics/Texture.hpp>

#include <Graphics/Exceptions.hpp>
#include <Utils/MiscUtils.hpp>

#include <lunasvg/svgdocument.h>

#include "Debug/Logger.hpp"
#include <iostream>

namespace obe::Graphics
{

    namespace
    {
        using VariantTexture
            = std::variant<sf::Texture, std::shared_ptr<sf::Texture>, const sf::Texture*>;

        //Gets the underlying sf::Texture
        sf::Texture& getMutableTexture(VariantTexture& texture)
        {
            constexpr static obe::Utils::Misc::Visitor visitor {
                [](sf::Texture& texture) -> sf::Texture& { return texture; },
                [](std::shared_ptr<sf::Texture>& texture) -> sf::Texture& {
                    return *texture;
                },
                [](const sf::Texture*) -> sf::Texture& {
                    throw Exceptions::ReadOnlyTexture("create", EXC_INFO);
                },
            };
            return std::visit(visitor, texture);
        }

        const sf::Texture& getTexture(const VariantTexture& texture)
        {
            constexpr static obe::Utils::Misc::Visitor visitor {
                [](const sf::Texture& texture) -> const sf::Texture& { return texture; },
                [](const std::shared_ptr<sf::Texture>& texture) -> const sf::Texture& {
                    return *texture;
                },
                [](const sf::Texture* texture) -> const sf::Texture& { return *texture; },
            };
            return std::visit(visitor, texture);
        }

        sf::IntRect toSfRect(const Transform::Rect& rect)
        {
            const Transform::UnitVector position
                = rect.getPosition().to<Transform::Units::ScenePixels>();
            const Transform::UnitVector size
                = rect.getPosition().to<Transform::Units::ScenePixels>();
            const sf::IntRect sfRect(position.x, position.y, size.x, size.y);
            return sfRect;
        }

        sf::Image loadSvgFromFile(const std::string filename)
        {
            lunasvg::SVGDocument document;
            document.loadFontFromFile(filename);
            //renderToBitmap by default uses the original document's dimension
            const auto bitmap = document.renderToBitmap();
            sf::Image image;
            image.create(bitmap.width(), bitmap.height(), bitmap.data());
            return image;
        }

    } //namespace

    Texture::Texture()
    {
        m_texture = sf::Texture {};
        static_assert(std::is_same_v<decltype(m_texture), VariantTexture>, "");
    }

    Texture::Texture(std::shared_ptr<sf::Texture> texture)
    {
        m_texture = texture;
    }

    Texture::Texture(const sf::Texture& texture)
    {
        m_texture = &texture;
    }

    Texture::Texture(const Texture& copy)
    {
        if (std::holds_alternative<sf::Texture>(copy.m_texture))
        {
            m_texture = &std::get<sf::Texture>(copy.m_texture);
        }
        else
        {
            m_texture = copy.m_texture;
        }
    }

    bool Texture::create(unsigned width, unsigned height)
    {
        return getMutableTexture(m_texture).create(width, height);
    }

    bool Texture::loadFromFile(const std::string& filename)
    {
        if (Utils::String::endsWith(filename, ".svg"))
        {
            const auto image = loadSvgFromFile(filename);
            return getMutableTexture(m_texture).loadFromImage(image);
        }
        return getMutableTexture(m_texture).loadFromFile(filename);
    }

    bool Texture::loadFromFile(const std::string& filename, const Transform::Rect& rect)
    {
        const sf::IntRect sfRect = toSfRect(rect);
        if (Utils::String::endsWith(filename, ".svg"))
        {
            const auto image = loadSvgFromFile(filename);
            return getMutableTexture(m_texture).loadFromImage(image, sfRect);
        }
        return getMutableTexture(m_texture).loadFromFile(filename, sfRect);
    }

    bool Texture::loadFromImage(const sf::Image& image)
    {
        return getMutableTexture(m_texture).loadFromImage(image);
    }

    Transform::UnitVector Texture::getSize() const
    {
        const sf::Vector2u textureSize = getTexture(m_texture).getSize();
        return Transform::UnitVector(
            textureSize.x, textureSize.y, Transform::Units::ScenePixels);
    }

    void Texture::setAntiAliasing(bool antiAliasing)
    {
        getMutableTexture(m_texture).setSmooth(antiAliasing);
    }

    bool Texture::isAntiAliased() const
    {
        return getTexture(m_texture).isSmooth();
    }

    void Texture::setRepeated(bool repeated)
    {
        getMutableTexture(m_texture).setRepeated(repeated);
    }

    bool Texture::isRepeated() const
    {
        return getTexture(m_texture).isRepeated();
    }

    void Texture::reset()
    {
        m_texture = sf::Texture {};
    }

    unsigned Texture::useCount()
    {
        if (std::holds_alternative<sf::Texture>(m_texture))
        {
            return 1;
        }
        if (std::holds_alternative<std::shared_ptr<sf::Texture>>(m_texture))
        {
            return std::get<std::shared_ptr<sf::Texture>>(m_texture).use_count();
        }
        if (std::holds_alternative<const sf::Texture*>(m_texture))
        {
            return 0;
        }
        return 0;
    }

    Texture::operator sf::Texture&()
    {
        return getMutableTexture(m_texture);
    }

    Texture::operator const sf::Texture&() const
    {
        return getTexture(m_texture);
    }

    Texture& Texture::operator=(const Texture& copy)
    {
        if (std::holds_alternative<sf::Texture>(copy.m_texture))
        {
            m_texture = &std::get<sf::Texture>(copy.m_texture);
        }
        else
        {
            m_texture = copy.m_texture;
        }
        return *this;
    }

    Texture& Texture::operator=(const sf::Texture& texture)
    {
        m_texture = &texture;
        return *this;
    }

    Texture& Texture::operator=(std::shared_ptr<sf::Texture> texture)
    {
        m_texture = texture;
        return *this;
    }
} //namespace obe::Graphics
