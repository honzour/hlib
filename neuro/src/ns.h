#ifndef nsH
#define nsH

#include <vector>
#include <string>

struct Neuron : public std::vector<double> {
  double potencial;
  double vystup;
  double derivace;
  double moment;
  std::vector<double> derivaceVah;
  Neuron(int pocet);
  std::string toString();
};

struct Vrstva : public std::vector<Neuron> {
  Vrstva(int pocet, int pocetPredchozi);
};

struct Sit : public std::vector<Vrstva> {
  double sumChyba;
  int krok;
  Sit(std::vector<int> &pocty);
  void vypocet(const std::vector<double> & vstup,
               std::vector<double> & vystup);
  void trenink(const std::vector<std::vector<std::vector<double> > > &t,
       bool (* krok)(Sit *));
};

#endif

