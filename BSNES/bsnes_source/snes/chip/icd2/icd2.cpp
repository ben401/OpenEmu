#include <snes/snes.hpp>

#define ICD2_CPP
namespace SNES {

#include "interface/interface.cpp"
#include "mmio/mmio.cpp"
#include "serialization.cpp"
ICD2 icd2;

void ICD2::Enter() { icd2.enter(); }

void ICD2::enter() {
  while(true) {
    if(scheduler.sync.i == Scheduler::SynchronizeMode::All) {
      GameBoy::system.runtosave();
      scheduler.exit(Scheduler::ExitReason::SynchronizeEvent);
    }

    if(r6003 & 0x80) {
      GameBoy::system.run();
      step(GameBoy::system.clocks_executed);
      GameBoy::system.clocks_executed = 0;
    } else {  //DMG halted
      audio.coprocessor_sample(0x0000, 0x0000);
      step(1);
    }
    synchronize_cpu();
  }
}

void ICD2::init() {
}

void ICD2::load() {
  function<uint8(unsigned)> reader(&ICD2::mmio_read, &icd2);
  function<void(unsigned, uint8)> writer(&ICD2::mmio_write, &icd2);
  bus.map(Bus::MapMode::Direct, 0x00, 0x3f, 0x2181, 0x2182, reader, writer);
  bus.map(Bus::MapMode::Direct, 0x00, 0x3f, 0x420b, 0x420b, reader, writer);
  bus.map(Bus::MapMode::Direct, 0x80, 0xbf, 0x2181, 0x2182, reader, writer);
  bus.map(Bus::MapMode::Direct, 0x80, 0xbf, 0x420b, 0x420b, reader, writer);
}

void ICD2::unload() {
}

void ICD2::power() {
  audio.coprocessor_enable(true);
  audio.coprocessor_frequency(4 * 1024 * 1024);

  reset();
}

void ICD2::reset() {
  create(ICD2::Enter, cpu.frequency / 5);

  r2181 = 0x00;
  r2182 = 0x00;

  r6000 = 0x00;
  r6003 = 0x00;
  r6004 = 0xff;
  r6005 = 0xff;
  r6006 = 0xff;
  r6007 = 0xff;
  for(unsigned n = 0; n < 16; n++) r7000[n] = 0x00;
  r7800 = 0x0000;
  mlt_req = 0;

  for(unsigned n = 0; n < 320; n++) vram[n] = 0xff;

  packetsize = 0;
  joyp_id = 3;
  joyp15lock = 0;
  joyp14lock = 0;
  pulselock = true;

  GameBoy::system.init(this);
  GameBoy::system.power();
}

}
