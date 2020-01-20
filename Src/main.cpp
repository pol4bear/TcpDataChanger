#include <iostream>
#include "LogManager.h"
#include "TcpDataChanger.h"

using namespace std;
using namespace pol4b;

static void on_error(int error_code) {
    switch(error_code) {
    case NetfilterManager::Error::NFQ_LOSING_PACKETS:
        LogManager::on_error("Losing Packets");
        break;
    }
}

int main(int argc, char *argv[]) {
    LogManager lm(argv[0]);
    uint16_t queue_number = NetfilterManager::DEFAULT_QUEUE_NUM;
    switch(argc) {
    case 1: break;
    case 2: queue_number = atoi(argv[1]); break;
    default: LogManager::on_fatal("Usage: " + string(argv[0]) + " [Netfilter Queue Number]");
    }

    TcpDataChanger data_changer(on_error);
    data_changer.word_map["hello"] = "Hello2";
    data_changer.word_map["hacking"] = "Hack";

    try {
        data_changer.start(queue_number);
    } catch(int error_code) { on_error(error_code); }

    while(data_changer.is_started());

    return 0;
}
