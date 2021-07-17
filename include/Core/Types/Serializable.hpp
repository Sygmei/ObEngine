#pragma once

#include <vili/node.hpp>

namespace obe::Types
{
    /**
     * \brief A Base Class that represents a vili serializable object
     */
    class Serializable
    {
    public:
        virtual ~Serializable() = default;
        Serializable& operator=(Serializable&&) = delete; //no copy nor move

        /**
         * \brief Dumps the content of the Serializable object to a ComplexNode
         */
        virtual vili::node dump() const = 0;
        /**
         * \brief Loads an object from a ComplexNode
         * \param data ComplexNode containing the data of the object
         */
        virtual void load(vili::node& data) = 0;
    };
} // namespace obe::Types
