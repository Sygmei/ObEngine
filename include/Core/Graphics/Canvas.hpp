#pragma once

#include <string>
#include <vector>

#include <SFML/Graphics/RenderTexture.hpp>
#include <sfe/RichText.hpp>

#include <Debug/Logger.hpp>
#include <Graphics/Exceptions.hpp>
#include <Graphics/Shapes.hpp>
#include <Graphics/Sprite.hpp>
#include <Graphics/Text.hpp>
#include <Transform/Polygon.hpp>
#include <Types/Identifiable.hpp>

namespace obe::Graphics::Canvas
{
    /**
     * \brief Type of the CanvasElement, used for identification
     * \lua_bind{Canvas.Type}
     */
    enum class CanvasElementType
    {
        CanvasElement,
        Line,
        Rectangle,
        Text,
        Circle,
        Polygon,
        Bezier
    };

    std::string canvasElementTypeToString(CanvasElementType type);
    std::ostream& operator<<(std::ostream& os, CanvasElementType type);

    class Canvas;
    /**
     * \brief A Drawable Canvas Element
     */
    class CanvasElement : public Types::ProtectedIdentifiable
    {
    public:
        static constexpr CanvasElementType Type = CanvasElementType::CanvasElement;

        /**
         * \nobind
         */
        Canvas& parent;
        unsigned int layer = 1;
        bool visible = true;
        CanvasElementType type = CanvasElementType::CanvasElement;

        /**
         * \brief Create a new CanvasElement
         * \param parent Reference to the Canvas
         * \param id Id of the new CanvasElement
         */
        explicit CanvasElement(Canvas& parent, const std::string& id);
        /**
         * \brief Abstract draw method
         * \param target Target where to render the result
         */
        virtual void draw(RenderTarget target) = 0;

        virtual ~CanvasElement() = default;
        CanvasElement& operator=(CanvasElement&&) = delete; //no copy nor move

        /**
         * \brief Change layer or object and will ask the Canvas to reorder
         *        elements automatically
         */
        void setLayer(unsigned int layer);

        using Ptr = std::unique_ptr<CanvasElement>;
    };

    /**
     * \brief A Canvas Line
     */
    class Line : public CanvasElement
    {
    public:
        Transform::UnitVector p1;
        Transform::UnitVector p2;
        unsigned int thickness = 1;
        Color p1color;
        Color p2color;
        static constexpr CanvasElementType Type = CanvasElementType::Line;

        /**
         * \brief Create a new Line
         * \param parent Reference to the Canvas
         * \param id Id of the new Line
         */
        explicit Line(Canvas& parent, const std::string& id);
        /**
         * \brief Draw the Line
         * \param target Target where to draw the Line to
         */
        void draw(RenderTarget target) override;
    };

    /**
     * \forceabstract
     * \brief Base class for CanvasElement classes with a position attribute
     */
    class CanvasPositionable : public CanvasElement
    {
    public:
        Transform::UnitVector position;
        CanvasPositionable(Canvas& parent, const std::string& id);
    };

    /**
     * \brief A Canvas Rectangle
     */
    class Rectangle : public CanvasPositionable
    {
    public:
        static constexpr CanvasElementType Type = CanvasElementType::Rectangle;

        Shapes::Rectangle shape;
        Transform::UnitVector size;
        /**
         * \brief Create a new Rectangle
         * \param parent Reference to the Canvas
         * \param id Id of the new Rectangle
         */
        explicit Rectangle(Canvas& parent, const std::string& id);
        /**
         * \brief Draw the Rectangle
         * \param target Target where to draw the Rectangle to
         */
        void draw(RenderTarget target) override;
    };

    /**
     * \brief Horizontal alignment state of Text
     * \lua_bind{Canvas.TextAlignment.Horizontal}
     */
    enum class TextHorizontalAlign
    {
        Left,
        Center,
        Right
    };

    /**
     * \brief Vertical alignment of Text
     * \lua_bind{Canvas.TextAlignment.Vertical}
     */
    enum class TextVerticalAlign
    {
        Top,
        Center,
        Bottom
    };

    /**
     * \brief A Canvas Text
     */
    class Text : public CanvasPositionable
    {
    public:
        static constexpr CanvasElementType Type = CanvasElementType::Text;

        std::string fontPath;
        Shapes::Text shape;
        TextHorizontalAlign h_align;
        TextVerticalAlign v_align;
        std::vector<Graphics::Text> texts;
        /**
         * \brief Create a new Text
         * \param parent Reference to the Canvas
         * \param id Id of the new Text
         */
        explicit Text(Canvas& parent, const std::string& id);
        /**
         * \brief Draw the Text
         * \param target Target where to draw the Text to
         */
        void draw(RenderTarget target) override;
        void refresh();
        /**
         * \bind{text}
         * \asproperty
         * \brief Returns the current Text part
         */
        Graphics::Text& currentText();
    };

    /**
     * \brief A Canvas Circle
     */
    class Circle : public CanvasPositionable
    {
    public:
        static constexpr CanvasElementType Type = CanvasElementType::Circle;

        Shapes::Circle shape;
        /**
         * \brief Create a new Circle
         * \param parent Reference to the Canvas
         * \param id Id of the new Circle
         */
        explicit Circle(Canvas& parent, const std::string& id);
        /**
         * \brief Draw the Circle
         * \param target Target where to draw the Circle to
         */
        void draw(RenderTarget target) override;
    };

    /**
     * \brief A Canvas Polygon
     */
    class Polygon : public CanvasPositionable
    {
    public:
        static constexpr CanvasElementType Type = CanvasElementType::Polygon;

        Shapes::Polygon shape;
        // Transform::Polygon polygon;

        explicit Polygon(Canvas& parent, const std::string& id);

        void draw(RenderTarget target) override;
    };

    /**
     * \brief A Canvas Bezier Curve
     */
    class Bezier : public CanvasElement
    {
    public:
        static constexpr CanvasElementType Type = CanvasElementType::Bezier;
        std::vector<Transform::UnitVector> points;
        std::vector<Graphics::Color> colors;
        unsigned int precision = 10;

        explicit Bezier(Canvas& parent, const std::string& id);
        /**
         * \brief Draw the Bezier Curve
         * \param target Target where to draw the Sprite to
         */
        void draw(RenderTarget target) override;
    };

    /**
     * \brief A Canvas where you can draw CanvasElements
     * \bind{Canvas}
     * \helper{Lib/Internal/Canvas.lua}
     */
    class Canvas
    {
    private:
        sf::RenderTexture m_canvas;
        std::vector<CanvasElement::Ptr> m_elements {};
        bool m_sortRequired = true;
        void sortElements();

    public:
        /**
         * \brief Create a new Canvas
         * \param width Width of the Canvas (in pixels)
         * \param height Height of the Canvas (in pixels)
         */
        Canvas(unsigned int width, unsigned int height);

        /**
         * \brief Adds a new CanvasElement of type T to the Canvas
         * \tparam T Class of the CanvasElement to add to the canvas
         * \param id Id of the new element to add to the canvas
         * \return a pointer to the newly created CanvasElement
         * \throw CanvasElementAlreadyExists if an element with
         *        the given id already exists in the Canvas
         *
         * \thints
         * \thint{Line, T=obe::Graphics::Canvas::Line}
         * \thint{Rectangle, T=obe::Graphics::Canvas::Rectangle}
         * \thint{Text, T=obe::Graphics::Canvas::Text}
         * \thint{Circle, T=obe::Graphics::Canvas::Circle}
         * \thint{Polygon, T=obe::Graphics::Canvas::Polygon}
         * \thint{Bezier, T=obe::Graphics::Canvas::Bezier}
         * \endthints
         *
         */
        template <class T> T& add(const std::string& id);

        /**
         * \brief Get a CanvasElement with the given id
         * \param id Id of the CanvasElement you want to retrieve
         * \return pointer to the CanvasElement with given id
         */
        CanvasElement* get(const std::string& id);

        /**
         * \brief Render all the Canvas content to the Sprite target
         */
        void render(Sprite& target);
        /**
         * \brief Clear all CanvasElement from the Canvas
         */
        void clear();
        /**
         * \brief Remove a CanvasElement from the Canvas
         * \param id Id of the CanvasElement to remove
         */
        void remove(const std::string& id);
        /**
         * \brief Get the current Texture of the Canvas
         * \return A reference to the current Texture of the Canvas
         */
        Texture getTexture() const;
        /**
         * \brief Ask the Canvas to sort elements for the next rendering
         */
        void requiresSort();
    };

    template <class T> inline T& Canvas::add(const std::string& id)
    {
        if (const auto existingElement = this->get(id); existingElement)
        {
            if (existingElement->type == T::Type)
            {
                Debug::Log->warn("<Scene> CanvasElement '{0}' already exists !", id);
                return *static_cast<T*>(this->get(id));
            }
            else
            {
                throw Exceptions::CanvasElementAlreadyExists(id,
                    canvasElementTypeToString(T::Type),
                    canvasElementTypeToString(existingElement->type), EXC_INFO);
            }
        }
        else
        {
            m_sortRequired = true;
            std::unique_ptr<T> newElement = std::make_unique<T>(*this, id);
            auto insert_it = std::find_if(m_elements.begin(), m_elements.end(),
                [&newElement](const CanvasElement::Ptr& elem) {
                    return newElement->layer <= elem->layer;
                });
            auto elem_it = m_elements.insert(insert_it, std::move(newElement));
            return *static_cast<T*>(elem_it->get());
        }
    }
} // namespace obe::Graphics::Canvas
