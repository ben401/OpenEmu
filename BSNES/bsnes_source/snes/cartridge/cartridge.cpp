#include <snes/snes.hpp>

#include <nall/crc32.hpp>
#include <nall/sha256.hpp>

#define CARTRIDGE_CPP
namespace SNES {

#include "xml.cpp"
#include "serialization.cpp"

Cartridge cartridge;

void Cartridge::load(Mode::e cartridge_mode, const lstring &xml_list) {
  mode.i = cartridge_mode;
  region.i = Region::NTSC;
  ram_size = 0;

  has_bsx_slot   = false;
  has_superfx    = false;
  has_sa1        = false;
  has_necdsp     = false;
  has_srtc       = false;
  has_sdd1       = false;
  has_spc7110    = false;
  has_spc7110rtc = false;
  has_cx4        = false;
  has_obc1       = false;
  has_st0018     = false;
  has_msu1       = false;
  has_serial     = false;

  nvram.reset();

  parse_xml(xml_list);
//print(xml_list[0], "\n\n");

  if(ram_size > 0) {
    ram.map(allocate<uint8>(ram_size, 0xff), ram_size);
    nvram.append(NonVolatileRAM("srm", ram.data(), ram.size()));
  }

  rom.write_protect(true);
  ram.write_protect(false);

  crc32 = crc32_calculate(rom.data(), rom.size());

  sha256_ctx sha;
  uint8_t shahash[32];
  sha256_init(&sha);
  sha256_chunk(&sha, rom.data(), rom.size());
  sha256_final(&sha);
  sha256_hash(&sha, shahash);

  string hash;
  foreach(n, shahash) hash << hex<2>(n);
  sha256 = hash;

  system.load();
  loaded = true;
}

void Cartridge::unload() {
  if(loaded == false) return;

  system.unload();
  rom.reset();
  ram.reset();

  loaded = false;
}

Cartridge::Cartridge() {
  loaded = false;
  unload();
}

Cartridge::~Cartridge() {
  unload();
}

}
