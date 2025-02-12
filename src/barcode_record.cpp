#include "barcode_record.hpp"

Glib::RefPtr<barcode_record> barcode_record::create ( const Glib::ustring& barcode )
{
    return Glib::make_refptr_for_instance<barcode_record> ( new barcode_record (
                barcode ) );
}

