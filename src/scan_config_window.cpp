#include "scan_config_window.hpp"

scan_config_window::scan_config_window()
{
    set_title ( "Scanner Configuration" );
    set_default_size ( 600, 600 );

    set_child ( main_frame );

    main_frame.set_orientation ( Gtk::Orientation::VERTICAL );
    sameness_level_frame.set_orientation ( Gtk::Orientation::VERTICAL );
    export_format_frame.set_orientation ( Gtk::Orientation::VERTICAL );

    main_frame.append ( *Gtk::make_managed<Gtk::Label> ( "Sameness Level" ) );
    main_frame.append ( sameness_level_frame );
    main_frame.append ( *Gtk::make_managed<Gtk::Label> ( "Group Size" ) );
    main_frame.append ( items_per_container_frame );
    main_frame.append ( item_code_length_frame );
    main_frame.append ( *Gtk::make_managed<Gtk::Label> ( "Container Definition" ) );
    main_frame.append ( container_prefix_frame );
    main_frame.append ( container_suffix_frame );
    main_frame.append ( autoincrement_frame );
    main_frame.append ( autoprint_label_frame );
    main_frame.append ( *Gtk::make_managed<Gtk::Label> ( "Export Format" ) );
    main_frame.append ( export_format_frame );

    sameness_level_frame.append ( sameness_freeform );
    sameness_level_frame.append ( sameness_unique );

    items_per_container_frame.set_spacing ( 8 );
    items_per_container_frame.append ( items_per_container_label );
    items_per_container_frame.append ( items_per_container );

    item_code_length_frame.set_spacing ( 8 );
    item_code_length_frame.append ( items_code_length_label );
    item_code_length_frame.append ( items_code_length_min );
    item_code_length_frame.append ( items_code_length_max );

    container_prefix_frame.set_spacing ( 8 );
    container_prefix_frame.append ( container_prefix_label );
    container_prefix_frame.append ( container_prefix );

    container_suffix_frame.set_spacing ( 8 );
    container_suffix_frame.append ( container_suffix_label );
    container_suffix_frame.append ( container_suffix );

    autoincrement_frame.append ( autoincrement_enabled );
    autoprint_label_frame.append ( autoprint_enabled );

    export_format_frame.append ( tab_delimited_format );
    export_format_frame.append ( xml_format );
    export_format_frame.append ( edi856_format );

    sameness_freeform.set_label ( "Freeform" );
    sameness_unique.set_label ( "Unique" );
    sameness_unique.set_group ( sameness_freeform );
    sameness_unique.set_active ( true );

    items_per_container_label.set_label ( "Items/container " );
    items_per_container.set_max_length ( 4 );

    items_code_length_label.set_label ( "Item code length" );
    items_code_length_min.set_max_length ( 1 );
    items_code_length_max.set_max_length ( 2 );

    container_prefix_label.set_label ( "Prefix" );
    container_prefix.set_max_length ( 32 );

    container_suffix_label.set_label ( "Suffix " );
    container_suffix.set_max_length ( 32 );

    autoincrement_enabled.set_label ( "Auto increment container" );
    autoincrement_enabled.set_active ( true );

    autoprint_enabled.set_label ( "Auto print container label" );
    autoprint_enabled.set_active ( false );

    tab_delimited_format.set_active ( true );
    tab_delimited_format.set_label ( "Tab-delimited" );
    xml_format.set_label ( "XML" );
    xml_format.set_group ( tab_delimited_format );
    edi856_format.set_label ( "EDI 856" );
    edi856_format.set_group ( tab_delimited_format );

    return;
}

void scan_config_window::set_config ( scan_config_record config )
{
    configuration = config;

    bool scan_config_unique_items = configuration.get_scan_config_unique_items();
    guint scan_config_items_per_container = configuration.get_scan_config_items_per_container();
    guint scan_config_item_length_min = configuration.get_scan_config_item_length_min();
    guint scan_config_item_length_max = configuration.get_scan_config_item_length_max();
    Glib::ustring scan_config_container_prefix = configuration.get_scan_config_container_prefix();
    Glib::ustring scan_config_container_suffix = configuration.get_scan_config_container_suffix();
    bool scan_config_container_autoincrement = configuration.get_scan_config_container_autoincrement();
    bool scan_config_container_autoprint = configuration.get_scan_config_container_autoprint();
    scan_config_export_type scan_config_export_format_type = configuration.get_scan_config_export_format_type();

    sameness_freeform.set_active ( scan_config_unique_items == false );
    sameness_unique.set_active ( scan_config_unique_items );
    items_per_container.set_text ( Glib::ustring::format ( scan_config_items_per_container ) );
    items_code_length_min.set_text ( Glib::ustring::format ( scan_config_item_length_min ) );
    items_code_length_max.set_text ( Glib::ustring::format ( scan_config_item_length_max ) );
    container_prefix.set_text ( scan_config_container_prefix );
    container_suffix.set_text ( scan_config_container_suffix );
    autoincrement_enabled.set_active ( scan_config_container_autoincrement );
    autoprint_enabled.set_active ( scan_config_container_autoprint );
    tab_delimited_format.set_active ( scan_config_export_format_type == scan_config_export_type::tab_delimited );
    xml_format.set_active ( scan_config_export_format_type == scan_config_export_type::xml );
    edi856_format.set_active ( scan_config_export_format_type == scan_config_export_type::edi_856 );

    return;
}

scan_config_record scan_config_window::get_config()
{
    configuration.set_scan_config_unique_items ( sameness_unique.get_active() );
    configuration.set_scan_config_items_per_container ( std::stoul ( items_per_container.get_text() ) );
    configuration.set_scan_config_item_length_min ( std::stoul ( items_code_length_min.get_text() ) );
    configuration.set_scan_config_item_length_max ( std::stoul ( items_code_length_max.get_text() ) );
    configuration.set_scan_config_container_prefix ( container_prefix.get_text() );
    configuration.set_scan_config_container_suffix ( container_suffix.get_text() );
    configuration.set_scan_config_container_autoincrement ( autoincrement_enabled.get_active() );
    configuration.set_scan_config_container_autoprint ( autoprint_enabled.get_active() );

    if ( tab_delimited_format.get_active() )
    {
        configuration.set_scan_config_export_format_type ( scan_config_export_type::tab_delimited );
    }
    else if ( xml_format.get_active() )
    {
        configuration.set_scan_config_export_format_type ( scan_config_export_type::xml );
    }
    else if ( edi856_format.get_active() )
    {
        configuration.set_scan_config_export_format_type ( scan_config_export_type::edi_856 );
    }

    return configuration;
}
