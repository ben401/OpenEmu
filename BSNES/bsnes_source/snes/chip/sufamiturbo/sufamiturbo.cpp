#include <snes/snes.hpp>

#define SUFAMITURBO_CPP
namespace SNES {

#include "serialization.cpp"
SufamiTurbo sufamiturbo;

void SufamiTurbo::load() {
  slotA.ram.map(allocate<uint8>(128 * 1024, 0xff), 128 * 1024);
  slotB.ram.map(allocate<uint8>(128 * 1024, 0xff), 128 * 1024);

  if(slotA.rom.data()) {
    cartridge.nvram.append(Cartridge::NonVolatileRAM( "srm", slotA.ram.data(), slotA.ram.size(), 1 ));
  } else {
    slotA.rom.map(allocate<uint8>(128 * 1024, 0xff), 128 * 1024);
  }

  if(slotB.rom.data()) {
    cartridge.nvram.append(Cartridge::NonVolatileRAM( "srm", slotB.ram.data(), slotB.ram.size(), 2 ));
  } else {
    slotB.rom.map(allocate<uint8>(128 * 1024, 0xff), 128 * 1024);
  }
}

void SufamiTurbo::unload() {
  slotA.rom.reset();
  slotA.ram.reset();
  slotB.rom.reset();
  slotB.ram.reset();
}

}
