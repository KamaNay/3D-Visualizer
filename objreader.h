#ifndef OBJREADER_H
#define OBJREADER_H

#include "object3d.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>

/*
 * Leitor OBJ simples:
 * - Lê vértices e faces básicos (v, f)
 * - Normaliza a malha e desloca cada modelo para não sobrepor
*/

class OBJReader {
public:
    static Object3D* loadOBJ(const QString &filePath) {
        static int modelIndex = 0;

        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return nullptr;

        QTextStream in(&file);
        auto *obj = new Object3D(filePath.split("/").last());

        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();
            if (line.startsWith("#") || line.isEmpty()) continue;

            QStringList parts = line.split(" ", Qt::SkipEmptyParts);
            if (parts[0] == "v" && parts.size() >= 4) {
                obj->addVertex(parts[1].toDouble(), parts[2].toDouble(), parts[3].toDouble());
            } else if (parts[0] == "f" && parts.size() >= 4) {
                std::vector<int> face;
                for (int i = 1; i < parts.size(); ++i) {
                    QString vertexStr = parts[i].split("/")[0];
                    face.push_back(vertexStr.toInt());
                }
                obj->addFace(face);
            }
        }

        obj->normalizeModel();
        // posiciona na cena: lado a lado e afastado (ajuste -Z conforme sua câmera)
        obj->translate3D(modelIndex * 300.0, 0.0, -500.0);
        modelIndex++;

        return obj;
    }
};

#endif
