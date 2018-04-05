#include "register_types.h"
#include "class_db.h"
#include "number_classification.h"

void register_number_classification_types() {

        ClassDB::register_class<NumberClassification>();
}

void unregister_number_classification_types() {
   //nothing to do here
}
