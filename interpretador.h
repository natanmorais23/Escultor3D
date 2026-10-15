#ifndef INTERPRETADOR_H
#define INTERPRETADOR_H

#include <string>

class Sculptor;

bool interpretarArquivo(const std::string &nomeArquivo,
                        Sculptor &escultor);

#endif
