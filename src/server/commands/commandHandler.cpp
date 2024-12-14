#include "commandHandler.hpp"

Command* CommandHandler::createCommand(std::string input) {

    if (input.back() != '\n') {
        return nullptr;
    }

    std::istringstream iss(input);
    std::string str;
    std::vector<std::string> inputs;
    while (iss >> str) { inputs.push_back(str); }

    if (inputs.size() == 3 && inputs[0] == START) {
        return new StartCommand(inputs[1], inputs[2]);
    } 
    else if (inputs.size() == 7 && inputs[0] == TRY) {
        return new TryCommand(inputs[1], inputs[2], inputs[3], inputs[4], inputs[5], inputs[6]);
    } 
    else if (inputs.size() == 2 && inputs[0] == SHOW_TRIALS) {
        return new ShowTrialsCommand(inputs[1]);
    } 
    else if (inputs.size() == 1 && inputs[0] == SCOREBOARD) {
        return new ScoreboardCommand();
    } 
    else if (inputs.size() == 2 && inputs[0] == QUIT_EXIT) {
        return new QuitExitCommand(inputs[1]);
    } 
    else if (inputs.size() == 7 && inputs[0] == DEBUG) {
        return new DebugCommand(inputs[1], inputs[2], inputs[3], inputs[4], inputs[5], inputs[6]);
    } 
    return nullptr;
}
