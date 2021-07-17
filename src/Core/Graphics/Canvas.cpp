#include <SFML/Graphics/Vertex.hpp>
#include <bezier/bezier.h>

#include <Graphics/Canvas.hpp>
#include <System/Loaders.hpp>
#include <Utils/StringUtils.hpp>

namespace obe::Graphics::Canvas
{
    std::string canvasElementTypeToString(CanvasElementType type)
    {
        switch (type)
        {
        case CanvasElementType::CanvasElement:
            return "CanvasElement";
            break;
        case CanvasElementType::Line:
            return "Line";
            break;
        case CanvasElementType::Rectangle:
            return "Rectangle";
            break;
        case CanvasElementType::Text:
            return "Text";
            break;
        case CanvasElementType::Circle:
            return "Circle";
            break;
        case CanvasElementType::Polygon:
            return "Polygon";
            break;
        case CanvasElementType::Bezier:
            return "Bezier";
            break;
        default:;
        }
    }

    std::ostream& operator<<(std::ostream& os, CanvasElementType type)
    {
        os << canvasElementTypeToString(type);
        return os;
    }

    CanvasElement::CanvasElement(Canvas& parent, const std::string& id)
        : ProtectedIdentifiable(id)
        , parent(parent)
    {
    }

    void CanvasElement::setLayer(const unsigned int layer)
    {
        if (this->layer != layer)
        {
            this->layer = layer;
            parent.requiresSort();
        }
    }

    Line::Line(Canvas& parent, const std::string& id)
        : CanvasElement(parent, id)
    {
    }

    void Line::draw(RenderTarget target)
    {
        const Transform::UnitVector p1px = p1.to<Transform::Units::ScenePixels>();
        const Transform::UnitVector p2px = p2.to<Transform::Units::ScenePixels>();
        const sf::Vertex firstVertex(sf::Vector2f(p1px.x, p1px.y), p1color);
        const sf::Vertex secondVertex(sf::Vector2f(p2px.x, p2px.y), p2color);
        const sf::Vertex line[] = { firstVertex, secondVertex };
        target.draw(line, 2, sf::Lines);
    }

    CanvasPositionable::CanvasPositionable(Canvas& parent, const std::string& id)
        : CanvasElement(parent, id)
    {
        // Default Canvas elements unit is ScenePixels
        position.unit = Transform::Units::ScenePixels;
    }

    Rectangle::Rectangle(Canvas& parent, const std::string& id)
        : CanvasPositionable(parent, id)
    {
        this->size.unit = Transform::Units::ScenePixels;
    }

    void Rectangle::draw(RenderTarget target)
    {
        target.draw(shape);
    }

    Text::Text(Canvas& parent, const std::string& id)
        : CanvasPositionable(parent, id)
        , h_align()
        , v_align()
    {
        texts.emplace_back();
    }

    void Text::draw(RenderTarget target)
    {
        Transform::UnitVector offset(Transform::Units::ScenePixels);
        if (h_align == TextHorizontalAlign::Center)
            offset.x -= shape.getGlobalBounds().getSize().x / 2;
        else if (h_align == TextHorizontalAlign::Right)
            offset.x -= shape.getGlobalBounds().getSize().x;
        if (v_align == TextVerticalAlign::Center)
            offset.y -= shape.getGlobalBounds().getSize().y / 2;
        else if (v_align == TextVerticalAlign::Bottom)
            offset.y -= shape.getGlobalBounds().getSize().y;
        shape.move(offset);
        target.draw(shape);
        shape.move(-offset);
    }

    void Text::refresh()
    {
        shape.clear();
        for (const auto& text : texts)
        {
            if (!text.string.empty())
            {
                shape.append(text);
            }
        }
    }

    Graphics::Text& Text::currentText()
    {
        return texts.back();
    }

    Circle::Circle(Canvas& parent, const std::string& id)
        : CanvasPositionable(parent, id)
    {
    }

    void Circle::draw(RenderTarget target)
    {
        target.draw(shape);
    }

    Polygon::Polygon(Canvas& parent, const std::string& id)
        : CanvasPositionable(parent, id)
    {
    }

    void Polygon::draw(RenderTarget target)
    {
        target.draw(shape);
    }

    Bezier::Bezier(Canvas& parent, const std::string& id)
        : CanvasElement(parent, id)
    {
    }

    void Bezier::draw(RenderTarget target)
    {
        std::vector<::Bezier::Point> controlPoints;
        controlPoints.reserve(points.size());
        for (Transform::UnitVector& point : points)
        {
            const Transform::UnitVector pixelPosition
                = point.to<Transform::Units::ScenePixels>();
            controlPoints.push_back(::Bezier::Point(pixelPosition.x, pixelPosition.y));
        }
        std::vector<::Bezier::Bezier<3>> bezierCurves;
        bezierCurves.reserve((controlPoints.size() - 1) / 3);
        for (std::size_t i = 3; i < controlPoints.size(); i += 3)
        {
            auto bezierPoints = std::vector<::Bezier::Point>(
                controlPoints.begin() + (i - 3), controlPoints.begin() + (i + 1));
            bezierCurves.push_back(bezierPoints);
        }
        const std::size_t maximum = bezierCurves.size() * precision;
        std::vector<sf::Vertex> vertices;
        vertices.reserve(maximum);

        std::size_t curveIndex = 0;
        for (::Bezier::Bezier<3>& bezier : bezierCurves)
        {
            for (std::size_t i = 0; i < (precision % 2 ? precision : precision + 1); i++)
            {
                double t = static_cast<double>(i) / precision;
                double tc = fmod(t * 4, 4);
                Color firstColor = colors[((t >= 1) ? 3 : floor(t * 4)) + curveIndex];
                Color secondColor = colors[((t >= 0.75) ? 3 : ceil(t * 4)) + curveIndex];
                Color color = (firstColor * (1 - tc)) + (secondColor * tc);
                ::Bezier::Point p = bezier.valueAt(t);
                vertices.emplace_back(sf::Vector2f(p.x, p.y), color);
            }
            curveIndex += 3;
        }
        target.draw(vertices.data(), maximum, sf::LineStrip);
    }

    void Canvas::sortElements()
    {
        std::sort(m_elements.begin(), m_elements.end(),
            [](const auto& elem1, const auto& elem2) {
                return elem1->layer > elem2->layer;
            });
    }

    Canvas::Canvas(unsigned int width, unsigned int height)
    {
        m_canvas.create(width, height);
    }

    CanvasElement* Canvas::get(const std::string& id)
    {
        for (auto& elem : m_elements)
        {
            if (elem->getId() == id)
            {
                return elem.get();
            }
        }
        return nullptr;
    }

    void Canvas::render(Sprite& target)
    {
        m_canvas.clear(sf::Color(0, 0, 0, 0));

        if (m_sortRequired)
        {
            this->sortElements();
            m_sortRequired = false;
        }

        for (auto& element : m_elements)
        {
            if (element->visible)
                element->draw(m_canvas);
        }
        m_canvas.display();
        target.setTexture(m_canvas.getTexture());
    }

    void Canvas::clear()
    {
        m_elements.clear();
    }

    void Canvas::remove(const std::string& id)
    {
        m_elements.erase(std::remove_if(m_elements.begin(), m_elements.end(),
                             [&id](auto& elem) { return elem->getId() == id; }),
            m_elements.end());
    }

    Texture Canvas::getTexture() const
    {
        return m_canvas.getTexture();
    }

    void Canvas::requiresSort()
    {
        m_sortRequired = true;
    }
} // namespace obe::Graphics::Canvas
