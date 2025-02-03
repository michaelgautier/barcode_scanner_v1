#ifndef inc_gautier_scan_config_record
#define inc_gautier_scan_config_record

#include <gtkmm.h>
#include <glibmm.h>

class scan_config_record {
    public:
        scan_config_record ( );
        scan_config_record ( const scan_config_record& config );
        bool get_scan_config_unique_barcodes();
        guint get_scan_config_items_per_container();
        guint get_scan_config_barcode_length_min();
        guint get_scan_config_barcode_length_max();
        Glib::ustring get_scan_config_container_prefix();
        Glib::ustring get_scan_config_container_suffix();
        bool get_scan_config_container_autoincrement();
        bool get_scan_config_container_autoprint();
        guint get_scan_config_export_format_typeid();

        void get_scan_config_unique_barcodes ( bool value );
        void get_scan_config_items_per_container ( guint value );
        void get_scan_config_barcode_length_min ( guint value );
        void get_scan_config_barcode_length_max ( guint value );
        void get_scan_config_container_prefix ( Glib::ustring value );
        void get_scan_config_container_suffix ( Glib::ustring value );
        void get_scan_config_container_autoincrement ( bool value );
        void get_scan_config_container_autoprint ( bool value );
        void get_scan_config_export_format_typeid ( guint value );

    private:
        bool scan_config_unique_barcodes = true;
        guint scan_config_items_per_container = 10;
        guint scan_config_barcode_length_min = 5;
        guint scan_config_barcode_length_max = 15;
        Glib::ustring scan_config_container_prefix = "CONTAINER";
        Glib::ustring scan_config_container_suffix = "OUTBOUND";
        bool scan_config_container_autoincrement = true;
        bool scan_config_container_autoprint = false;
        guint scan_config_export_format_typeid = 0;
};
#endif
