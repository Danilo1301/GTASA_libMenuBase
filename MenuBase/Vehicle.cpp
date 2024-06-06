#include "Vehicle.h"

#include "Log.h"
#include "Mod.h"
#include "Peds.h"


extern void* (*GetVehicleFromRef)(int);

Vehicle::Vehicle(int hVehicle)
{
    this->hVehicle = hVehicle;

    this->pVehicle = (CVehicle*)GetVehicleFromRef(hVehicle);

    Log::Level(LOG_LEVEL::LOG_BOTH) << "Vehicle constructor " << hVehicle << ", model " << "?" << std::endl;
}

Vehicle::~Vehicle()
{
    
}


void Vehicle::Update(int dt)
{
    
}