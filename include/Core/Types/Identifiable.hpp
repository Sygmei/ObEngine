#pragma once

#include <string>

namespace obe::Types
{
    /**
     * \brief A Base class that does carry an unique identifier for a given
     *        category (Sprites / Colliders etc..)
     */
    class Identifiable
    {
    protected:
        std::string m_id;

    public:
        virtual ~Identifiable() = default;
        Identifiable& operator=(Identifiable&&) = delete; //no copy nor move

        /**
         * \brief Creates a new Identifiable
         * \param id A std::string containing the id of the Identifiable
         */
        explicit Identifiable(const std::string& id);
        /**
         * \brief Set a new id for the Identifiable
         * \param id Sets the id of the Identifiable
         */
        void setId(const std::string& id);
        /**
         * \brief Get the id of the Identifiable
         * \return A std::string containing the id of the Identifiable
         */
        [[nodiscard]] std::string getId() const;
    };

    /**
     * \forceabstract
     * \brief Exactly like Identifiable class except you can't change the Id
     *        after creation
     */
    class ProtectedIdentifiable : public Identifiable
    {
    private:
        using Identifiable::setId;

    public:
        /**
         * \nobind
         */
        using Identifiable::getId;
        /**
         * \nobind
         */
        using Identifiable::Identifiable;
    };
} // namespace obe::Types
