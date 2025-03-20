#include "TypeInfo.h"
TypeInfo type_info_int = {
    .compare = IntCompare,
    .destruct = IntDestructor,
    .size_element = sizeof(int)
};

TypeInfo type_info_double = {
    .compare = DoubleCompare,
    .destruct = DoubleDestructor,
    .size_element = sizeof(double)
};

TypeInfo type_info_segment = {
    .compare = SegmentCompare,
    .destruct = SegmentDesrtuctor,
    .size_element = sizeof(Segment)
};
