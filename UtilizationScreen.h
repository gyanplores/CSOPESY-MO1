#pragma once

#include "Screen.h"

class UtilizationScreen : public Screen {
public:
    UtilizationScreen();

    void onEnabled() override;
    void display() override;
    void process() override;
};
