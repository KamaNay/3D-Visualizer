#ifndef DF_H
#define DF_H

#include <vector>
#include "object.h"

class DisplayFile {
private:
    std::vector<Object*> objects; // Lista de objetos gráficos

public:
    // Adiciona um objeto à lista
    void addObject(Object* obj) {
        objects.push_back(obj);
    }

    // Retorna a lista de objetos
    const std::vector<Object*>& getObjects() const {
        return objects;
    }

    // Desenha todos os objetos usando o QPainter
    void drawAll(QPainter *painter) {
        for (auto obj : objects) {
            obj->draw(painter);
        }
    }
};

#endif
