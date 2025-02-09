#ifndef inc_gautier_barcode_record
#define inc_gautier_barcode_record

#include "config.h"
#include <gtkmm.h>
#include <glibmm.h>

class barcode_record : public Glib::Object {
    public:
        Glib::ustring value;
        static Glib::RefPtr<barcode_record> create ( const Glib::ustring& barcode ) {
            return Glib::make_refptr_for_instance<barcode_record> ( new barcode_record (
                        barcode ) );
        }
    protected:
        barcode_record ( const Glib::ustring& barcode ) {
            value = barcode;
            return;
        }
};
#endif
