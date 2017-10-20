/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TGUI - Texus' Graphical User Interface
// Copyright (C) 2012-2017 Bruno Van de Velde (vdv_b@tgui.eu)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef TGUI_GUI_HPP
#define TGUI_GUI_HPP


#include <TGUI/Container.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <queue>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Gui class
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class TGUI_API Gui
    {
      public:

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Default constructor
        ///
        /// If you use this constructor then you will still have to call the setTarget yourself.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Gui();

#if SFML_VERSION_MAJOR == 2 && SFML_VERSION_MINOR < 5
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Constructs the gui and set the window on which the gui should be drawn
        ///
        /// @param window  The sfml window that will be used by the gui
        ///
        /// If you use this constructor then you will no longer have to call setTarget yourself.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Gui(sf::RenderWindow& window);
#endif

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Constructs the gui and set the target on which the gui should be drawn
        ///
        /// @param target  The render target (typically sf::RenderWindow) that will be used by the gui
        ///
        /// If you use this constructor then you will no longer have to call setTarget yourself.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Gui(sf::RenderTarget& target);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Deleted copy constructor
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Gui(const Gui& copy) = delete;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Deleted assignment operator overload
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Gui& operator=(const Gui& right) = delete;

#if SFML_VERSION_MAJOR == 2 && SFML_VERSION_MINOR < 5
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Sets the window on which the gui should be drawn
        ///
        /// @param window  The sfml window that will be used by the gui
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setTarget(sf::RenderWindow& window);
#endif

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Sets the target on which the gui should be drawn
        ///
        /// @param target  The render target (typically sf::RenderWindow) that will be used by the gui
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setTarget(sf::RenderTarget& target);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the render target on which the gui is being drawn
        ///
        /// @return The sfml render target that is used by the gui
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        sf::RenderTarget* getTarget() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Changes the view that is used by the gui
        ///
        /// @param view  The new view
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setView(const sf::View& view);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the view that is currently used by the gui
        ///
        /// @return Currently set view
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        const sf::View& getView() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Passes the event to the widgets
        ///
        /// @param event  The event that was polled from the gui
        ///
        /// @return Has the event been consumed?
        ///         When this function returns false, then the event was ignored by all widgets.
        ///
        /// You should call this function in your event loop.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool handleEvent(sf::Event event);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Draws all the widgets that were added to the gui
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void draw();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the internal container of the Gui
        ///
        /// This could be useful when having a function that should accept both the gui and e.g. a child window as parameter.
        ///
        /// @warning Not all functions in the Container class make sense for the Gui (which is the reason that the Gui does not
        ///          inherit from Container). So calling some functions (e.g. setSize) will have no effect.
        ///
        /// @return Reference to the internal Container class
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        GuiContainer::Ptr getContainer() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Changes the global font
        ///
        /// @param font  Font to use
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setFont(const Font& font);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the global font
        ///
        /// @return global font
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        std::shared_ptr<sf::Font> getFont() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns a list of all the widgets
        ///
        /// @return Vector of all widget pointers
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        const std::vector<Widget::Ptr>& getWidgets() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns a list of the names of all the widgets
        ///
        /// @return Vector of all widget names
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        const std::vector<sf::String>& getWidgetNames() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Adds a widget to the container
        ///
        /// @param widgetPtr   Pointer to the widget you would like to add
        /// @param widgetName  If you want to access the widget later then you must do this with this name
        ///
        /// @warning The widget name should not contain whitespace
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void add(const Widget::Ptr& widgetPtr, const sf::String& widgetName = "");


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns a pointer to an earlier created widget
        ///
        /// @param widgetName The name that was given to the widget when it was added to the container
        /// @param recursive  Should the function also search for widgets inside containers that are inside this container?
        ///
        /// @return Pointer to the earlier created widget
        ///
        /// The gui will first search for widgets that are direct children of it, but when none of the child widgets match
        /// the given name, a recursive search will be performed.
        ///
        /// @warning This function will return nullptr when an unknown widget name was passed
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Widget::Ptr get(const sf::String& widgetName) const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns a pointer to an earlier created widget
        ///
        /// @param widgetName The name that was given to the widget when it was added to the container
        /// @param recursive  Should the function also search for widgets inside containers that are inside this container?
        ///
        /// @return Pointer to the earlier created widget.
        ///         The pointer will already be casted to the desired type
        ///
        /// The gui will first search for widgets that are direct children of it, but when none of the child widgets match
        /// the given name, a recursive search will be performed.
        ///
        /// @warning This function will return nullptr when an unknown widget name was passed
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        template <class T>
        typename T::Ptr get(const sf::String& widgetName) const
        {
            return m_container->get<T>(widgetName);
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Removes a single widget that was added to the container
        ///
        /// @param widget  Pointer to the widget to remove
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool remove(const Widget::Ptr& widget);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Removes all widgets that were added to the container
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void removeAllWidgets();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Changes the name of a widget
        ///
        /// @param widget  Widget of which the name should be changed
        /// @param name    New name for the widget
        ///
        /// @return True when the name was changed, false when the widget wasn't part of this container
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool setWidgetName(const Widget::Ptr& widget, const std::string& name);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the name of a widget
        ///
        /// @param widget  Widget of which the name should be retrieved
        ///
        /// @return Name of the widget or an empty string when the widget didn't exist or wasn't given a name
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        std::string getWidgetName(const Widget::Ptr& widget) const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Focuses a widget
        ///
        /// The previously focused widget will be unfocused.
        ///
        /// @param widget  The widget that has to be focused
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void focusWidget(const Widget::Ptr& widget);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Focuses the next widget
        ///
        /// The currently focused widget will be unfocused, even if it was the only widget.
        /// When no widget was focused, the first widget in the container will be focused.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void focusNextWidget();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Focuses the previous widget
        ///
        /// The currently focused widget will be unfocused, even if it was the only widget.
        /// When no widget was focused, the last widget in the container will be focused.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void focusPreviousWidget();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Unfocus all the widgets
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void unfocusWidgets();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Unchecks all the radio buttons
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void uncheckRadioButtons();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Changes the opacity of all widgets
        ///
        /// @param opacity  The opacity of the widgets. 0 means completely transparent, while 1 (default) means fully opaque
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setOpacity(float opacity);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the opacity of all the widgets
        ///
        /// @return The opacity of the widgets. 0 means completely transparent, while 1 (default) means fully opaque
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        float getOpacity() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Loads the child widgets from a text file
        ///
        /// @param filename  Filename of the widget file
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void loadWidgetsFromFile(const std::string& filename);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Saves the child widgets to a text file
        ///
        /// @param filename  Filename of the widget file
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void saveWidgetsToFile(const std::string& filename);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Loads the child widgets from a string stream
        ///
        /// @param stream  stringstream that contains the widget file
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void loadWidgetsFromStream(std::stringstream& stream);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Loads the child widgets from a string stream
        ///
        /// @param stream  stringstream that contains the widget file
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void loadWidgetsFromStream(std::stringstream&& stream);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Saves this the child widgets to a text file
        ///
        /// @param stream  stringstream to which the widget file will be added
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void saveWidgetsToStream(std::stringstream& stream) const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @internal
        // Updates the internal clock to make animation possible. This function is called automatically by the draw function.
        // You will thus only need to call it yourself when you are drawing everything manually.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void updateTime(const sf::Time& elapsedTime);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      protected:

        // The internal clock which is used for animation of widgets
        sf::Clock m_clock;

        // The sfml render target to draw on
        sf::RenderTarget* m_target;

    #if SFML_VERSION_MAJOR == 2 && SFML_VERSION_MINOR < 5
        // Does m_target contains a sf::RenderWindow?
        bool m_accessToWindow;
    #endif

        // Internal container to store all widgets
        GuiContainer::Ptr m_container = std::make_shared<GuiContainer>();

        Widget::Ptr m_visibleToolTip = nullptr;
        sf::Time m_tooltipTime;
        bool m_tooltipPossible = false;
        sf::Vector2f m_lastMousePos;

        sf::View m_view;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // TGUI_GUI_HPP