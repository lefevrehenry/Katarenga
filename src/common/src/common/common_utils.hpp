#ifndef KATARENGA_COMMON_COMMON_UTILS_HPP
#define KATARENGA_COMMON_COMMON_UTILS_HPP

/**
 * @brief The Common struct
 */
struct Common
{
    using GameId = unsigned int;

    enum class GameActor {
        White,
        Black,
        Default
    };
};

#endif // KATARENGA_COMMON_COMMON_UTILS_HPP
