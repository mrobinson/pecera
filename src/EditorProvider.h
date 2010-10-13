#ifndef EditorProvider_h
#define EditorProvider_h

#include <QString>

namespace Pecera {

class EditorProvider {
public:
    virtual void openFile(QString filename) {};
    virtual ~EditorProvider() {};
private:
};
}

#endif
