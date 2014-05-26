#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "ns.h"

static double sigma(double x) {
  return 1.0 / (1.0 + exp(-x));
}

Neuron::Neuron(int pocet) {
	moment = 0;
    reserve(pocet + 1);
    derivaceVah.reserve(pocet + 1);
    for (int i = 0; i <= pocet; i++) {
      push_back((rand() % 0xFFFF) / (double) (0xFFFF) / (double) pocet);
      derivaceVah.push_back(0);
    }
  }

std::string Neuron::toString() {
  std::string r("N(pot, vys, der, vahy(), derVah()) = ");
  char s[1024];
  sprintf(s, "(%f, %f, %f, (", potencial, vystup, derivace);
  r += s;
  int i;
  for (i = 0; i < size(); i++) {
    sprintf(s, "%f%s", (*this)[i], (i == size() - 1 ? ")" : ", "));
    r += s;
  }
  r += ", (";
  for (i = 0; i < size(); i++) {
    sprintf(s, "%f%s", (*this).derivaceVah[i], (i == size() - 1 ? "))" : ", "));
    r += s;
  }

  return r;
}

Vrstva::Vrstva(int pocet, int pocetPredchozi) {
  for (int i = 0; i < pocet; i++) {
    push_back(Neuron(pocetPredchozi));
  }
}

Sit::Sit(std::vector<int> &pocty) {
  if (pocty.size() < 2) return;
  reserve(pocty.size() - 1);
  for (int i = 1; i < pocty.size(); i++) {
    push_back(Vrstva(pocty[i], pocty[i - 1]));
  }
}

void Sit::vypocet(const std::vector<double> & vstup,
               std::vector<double> & vystup) {
  vystup.clear();
  if (vstup.size() != (*this)[0][0].size() - 1) {
    return;
  }
  int i, j, k;
  // 1. vrstva
  for (j = 0; j < (*this)[0].size(); j++) {
    double potencial = 0;
    for (k = 0; k < (*this)[0][j].size() - 1; k++) {
      potencial += vstup[k] * (*this)[0][j][k];
    }
    potencial += -1 * (*this)[0][j][k];
    (*this)[0][j].potencial = potencial;
    (*this)[0][j].vystup = sigma(potencial);
  }
 // ostatní vrstvy
  for (i = 1; i < size(); i++) {
    for (j = 0; j < (*this)[i].size(); j++) {
      double potencial = 0;
      for (k = 0; k < (*this)[i][j].size() - 1; k++) {
        potencial +=  (*this)[i - 1][k].vystup * (*this)[i][j][k];
      }
      potencial += -1 * (*this)[i][j][k];
      (*this)[i][j].potencial = potencial;
      (*this)[i][j].vystup = sigma(potencial);
    }
  }
  vystup.reserve((*this)[size() - 1].size());
  for (i = 0; i < (*this)[size() - 1].size(); i++) {
    vystup.push_back((*this)[this->size() - 1][i].vystup);
  }
}

void Sit::trenink(const std::vector<std::vector<std::vector<double> > > &t,
     bool (* krok)(Sit *)) {
  int i, j, k, l;
  this->krok = 0;
  while (1) {
    (this->krok)++;
    sumChyba = 0;
    for (i = 0; i < size(); i++) {
      for (j = 0; j < (*this)[i].size(); j++) {
        for (k = 0; k < (*this)[i][j].size(); k++) {
          (*this)[i][j].derivaceVah[k] = 0;
          (*this)[i][j].moment = 0;
        }
      }
    }
    std::vector<double> vystup;
    for (i = 0; i < t.size(); i++) {
      vypocet(t[i][0], vystup);
      for (j = 0; j < vystup.size(); j++) {
        sumChyba += ((vystup[j] - t[i][1][j]) * (vystup[j] - t[i][1][j]));
      }
      for (j = size() - 1; j >= 0; j--) { // backpropagation
        for (k = 0; k < (*this)[j].size(); k++) {
          if (j == size() - 1) {
            (*this)[j][k].derivace = (*this)[j][k].vystup - t[i][1][k];
          } else {
            (*this)[j][k].derivace = 0;
            for (l = 0; l < (*this)[j + 1].size(); l++) {
              (*this)[j][k].derivace += 
               (*this)[j + 1][l].derivace *
               (*this)[j + 1][l].vystup *
               (1 - (*this)[j + 1][l].vystup) *
               (*this)[j + 1][l][k];
            }
          }
          for (l = 0; l < (*this)[j][k].size(); l++) {
            (*this)[j][k].derivaceVah[l] +=
              (*this)[j][k].derivace *
              (*this)[j][k].vystup *
              (1 - (*this)[j][k].vystup) *
              ((l == (*this)[j][k].size() - 1) ? -1 : 
                (j == 0 ? t[i][0][l] : (*this)[j - 1][l].vystup));
          }
        }
      }
    }
    for (i = 0; i < size(); i++) {
      for (j = 0; j < (*this)[i].size(); j++) {
        for (k = 0; k < (*this)[i][j].size(); k++) {
          (*this)[i][j].moment = 
            0.01 * (*this)[i][j].derivaceVah[k] + 0.0 * (*this)[i][j].moment;
          (*this)[i][j][k] -= (*this)[i][j].moment;
        }
      }
    }
    sumChyba /= 2;
    if (!krok(this)) {
     return;
    }
  }
}

