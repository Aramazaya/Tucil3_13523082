#include "controller/ProgramController.hpp"

int main() {
    ProgramController controller;
    controller.init();
    while (true) {
        controller.main_menu();
    }
    return 0;
}