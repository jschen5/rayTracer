#include "BoundBox.h"

BoundBox::BoundBox(void)
{
}

BoundBox::BoundBox(glm::vec3 min, glm::vec3 dv) {
    this->boxMin = min ;
    this->dVector = dv ;
}

BoundBox::~BoundBox(void)
{
}
