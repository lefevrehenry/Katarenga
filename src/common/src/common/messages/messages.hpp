#ifndef KATARENGA_COMMON_MESSAGES_MESSAGES_HPP
#define KATARENGA_COMMON_MESSAGES_MESSAGES_HPP

// ZMQPP
#include <zmqpp/zmqpp.hpp>

// Standard Library
#include <stdexcept>
#include <string>

/**
 * @brief The Message class
 */
class Message
{
    static int pid;

public:
    template<typename M>
    static int Id() {
        static int _id = pid++;
        return _id;
    }

    template<typename M>
    static std::string Name() {
        const char* _name = typeid(M).name();
        return _name;
    }

    template<typename M, class P = typename M::Parameters>
    static zmqpp::message Create(P& parameters)
    {
        zmqpp::message message;

        int id = Message::Id<M>();

        // the first part of the message is the id of M
        message.add(id);
        // the second part of the message is the payload of M
        message.add_raw<P>(&parameters, sizeof(P));

        return message;
    }

    template<typename M, class ... Args>
    static zmqpp::message Create(Args&&... args)
    {
        using P = typename M::Parameters;
        P parameters = { std::forward<Args>(args)... };

        return Create<M>(parameters);
    }

    template<typename M, class P = typename M::Parameters>
    static P Payload(const zmqpp::message& message)
    {
        int id;

        // read the id of the message
        message.get(id, 0);

        // check the message id match the template M
        if (id != Message::Id<M>())
            throw std::runtime_error("the message does not match the target '" + Message::Name<M>() + "'");

        // read the payload of the message as parameters of M
        const P* ptr = static_cast<const P*>(message.raw_data(1));

        return *ptr;
    }
};

//struct MovePlayer {
//    struct Parameters {
//        int a;
//    };
//};

//struct GameStatus {
//    struct Parameters {
//    };
//};

struct NewConnection
{
    struct Request {
        struct Parameters {
            const char* name;
            const char* ip;
            const char* port;
        };

    };

    struct Reply {
        struct Parameters {
            const char* ok;
        };
    };
};

//struct CloseConnection {
//    struct Parameters {
//    };
//};

#endif // KATARENGA_COMMON_MESSAGES_MESSAGES_HPP
