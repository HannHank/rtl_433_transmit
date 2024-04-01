// Used demodulation code from https://triq.org/
#include <algorithm>
#include <cctype>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;

class S {
public:
  S(string t = "") { fromString(t); }

  void fromString(string t) {
    t.erase(std::remove_if(t.begin(), t.end(), ::isspace), t.end());
    line = t;
    index = 0;
  }

  bool hasNibble() { return index + 1 <= line.length(); }

  bool hasByte() { return index + 2 <= line.length(); }

  bool hasWord() { return index + 4 <= line.length(); }

  int peekNibble() { return std::stoi(line.substr(index, 1), nullptr, 16); }

  int peekByte() { return std::stoi(line.substr(index, 2), nullptr, 16); }

  int peekWord() { return std::stoi(line.substr(index, 4), nullptr, 16); }

  int getNibble() {
    int t = std::stoi(line.substr(index, 1), nullptr, 16);
    index += 1;
    return t;
  }

  int getByte() {
    int t = std::stoi(line.substr(index, 2), nullptr, 16);
    index += 2;
    return t;
  }

  int getWord() {
    int t = std::stoi(line.substr(index, 4), nullptr, 16);
    index += 4;
    return t;
  }

  void pushNibble(int t) { line += std::to_string(t); }

  void pushByte(int t) { line += std::to_string(t); }

  void pushWord(int t) { line += std::to_string(t); }
  int getIndex() { return index; }
  void setIndex(int idx) { index = idx; }

private:
  string line;
  int index;
};

int getSizeCode(string data) {
  S e(data);
  if (e.getByte() != 0xAA) {
    cout << "Invalid header" << endl;
    return 0;
  }

  int n = e.getByte();
  int l, a, o;
  cout << "n: " << n << endl;
  if (n == 0xB1) {
    a = e.getByte();
    // we need to set repetitions to 1
    o = 1;
  } else if (n == 0xB0) {
    l = e.getByte();
    a = e.getByte();
    o = e.getByte();
  } else {
    cout << "Invalid header" << endl;
    return 0;
  }

  int r[a];
  for (int g = 0; g < a; ++g)
    r[g] = e.getWord();

  bool c = false;
  int f = e.getIndex();
  while (e.hasNibble()) {
    if (e.getNibble() > 7) {
      c = true;
      break;
    }
  }
  e.setIndex(f);
  int size = 0;
  while (e.hasNibble() && e.peekByte() != 0x55) {
    int g = e.getNibble();
    size++;
  }
  cout << "Size inside: " << size << endl;
  return size * o;
}
int *getCodePulses(string t, int *s) {
  S e(t);
  S e_copy(t);

  if (e.getByte() != 0xAA) {
    return s;
  }

  int n = e.getByte();
  int l, a, o;
  if (n == 0xB1)
    a = e.getByte();
  else if (n == 0xB0) {
    l = e.getByte();
    a = e.getByte();
    o = e.getByte();
  } else {
    return s;
  }

  int r[a];
  for (int g = 0; g < a; ++g)
    r[g] = e.getWord();

  bool c = false;
  int f = e.getIndex();
  while (e.hasNibble()) {
    if (e.getNibble() > 7) {
      c = true;
      break;
    }
  }
  e.setIndex(f);
  if (!c) {
    s[0] = 0;
    int i = 1;
    while (e.hasNibble() && e.peekByte() != 0x55) {
      int g = e.getNibble();
      if (g > a)
        break;
      s[i++] = r[g];
    }
    s[i++] = 0;
    if (o) {
      int repeats = o;
      for (int j = 0; j < repeats; j++) {
        for (int k = 0; k < i; k++) {
          s[i * j + k] = s[k];
        }
      }
    }
    return s;
  }

  bool d = true;
  int i = 0;
  while (e.hasNibble() && e.peekByte() != 0x55) {
    int g = e.getNibble();
    int b = g & 7;
    if (b > a)
      break;
    if (g & 8) {
      if (!d)
        s[i++] = 0;
      s[i++] = r[b];
      d = false;
    } else {
      if (d)
        s[i++] = 0;
      s[i++] = r[b];
      d = true;
    }
  }

  if (!d)
    s[i++] = 0;

  if (o) {
    int repeats = o;
    for (int j = 0; j < repeats; j++) {
      for (int k = 0; k < i; k++) {
        s[i * j + k] = s[k];
      }
    }
  }
  return s;
}

int main(int argc, char *argv[]) {
  if (argc < 1) {
    cout << "Usage: " << argv[0] << " \"AA...\"" << endl;
    return 1;
  }
  vector<int> pulses;
  for (int i = 1; i < argc; i++) {
    string data = argv[i];
    // split string by +
    char *p = strtok((char *)data.c_str(), "+");
    while (p != NULL) {
      int size = getSizeCode(data);
      int s[size];
      getCodePulses(data, s);
      cout << "Size: " << size << endl;
      cout << "Pulses: ";
      for (int j = 0; j < size; j++) {
        pulses.push_back(s[j]);
        cout << s[j] << " ";
      }
      cout << endl;
      p = strtok(NULL, "+");
    }
    for (auto i : pulses) {
      cout << i << " ";
    }
    cout << endl;
  }

  // open header file and override content
  ofstream pulse_file;
  pulse_file.open("../pulse.h", ios::out);
  if(!pulse_file.is_open()){
		  cout << "Unable to open file" << endl;
  }
  // vector into string seperated by comma 
  std::stringstream ss;
  for(int i = 0; i < pulses.size() ; i++){
		  // stop before the last element
		  ss << pulses.at(i);
		  if(i < pulses.size()-1){
		     ss << ",";
		  }

  }
  pulse_file << "const int pulses[] = {" << ss.str() << "};" ;
  pulse_file.close();
}
