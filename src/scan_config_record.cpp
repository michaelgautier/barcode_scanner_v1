#include "scan_config_record.hpp"

scan_config_record::scan_config_record()
{
    return;
}

scan_config_record::scan_config_record ( const scan_config_record& config )
{
    scan_config_unique_barcodes = config.scan_config_unique_barcodes;
    scan_config_items_per_container = config.scan_config_items_per_container;
    scan_config_barcode_length_min = config.scan_config_barcode_length_min;
    scan_config_barcode_length_max = config.scan_config_barcode_length_max;
    scan_config_container_prefix = config.scan_config_container_prefix;
    scan_config_container_suffix = config.scan_config_container_suffix;
    scan_config_container_autoincrement = config.scan_config_container_autoincrement;
    scan_config_container_autoprint = config.scan_config_container_autoprint;
    scan_config_export_format_type = config.scan_config_export_format_type;

    return;
}

bool scan_config_record::get_scan_config_unique_barcodes()
{
    return scan_config_unique_barcodes;
}

guint scan_config_record::get_scan_config_items_per_container()
{
    return scan_config_items_per_container;
}

guint scan_config_record::get_scan_config_barcode_length_min()
{
    return scan_config_barcode_length_min;
}

guint scan_config_record::get_scan_config_barcode_length_max()
{
    return scan_config_barcode_length_max;
}

Glib::ustring scan_config_record::get_scan_config_container_prefix()
{
    return scan_config_container_prefix;
}

Glib::ustring scan_config_record::get_scan_config_container_suffix()
{
    return scan_config_container_suffix;
}

bool scan_config_record::get_scan_config_container_autoincrement()
{
    return scan_config_container_autoincrement;
}

bool scan_config_record::get_scan_config_container_autoprint()
{
    return scan_config_container_autoprint;
}

scan_config_export_type scan_config_record::get_scan_config_export_format_type()
{
    return scan_config_export_format_type;
}

void scan_config_record::set_scan_config_unique_barcodes ( bool value )
{
    scan_config_unique_barcodes = value;

    return;
}

void scan_config_record::set_scan_config_items_per_container ( guint value )
{
    scan_config_items_per_container = value;

    return;
}

void scan_config_record::set_scan_config_barcode_length_min ( guint value )
{
    scan_config_barcode_length_min = value;

    return;
}

void scan_config_record::set_scan_config_barcode_length_max ( guint value )
{
    scan_config_barcode_length_max = value;

    return;
}

void scan_config_record::set_scan_config_container_prefix ( Glib::ustring value )
{
    scan_config_container_prefix = value;

    return;
}

void scan_config_record::set_scan_config_container_suffix ( Glib::ustring value )
{
    scan_config_container_suffix = value;

    return;
}

void scan_config_record::set_scan_config_container_autoincrement ( bool value )
{
    scan_config_container_autoincrement = value;

    return;
}

void scan_config_record::set_scan_config_container_autoprint ( bool value )
{
    scan_config_container_autoprint = value;

    return;
}

void scan_config_record::set_scan_config_export_format_type ( scan_config_export_type value )
{
    scan_config_export_format_type = value;

    return;
}

