#include "ErrorHandler.hpp"

namespace aube
{
	ErrorMessage::ErrorMessage(std::string file, std::vector<std::string> location, std::string message, std::vector<std::string> hints)
	{
		m_file = file;
		m_location = location;
		m_message = message;
		m_hints = hints;
	}

	std::string ErrorMessage::getFile() const
	{
		return m_file;
	}

	std::string ErrorMessage::getLocation()
	{
		std::string stringLocation = m_location[0];
		for (int i = 1; i < m_location.size(); i++)
		{
			stringLocation += "::" + m_location[i];
		}
		return stringLocation;
	}

	std::string ErrorMessage::getMessage() const
	{
		return m_message;
	}

	std::vector<std::string> ErrorMessage::getHints() const
	{
		return m_hints;
	}

	std::map<std::string, ErrorMessage*> ErrorHandler::m_errors = std::map<std::string, ErrorMessage*>();
	void ErrorHandler::Load(std::string errorId, std::string filename, std::vector<std::string> location, std::string message, std::vector<std::string> hints)
	{
		m_errors[errorId] = new ErrorMessage(filename, location, message, hints);
	}

	std::exception ErrorHandler::Raise(std::string errorId, std::map<std::string, std::string> parameters)
	{
		if (m_errors.find(errorId) != m_errors.end())
		{
			ErrorMessage* currentError = m_errors[errorId];
			std::string errorMessage = "[" + errorId + "]{" + currentError->getFile() + "}";
			errorMessage += "<" + currentError->getLocation() + "> : " + currentError->getMessage();
			for (std::pair<std::string, std::string> parameter : parameters)
			{
				errorMessage = errorMessage.replace(errorMessage.find("%" + parameter.first + "%"), parameter.first.size() + 2, parameter.second);
			}
			std::cerr << errorMessage << std::endl;
			for (std::string& hint : currentError->getHints())
			{
				std::cerr << "    > " << hint << std::endl;
			}
			std::exception returnException(errorMessage.c_str());
			return returnException;
		}
		std::cerr << "Raised Unknown Exception : " << errorId << " with parameters : " << std::endl;
		for (std::pair<std::string, std::string> parameter : parameters)
		{
			std::cerr << "    > " << parameter.first << " = " << parameter.second << std::endl;
		}
		std::exception returnException(errorId.c_str());
		return returnException;
	}
}
