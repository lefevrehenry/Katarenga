#include "server_utils.hpp"
#include "Board.hpp"

#include <message/message_utils.hpp>

struct ServerInfo ServerInfo;

using MessageType = MessageWrapper::MessageType;

//template< typename T >
//void toto(typename T::Request*, typename T::Reply*)
//{
//    throw std::runtime_error("missing template specialization");
//}

//template<>
//void toto<BoardConfiguration>(BoardConfiguration::Request*, BoardConfiguration::Reply* reply)
//{
//    std::string boardString = ServerInfo.board->getBoard();

//    reply->setConfiguration(boardString);
//}

//template< typename T >
//void construct_reply(zmqpp::message& request_message, zmqpp::message& reply_message)
//{
//    // reconstruct the request object from the (input) request message
//    typename T::Request request;
//    request.fromMessage(request_message);

//    // construct the reply object for the (output) reply message
//    typename T::Reply reply;
//    toto<T>(&request, &reply);

//    // write the output message from the reply object
//    reply.toMessage(reply_message);
//}

//zmqpp::message process_request(zmqpp::message& request_message)
//{
//    // the message returned
//    zmqpp::message reply_message;

//    // read the header (correspond to the type of the request sent)
//    MessageType type = *request_message.get<const MessageType*>(0);

//    // according to the type of the request we construct the reply message
//    switch (type) {
//    case MessageType::AskBoardConfiguration: {
//        construct_reply<BoardConfiguration>(request_message, reply_message);
//        break;
//    }
//    case MessageType::CheckConnectivity: {
//        construct_reply<CheckConnectivity>(request_message, reply_message);
//        break;
//    }
//    case MessageType::IsThisMoveValid: {
//        break;
//    }
//    case MessageType::PlayThisMove: {
//        break;
//    }
//    case MessageType::ForgetItRageQuit: {
//        break;
//    }
//    }

//    return reply_message;
//}

/*template< typename T >
void titi(typename T::Request*, typename T::Reply*, typename T::Broadcast*)
{
    throw std::runtime_error("missing template specialization");
}

template<>
void titi<PlayThisMove>(PlayThisMove::Request* request, PlayThisMove::Reply*, PlayThisMove::Broadcast* broadcast)
{
    broadcast->setMove(request->getMove());
}

template< typename T >
zmqpp::message construct_publish(zmqpp::message& request_message, zmqpp::message& reply_message)
{
    zmqpp::message publish_message;

    // reconstruct the request object from the (input) request message
    typename T::Request request;
    request.fromMessage(request_message);

    // reconstruct the reply object from the (output) reply message
    typename T::Reply reply;
    reply.fromMessage(reply_message);

    // construct the broadcast object for the (output) broadcast message
    typename T::Broadcast broadcast;
    titi<T>(&request, &reply, &broadcast);

    // write the output message from the broadcast object
    broadcast.toMessage(reply_message);

    return publish_message;
}

zmqpp::message process_broadcast(zmqpp::message& request_message, zmqpp::message& reply_message)
{
    zmqpp::message publish_message;

    // read the header (correspond to the type of the request sent)
    MessageType type = *request_message.get<const MessageType*>(0);

    // according to the type of the request we construct the publish message
    switch (type) {
    case MessageType::PlayThisMove: {
        publish_message = construct_publish<PlayThisMove>(request_message, reply_message);
        break;
    }
    case MessageType::ForgetItRageQuit: {
        break;
    }
    default: {
        break;
    }
    }

    return publish_message;
}*/

// TODO Implement rotations of the tiles
const char bb[8][4][4] = {{{'R', 'B', 'K', 'N'}, // 0
                           {'N', 'R', 'N', 'K'},
                           {'B', 'B', 'K', 'R'},
                           {'K', 'N', 'R', 'B'}},
                          {{'R', 'B', 'N', 'K'}, // 1
                           {'N', 'B', 'R', 'B'},
                           {'K', 'K', 'N', 'R'},
                           {'B', 'R', 'K', 'N'}},
                          {{'R', 'N', 'K', 'B'}, // 2
                           {'B', 'B', 'N', 'R'},
                           {'K', 'K', 'R', 'N'},
                           {'N', 'R', 'B', 'K'}},
                          {{'R', 'N', 'K', 'B'}, // 3
                           {'B', 'B', 'K', 'R'},
                           {'K', 'N', 'R', 'N'},
                           {'N', 'R', 'B', 'K'}},
                          {{'R', 'N', 'B', 'K'}, // 4
                           {'B', 'R', 'B', 'N'},
                           {'K', 'N', 'K', 'R'},
                           {'N', 'K', 'R', 'B'}},
                          {{'R', 'B', 'N', 'K'}, // 5
                           {'N', 'B', 'R', 'B'},
                           {'K', 'K', 'N', 'R'},
                           {'B', 'R', 'K', 'N'}},
                          {{'R', 'B', 'K', 'N'}, // 6
                           {'N', 'B', 'K', 'R'},
                           {'K', 'N', 'R', 'B'},
                           {'B', 'R', 'N', 'K'}},
                          {{'R', 'B', 'K', 'N'}, // 7
                           {'N', 'B', 'N', 'R'},
                           {'K', 'K', 'R', 'B'},
                           {'B', 'R', 'N', 'K'}}};

// Generates 4 different random values between 0 and 7.
void pickRand(int *a, int *b, int *c, int *d)
{
    srand (time(NULL));
    *a = rand()%8;
    do {
        *b = rand()%8;
    } while(*b == *a);
    do {
        *c = rand()%8;
    } while(*c == *a || *c == *b);
    do {
        *d = rand()%8;
    } while(*d == *a || *d == *b || *d == *c);
}

std::string generateBoardCellTypes()
{
    int a, b, c, d;
    pickRand(&a, &b, &c, &d);

    std::string s1 = "";
    std::string s2 = "";
    for(int i = 0; i < 4; ++i) {
        for(int j = 0; j < 4; ++j) {
            s1 += bb[a][i][j];
            s2 += bb[c][i][j];
        }
        for(int j = 0; j < 4; ++j) {
            s1 += bb[b][i][j];
            s2 += bb[d][i][j];
        }
    }

    std::string boardString = s1 + s2;
    return boardString;
}

void server_msg(const std::string& msg)
{
    std::cout << "[server]" << msg << std::endl;
}
