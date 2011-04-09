struct CheatCode {
  bool enabled;
  array<unsigned> addr;
  array<uint8> data;

  bool operator=(string);
  CheatCode();
};

class Cheat : public linear_vector<CheatCode> {
public:
  struct Type{ enum e{ ProActionReplay, GameGenie } i; };

  bool enabled() const;
  void enable(bool);
  void synchronize();
  uint8 read(unsigned) const;
  void init();

  Cheat();
  ~Cheat();

  static bool decode(const char*, unsigned&, uint8&, Type&);
  static bool encode(string&, unsigned, uint8, Type);

private:
  uint8 *lookup;
  bool system_enabled;
  bool code_enabled;
  bool cheat_enabled;
  unsigned mirror(unsigned) const;

  static uint8 default_reader(unsigned);
  static void default_writer(unsigned, uint8);
};

extern Cheat cheat;
