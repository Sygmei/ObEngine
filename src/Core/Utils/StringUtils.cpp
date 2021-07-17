#include <algorithm>
#include <cctype>

#include <Utils/MathUtils.hpp>
#include <Utils/StringUtils.hpp>
#include <Utils/VectorUtils.hpp>

namespace obe::Utils::String
{
    std::vector<std::string> split(const std::string& str, const std::string& delimiters)
    {
        std::vector<std::string> tokens;
        std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
        std::string::size_type pos = str.find_first_of(delimiters, lastPos);
        while (std::string::npos != pos || std::string::npos != lastPos)
        {
            tokens.push_back(str.substr(lastPos, pos - lastPos));
            lastPos = str.find_first_not_of(delimiters, pos);
            pos = str.find_first_of(delimiters, lastPos);
        }
        return tokens;
    }

    int occurencesInString(const std::string& str, const std::string& occur)
    {
        int occurrences = 0;
        std::string::size_type start = 0;
        while ((start = str.find(occur, start)) != std::string::npos)
        {
            ++occurrences;
            start += occur.length();
        }
        return occurrences;
    }

    bool isStringAlpha(const std::string& str)
    {
        if (!str.empty())
            return all_of(str.begin(), str.end(), isalpha);
        return false;
    }

    bool isStringAlphaNumeric(const std::string& str)
    {
        if (!str.empty())
            return all_of(str.begin(), str.end(), isalnum);
        return false;
    }

    bool isStringInt(const std::string& str)
    {
        if (!str.empty())
        {
            if (str.substr(0, 1) == "-")
            {
                std::string withoutSign = str.substr(1);
                return all_of(withoutSign.begin(), withoutSign.end(), isdigit);
            }
            return all_of(str.begin(), str.end(), isdigit);
        }
        return false;
    }

    bool isStringFloat(const std::string& str)
    {
        std::string modifyStr = str;
        bool isFloat = false;
        if (!modifyStr.empty())
        {
            if (modifyStr.substr(0, 1) == "-")
                modifyStr = modifyStr.substr(1);
            if (occurencesInString(modifyStr, ".") == 1)
            {
                isFloat = true;
                replaceInPlace(modifyStr, ".", "");
            }
            return (all_of(modifyStr.begin(), modifyStr.end(), isdigit) && isFloat);
        }
        return false;
    }

    bool isStringNumeric(const std::string& str)
    {
        return (isStringFloat(str) || isStringInt(str));
    }

    void replaceInPlace(
        std::string& subject, const std::string& search, const std::string& replace)
    {
        size_t pos = 0;
        while ((pos = subject.find(search, pos)) != std::string::npos)
        {
            subject.replace(pos, search.length(), replace);
            pos += replace.length();
        }
    }

    std::string replace(
        std::string subject, const std::string& search, const std::string& replace)
    {
        size_t pos = 0;
        while ((pos = subject.find(search, pos)) != std::string::npos)
        {
            subject.replace(pos, search.length(), replace);
            pos += replace.length();
        }
        return subject;
    }

    bool isSurroundedBy(const std::string& string, const std::string& bet)
    {
        return (string.substr(0, bet.size()) == bet
            && string.substr(string.size() - bet.size(), bet.size()) == bet);
    }

    std::string getRandomKey(const std::string& set, const int len)
    {
        std::string r;
        for (int i = 0; i < len; i++)
            r.push_back(set.at(size_t(Math::randint(0, 100000) % set.size())));
        return r;
    }

    bool contains(const std::string& string, const std::string& search)
    {
        return (string.find(search) != std::string::npos);
    }

    bool startsWith(const std::string& string, const std::string& search)
    {
        if (string.size() < search.size())
            return false;
        return (std::mismatch(search.begin(), search.end(), string.begin()).first
            == search.end());
    }

    bool endsWith(const std::string& string, const std::string& search)
    {
        if (string.size() < search.size())
        {
            return false;
        }
        return (std::mismatch(search.rbegin(), search.rend(), string.rbegin()).first
            == search.rend());
    }

    std::size_t distance(std::string_view source, std::string_view target)
    {
        if (source.size() > target.size())
        {
            return distance(target, source);
        }

        const std::size_t min_size = source.size(), max_size = target.size();
        std::vector<std::size_t> lev_dist(min_size + 1);

        for (std::size_t i = 0; i <= min_size; ++i)
        {
            lev_dist[i] = i;
        }

        for (std::size_t j = 1; j <= max_size; ++j)
        {
            std::size_t previous_diagonal = lev_dist[0];
            ++lev_dist[0];

            for (std::size_t i = 1; i <= min_size; ++i)
            {
                const std::size_t previous_diagonal_save = lev_dist[i];
                if (source[i - 1] == target[j - 1])
                {
                    lev_dist[i] = previous_diagonal;
                }
                else
                {
                    lev_dist[i] = std::min(std::min(lev_dist[i - 1], lev_dist[i]),
                                      previous_diagonal)
                        + 1;
                }
                previous_diagonal = previous_diagonal_save;
            }
        }

        return lev_dist[min_size];
    }

    std::vector<std::string> sortByDistance(const std::string& source,
        const std::vector<std::string>& words, std::size_t limit)
    {
        std::vector<std::string> sortedByDistance = words;
        std::sort(sortedByDistance.begin(), sortedByDistance.end(),
            [source](const std::string& s1, const std::string& s2) {
                return Utils::String::distance(s1, source)
                    < Utils::String::distance(s2, source);
            });
        if (limit && !sortedByDistance.empty())
        {
            return std::vector<std::string>(sortedByDistance.begin(),
                sortedByDistance.begin() + std::min(sortedByDistance.size() - 1, limit));
        }
        else
            return sortedByDistance;
    }

    std::string quote(const std::string& source)
    {
        return "\"" + source + "\"";
    }
} // namespace obe::Utils::String
