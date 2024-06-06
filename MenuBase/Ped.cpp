#include "Ped.h"

#include "Log.h"
#include "Mod.h"

extern void* (*GetPedFromRef)(int);

Ped::Ped(int hPed)
{
    this->hPed = hPed;

    this->pPed = (CPed*)GetPedFromRef(hPed);
}

Ped::~Ped()
{
    
}

void Ped::Update(int dt)
{
}