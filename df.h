#ifndef DF_H
#define DF_H

#include <vector>
#include "object.h"

class DisplayFile {
private:
    std::vector<Object*> objects;

public:
    void addObject(Object* obj) {
        objects.push_back(obj);
    }

    const std::vector<Object*>& getObjects() const {
        return objects;
    }

    void drawAll(QPainter *painter) {
        for (auto obj : objects) {
            obj->draw(painter);
        }
    }
};

#endif
