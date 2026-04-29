#pragma once

namespace PurrKatEngine
{
    /**
     * Makes a format string that can displays all passed variables.
     * @param names All names separated by spaces.
     * @return String of nicified names with "={}" appended to each.
     */
    inline std::string Make_Fmt(std::string_view names) {
        std::string result;

        size_t pos = 0;
        while (pos < names.size()) {
            size_t comma = names.find(',', pos);
            auto token = names.substr(pos, comma - pos);

            // trim spaces
            while (!token.empty() && token.front() == ' ') token.remove_prefix(1);
            while (!token.empty() && token.back() == ' ') token.remove_suffix(1);

            // strip "m_" prefix if present
            if (token.starts_with("m_"))
                token.remove_prefix(2);

            if (!result.empty())
                result += " ";

            result += std::string(token) + "={}";

            if (comma == std::string_view::npos)
                break;

            pos = comma + 1;
        }

        return result;
    }
}
