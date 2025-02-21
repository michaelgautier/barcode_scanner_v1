#ifndef inc_gautier_scan_config_record
#define inc_gautier_scan_config_record

#include "config.h"
#include <gtkmm.h>
#include <glibmm.h>
#include "scan_config_export_type.hpp"

class scan_config_record {
    public:
        scan_config_record ( );
        scan_config_record ( const scan_config_record& config );
        bool get_scan_config_unique_items();
        guint get_scan_config_items_per_container();
        guint get_scan_config_item_length_min();
        guint get_scan_config_item_length_max();
        Glib::ustring get_scan_config_container_prefix();
        Glib::ustring get_scan_config_container_suffix();
        bool get_scan_config_container_autoincrement();
        bool get_scan_config_container_autoprint();
        scan_config_export_type get_scan_config_export_format_type();

        void set_scan_config_unique_items ( bool value );
        void set_scan_config_items_per_container ( guint value );
        void set_scan_config_item_length_min ( guint value );
        void set_scan_config_item_length_max ( guint value );
        void set_scan_config_container_prefix ( Glib::ustring value );
        void set_scan_config_container_suffix ( Glib::ustring value );
        void set_scan_config_container_autoincrement ( bool value );
        void set_scan_config_container_autoprint ( bool value );
        void set_scan_config_export_format_type ( scan_config_export_type value );

    private:
        bool scan_config_unique_items = true;
        guint scan_config_items_per_container = 5;
        guint scan_config_item_length_min = 6;
        guint scan_config_item_length_max = 13;
        Glib::ustring scan_config_container_prefix = "CARGO #";
        Glib::ustring scan_config_container_suffix = "BUILD";
        bool scan_config_container_autoincrement = true;
        bool scan_config_container_autoprint = false;
        scan_config_export_type scan_config_export_format_type = scan_config_export_type::tab_delimited;
};
#endif
