#pragma once

#include "pch.h"

#include "SimpleGTA.h"

class Vehicle {
public:
    int hVehicle;
    CVehicle* pVehicle = NULL;

    Vehicle(int hVehicle);
	~Vehicle();

    void Update(int dt);
};