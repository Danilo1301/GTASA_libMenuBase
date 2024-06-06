#pragma once

#include "pch.h"

#include "SimpleGTA.h"

class Ped {
public:
    int hPed;
    CPed* pPed = NULL;
    
    Ped(int hPed);
	~Ped();

    void Update(int dt);
};