#pragma once

namespace PurrKatEngine
{
    // ------- GLM Extensions --------
    inline std::string to_string(const glm::mat4& mat)
    {
        return std::to_string(mat[0][0]) + ", " + std::to_string(mat[0][1]) + ", " + std::to_string(mat[0][2]) + ", " + std::to_string(mat[0][3]) + "\n" +
               std::to_string(mat[1][0]) + ", " + std::to_string(mat[1][1]) + ", " + std::to_string(mat[1][2]) + ", " + std::to_string(mat[1][3]) + "\n" +
               std::to_string(mat[2][0]) + ", " + std::to_string(mat[2][1]) + ", " + std::to_string(mat[2][2]) + ", " + std::to_string(mat[2][3]) + "\n" +
               std::to_string(mat[3][0]) + ", " + std::to_string(mat[3][1]) + ", " + std::to_string(mat[3][2]) + ", " + std::to_string(mat[3][3]);
    }
    inline std::string format_as(const glm::mat4& mat) { return to_string(mat); }
    
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
